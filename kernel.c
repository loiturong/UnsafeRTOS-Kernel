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
#include "port.h"
#include "scheduler.h"

/* -------- Include: Kernel Modules Include     -------- */

/* -------- 		  Define             	-------- */

/* -------- 		  Types             	-------- */

/* -------- Objects:     Global Object          -------- */

/* -------- Objects:     Static Obejct          -------- */

/* -------- Function:   Static Function         -------- */

/* -------- Function:      Public API           -------- */
void kernel_start(void) { SYS_CALL(0x00); while(1); }
void task_yield(void)	{ SYS_CALL(0x01); }
void task_delay(process_control_block_t *p_tsk, uint32_t ticks)
{
	SYS_CALL(0x02);
	/* Compiler warnning */
	(void)p_tsk; (void)ticks;
}

/* -------- Function: Public Internal API       -------- */

/* -------- Function: Static Implementation     -------- */

