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
#include "portable.h"

/* -------- 		  Define             	-------- */
#define SCB_ICSR		((uintptr_t *)0xE000ED04)
#define SCB_ICSR_PENDSVSET_Msk	(1 << 28)
#define SET_PENDSV_BIT()	do { *SCB_ICSR |= SCB_ICSR_PENDSVSET_Msk; } while(0)

/* -------- 		  Types             	-------- */

/* -------- Objects:     Global Object          -------- */
extern int volatile g_wait_list_lock;

/* -------- Objects:     Static Obejct          -------- */

/* -------- Function:   Static Function         -------- */

/* -------- Function:      Public API           -------- */

/* -------- Function: Public Internal API       -------- */
void SysTick_Handler(void)
{
	if ((g_wait_list_lock == 1))
		return;

	scheduler_update_wait_list();

	// PendSV may be preempt by external interrupt
	if ((*SCB_ICSR & SCB_ICSR_PENDSVSET_Msk) != 0)
		return;
	
	SET_PENDSV_BIT();
	return;
}

void syscall_task_yield(void)
{
	SET_PENDSV_BIT();
	return;
}

void syscall_task_delay(process_control_block_t *p_tsk, uint32_t ticks)
{
	g_wait_list_lock = 1;
	scheduler_delayed_task(p_tsk, ticks);
	g_wait_list_lock = 0;
	SET_PENDSV_BIT();
}
/* -------- Function: Static Implementation     -------- */

