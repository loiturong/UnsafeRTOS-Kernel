/**
 * @file    : kernel.c
 * @brief   : System Interface
 *
 * @Author  : Loiturong
 * @License : GNU GENERAL PUBLIC LICENSE
 */

/* -------- Include: Compiler Static Library    -------- */

/* -------- Include:   Public API Include       -------- */
#include "kernel.h"
#include "portable.h"
#include "scheduler.h"

/* -------- Include: Kernel Modules Include     -------- */

/* -------- 		  Define             	-------- */

/* -------- 		  Types             	-------- */

/* -------- Objects:     Global Object          -------- */
int volatile g_wait_list_lock = 0;

/* -------- Objects:     Static Obejct          -------- */

/* -------- Function:   Static Function         -------- */

/* -------- Function:      Public API           -------- */
void kernel_start(void) { SYS_CALL(0x00); while(1); }
void task_yield(void)	{ SYS_CALL(0x01); }
void task_delay(process_control_block_t *p_tsk, uint32_t ticks)
{
	register uintptr_t r0 __asm__("r0") = (uintptr_t)p_tsk;
	register uintptr_t r1 __asm__("r1") = (uintptr_t)ticks;
	__asm__ volatile (
		"svc %[input]"
		:
		: [input] "i" (0x02), "r" (r0), "r" (r1)
		: "memory"
	);
}

/* -------- Function: Public Internal API       -------- */

/* -------- Function: Static Implementation     -------- */

