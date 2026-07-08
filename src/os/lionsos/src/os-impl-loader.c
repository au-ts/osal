#include <microkit.h>
#include <sel4/sel4.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include <elf/elf.h>
#include <vspace.h>

#include "os-shared-globaldefs.h"
#include "os-shared-module.h"
#include "os-impl-loader.h"
#include "os-shared-idmap.h"
#include "osapi-error.h"

OS_impl_module_internal_record_t OS_impl_module_table[OS_MAX_MODULES];

uintptr_t cfs_vspace_window;

/* This has to match what the Microkit SDF describes for cFS child PDs */
static seL4_Word child_microkit_irqs = 0x0;
static seL4_Word child_microkit_notifications = 0x1;
static seL4_Word child_microkit_pps = 0x1;
static seL4_Word child_microkit_ioports = 0x0;

int32 OS_LionsOS_ModuleAPI_Impl_Init(void)
{
    /* Module 0 is reserved
     * Module 1 should be the LionsOS proxy */
    OS_impl_module_table[0].elf = (elf_t) {.elfFile = NULL, .elfSize = 0, .elfClass = ELFCLASSNONE};
    OS_impl_module_table[0].pd = -1;
    OS_impl_module_table[1].elf = (elf_t) {.elfFile = NULL, .elfSize = 0, .elfClass = ELFCLASSNONE};
    OS_impl_module_table[1].pd = -1;

    for (int i = 2; i < OS_MAX_MODULES; i++)
    {
        OS_impl_module_table[i].elf = (elf_t) {.elfFile = NULL, .elfSize = 0, .elfClass = ELFCLASSNONE};
        OS_impl_module_table[i].pd = i - 2;
    }

    libvspace_init_mapping_regions(cfs_vspace_window, 0);
    return OS_SUCCESS;
}

int32 OS_ModuleLoad_Impl(const OS_object_token_t *token, const char *path)
{
    OS_impl_module_internal_record_t *impl = OS_OBJECT_TABLE_GET(OS_impl_module_table, *token);
    int32 ret = OS_ERROR;
    void *data = NULL;

    FILE *file = fopen(path, "r");

    if (file == NULL)
    {
        OS_DEBUG("Could not open file '%s'\n", path);
        goto cleanup;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    data = malloc(size);

    if (data == NULL)
    {
        OS_DEBUG("Could not allocate space for ELF '%s'\n", path);
        goto cleanup;
    }

    fread(data, size, 1, file);

    void *previous = (void*) impl->elf.elfFile;

    if (elf_newFile(data, size, &impl->elf) != 0)
    {
        OS_DEBUG("Invalid ELF file '%s'\n", path);
        goto cleanup;
    }
    else
    {
        // We're committed to loading the new ELF, so we can free the old one
        free(previous);
    }

    for (size_t i = 0; i < elf_getNumProgramHeaders(&impl->elf) && elf_getProgramHeaderType(&impl->elf, i) == PT_LOAD; i++)
    {
        char *src_addr = (char*) data + elf_getProgramHeaderOffset(&impl->elf, i);
        uintptr_t seg_vaddr = elf_getProgramHeaderVaddr(&impl->elf, i);
        size_t segment_size = elf_getProgramHeaderMemorySize(&impl->elf, i);
        size_t file_size = elf_getProgramHeaderFileSize(&impl->elf, i);
        assert(seg_vaddr % 0x1000 == 0);

        uint32_t written;

        written = libvspace_write_bytes(impl->pd, seg_vaddr, src_addr, file_size);
        assert(written == file_size);
        written = libvspace_write_bytes(impl->pd, seg_vaddr + file_size, NULL, segment_size - file_size);
        assert(written == segment_size - file_size);
    }

    // Now that the ELF is loaded, we need to find where the Microkit
    // state lives by parsing the symbol table and then write a
    // default set of values so that the PD can communicate with the outside world
    size_t symtab_index;
    const Elf64_Sym *symtab = elf_getSectionNamed(&impl->elf, ".symtab", &symtab_index);
    const char *strtab = elf_getSectionNamed(&impl->elf, ".strtab", NULL);

    for (size_t i = 0; i < elf_getSectionSize(&impl->elf, symtab_index) / sizeof(Elf64_Sym); i++)
    {
        const char *name = strtab + symtab[i].st_name;

        if (strcmp(name, "microkit_irqs") == 0)
        {
            libvspace_write_word(impl->pd, symtab[i].st_value, child_microkit_irqs);
        }
        else if (strcmp(name, "microkit_notifications") == 0)
        {
            libvspace_write_word(impl->pd, symtab[i].st_value, child_microkit_notifications);
        }
        else if (strcmp(name, "microkit_pps") == 0)
        {
            libvspace_write_word(impl->pd, symtab[i].st_value, child_microkit_pps);
        }
        else if (strcmp(name, "microkit_ioports") == 0)
        {
            libvspace_write_word(impl->pd, symtab[i].st_value, child_microkit_ioports);
        }
    }

    ret = OS_SUCCESS;

cleanup:
    if (fclose(file) != 0)
    {
        // Not fatal, but we should log failure
        OS_DEBUG("Failed to close file '%s'\n", path);
    }

    return ret;
}

int32 OS_ModuleUnload_Impl(const OS_object_token_t *token)
{
    OS_impl_module_internal_record_t *impl = OS_OBJECT_TABLE_GET(OS_impl_module_table, *token);
    microkit_pd_stop(impl->pd);
    return OS_SUCCESS;
}

int32 OS_ModuleGetInfo_Impl(const OS_object_token_t *token, OS_module_prop_t *module_prop)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_ModuleSymbolLookup_Impl(const OS_object_token_t *token, cpuaddr *address, const char *name)
{
    OS_module_internal_record_t *module = OS_OBJECT_TABLE_GET(OS_module_table, *token);
    OS_impl_module_internal_record_t *impl = OS_OBJECT_TABLE_GET(OS_impl_module_table, *token);

    if (module->module_type == OS_MODULE_TYPE_STATIC)
    {
        /* Symbol lookup not possible for static modules */
        return OS_ERROR;
    }

    size_t symtab_index;
    const Elf64_Sym *symtab = elf_getSectionNamed(&impl->elf, ".symtab", &symtab_index);
    const char *strtab = elf_getSectionNamed(&impl->elf, ".strtab", NULL);

    for (size_t i = 0; i < elf_getSectionSize(&impl->elf, symtab_index) / sizeof(Elf64_Sym); i++)
    {
        const char *entry = strtab + symtab[i].st_name;

        if (strcmp(name, entry) == 0)
        {
            *address = symtab[i].st_value;
            return OS_SUCCESS;
        }
    }

    return OS_ERROR;
}

int32 OS_SymbolLookup_Impl(cpuaddr *address, const char *name)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_SymbolTableDump_Impl(const char *filename, size_t limit)
{
    return OS_ERR_NOT_IMPLEMENTED;
}
