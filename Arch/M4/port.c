/**
 * @file    : port.c
 * @brief   : Architecture specific code
 *
 * @Author  : Loiturong
 * @License : GNU GENERAL PUBLIC LICENSE
 */

/* -------- Include: Compiler Static Library    -------- */
#include <stdint.h>

/* -------- Include:   Public API Include       -------- */
#include "scheduler.h"

/* -------- Include: Kernel Modules Include     -------- */
#include "port.h"
#include "core.h"

/* -------- 		  Define             	-------- */

/* -------- 		  Types             	-------- */

/* -------- Objects:     Global Object          -------- */

/* -------- Objects:     Static Obejct          -------- */

/* -------- Function:   Static Function         -------- */

/* -------- Function:      Public API           -------- */

/* -------- Function: Public Internal API       -------- */
void SysTick_Handler(void)
{
	scheduler_update_wait_list();

	// PendSV may be preempt by external interrupt
	if(port__pendsv_is_pend())
		return;
	port__pendsv_set_pend();
	return;
}

void syscall_task_yield(void)
{
	port__pendsv_set_pend();
	return;
}

void syscall_task_delay(process_control_block_t *p_tsk, uint32_t ticks)
{
	/* This process touched kernel shared object - scheduler task list */
	port__critical_enter();
	scheduler_delayed_task(p_tsk, ticks);
	port__critical_exit();
	/* the task itself called delay - force a context switch */
	if (p_tsk == NULL)
		port__pendsv_set_pend();
}
/* -------- Function: Static Implementation     -------- */

