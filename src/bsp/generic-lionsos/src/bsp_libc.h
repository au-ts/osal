#ifndef LIONSOS_BSP_LIBC_H
#define LIONSOS_BSP_LIBC_H

#include <microkit.h>

void OS_BSP_Initialize(void);
void OS_BSP_Handle_Notifications(microkit_channel channel);

#endif /* LIONSOS_BSP_LIBC_H */

