/**
 * @file    : kernel.h
 * @brief   : Provide Kernel Public API. Kernel will provide some incomplete data type, that
 * Application will pass this object back to Kernel API that use it. Application is
 * not supposed to know what the structure is nor could modify it.
 *
 * @Author  : Loiturong
 * @License : GNU GENERAL PUBLIC LICENSE
 */

#ifndef kernel_H
#define kernel_H

/* -------- Include: Compiler Static Library    -------- */
#include <stdint.h>
#include <stddef.h>

/* -------- Include:   Public API Include       -------- */

/* -------- Include: Kernel Modules Include     -------- */

/* -------- 		  Define             	-------- */
#define tcb_t				task_control_block_t

/* -------- 		  Types             	-------- */
typedef enum {
	RUNNING 	= 1,
	WAIT		= 2,
	SUSPENDED	= 3,
	DONE		= 4,
} task_status_t;

typedef struct tcb_t {
	uintptr_t *task_st;
	task_status_t status;
	uint32_t delayed;

	struct tcb_t *next;
	struct tcb_t *prev;
} __attribute__((aligned(sizeof(uintptr_t)))) task_control_block_t;


/* Kernel API */
void task_create(tcb_t *p_task_block, uintptr_t *p_array_stack, 
		size_t stack_size, uintptr_t *p_task_entry);
void kernel_start(void);
void task_yield(void);
void task_delay(tcb_t *p_tsk, uint32_t ticks);

#endif /* kernel_H */
