/**
 * @file    : task.c
 * @brief   : task create, allocate
 *
 * @Author  : Loiturong
 * @License : GNU GENERAL PUBLIC LICENSE
 */

/* -------- Include: Compiler Static Library    -------- */

/* -------- Include:   Public API Include       -------- */
#include "kernel.h"

/* -------- Include: Kernel Modules Include     -------- */
#include "task.h"
#include "scheduler.h"
#include "heap.h"
#include "stack.h"

/* -------- 		  Define             	-------- */

/* -------- 		  Types             	-------- */

/* -------- Objects:     Global Object          -------- */

/* -------- Objects:     Static Obejct          -------- */

/* -------- Function:   Static Function         -------- */
static inline uintptr_t *task__init_context(uintptr_t *st, uintptr_t *p_task_entry);

/* -------- Function:      Public API           -------- */
void task_create_static(
	process_control_block_t *p_kernel_block,
	uintptr_t *p_array_stack, 
	size_t stack_size, 
	uintptr_t *p_task_entry
)
{
	_Static_assert(
		sizeof(struct task_control_block_t) <= TASK_CONTROL_BLOCK_SIZE, 
		"Space for TaskControlBlock is too small");

	struct task_control_block_t *p_task = (struct task_control_block_t *)p_kernel_block;

	p_task->task_st = stack_create(p_array_stack, stack_size);
	p_task->task_st = task__init_context(p_task->task_st, p_task_entry);
	p_task->status = RUNNING;
	
	scheduler_register_task_static(p_kernel_block);

	return;
}

/* -------- Function: Public Internal API       -------- */

/* -------- Function: Static Implementation     -------- */
uintptr_t *task__init_context(uintptr_t *st, uintptr_t *p_task_entry)
{
	// Align by 8-bytes
	uintptr_t alligned_st = (uintptr_t)st;
	alligned_st &= ~(0x07);
	st = (uintptr_t *)alligned_st;

	*(--st) = 0x01000000;			// xPRS
	*(--st) = (uintptr_t)p_task_entry;	// PC
	*(--st) = 0x00000000;			// LR (a task return would cause Busfault here)
	
	/* Gives each register init value for debug */
	*(--st) = 0x11110012;			// R12
	*(--st) = 0x11110003;			// R3
	*(--st) = 0x11110002;			// R2
	*(--st) = 0x11110001;			// R1
	*(--st) = 0x11110000;			// R0

	/* Also preserve space for R4-R11 which will be poped by PendSV on context switch */
	*(--st) = 0x11110004;			// R4
	*(--st) = 0x11110005;			// R5
	*(--st) = 0x11110006;			// R6
	*(--st) = 0x11110007;			// R7
	*(--st) = 0x11110008;			// R8
	*(--st) = 0x11110009;			// R9
	*(--st) = 0x11110010;			// R10
	*(--st) = 0x11110011;			// R11

	return st;
}
