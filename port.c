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

void context_switch() 
{ 
	if(port__pendsv_is_pend())
		return;
	port__pendsv_set_pend();
	return;
}

//	void syscall_task_yield(void)
//	{
//		port__pendsv_set_pend();
//		return;
//	}

/* -------- Function: Static Implementation     -------- */

