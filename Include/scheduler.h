/**
 * @file    : scheduler.h
 * @brief   : 
 *
 * @Author  : Loiturong
 * @License : GNU GENERAL PUBLIC LICENSE
 */

#ifndef SCHEDULER_H
#define SCHEDULER_H

/* -------- Include: Compiler Static Library    -------- */

/* -------- Include:   Public API Include       -------- */
#include "kernel.h"

/* -------- Include: Kernel Modules Include     -------- */
void scheduler_register_task_static(tcb_t *p_process_block);
void scheduler_delayed_task(tcb_t *p_tsk, uint32_t ticks);
void scheduler_update_wait_list(void);
int scheduler_pick_new_task(void);

/* -------- 		  Types             	-------- */

/* -------- Objects:     Global Object          -------- */
extern tcb_t *g_p_task_current;

/* -------- Objects:     Static Obejct          -------- */

/* -------- Function:   Static Function         -------- */

/* -------- Function:      Public API           -------- */

/* -------- Function: Public Internal API       -------- */
void scheduler_delayed_task(tcb_t *p_tsk, uint32_t ticks);
int scheduler_pick_new_task(void);
void scheduler_update_wait_list(void);

/* -------- Function: Static Implementation     -------- */

#endif /* SCHEDULER_H */
