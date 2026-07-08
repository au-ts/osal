#pragma once

/* Cothreads for the initial, initial bootstrap, CFE_EVS, CFE_SB, CFE_ES, ES_BG_TASK, CFE_TIME, TIME_TONE_TASK, TIME_ONEHZ_TASK and CFE_TBL and LIONSOS_PROXY tasks
 * TODO: Track this file in the build system */
#define LIBMICROKITCO_MAX_COTHREADS 11

/* 64k of stack size */
#define COTHREAD_STACK_SIZE 0x10000
