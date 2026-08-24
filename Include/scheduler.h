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
#include "heap.h"
#include "task.h"

/* -------- Include: Kernel Modules Include     -------- */
void scheduler_register_task_static(process_control_block_t *p_process_block);
void scheduler_delayed_task(process_control_block_t *p_tsk, uint32_t ticks);
void scheduler_update_wait_list(void);
int scheduler_pick_new_task(void);

/* -------- 		  Types             	-------- */
typedef struct node_t {
	struct task_control_block_t tcb;
	// Task list
	struct node_t *next;
	struct node_t *prev;
	// Wait list
	struct node_t *wait_next;
	struct node_t *wait_prev;
} node_t;

/* -------- Objects:     Global Object          -------- */
extern node_t *g_p_task_current;

/* -------- Objects:     Static Obejct          -------- */

/* -------- Function:   Static Function         -------- */

/* -------- Function:      Public API           -------- */

/* -------- Function: Public Internal API       -------- */
void scheduler_delayed_task(process_control_block_t *p_tsk, uint32_t ticks);
int scheduler_pick_new_task(void);
void scheduler_update_wait_list(void);

/* -------- Function: Static Implementation     -------- */

#endif /* SCHEDULER_H */
