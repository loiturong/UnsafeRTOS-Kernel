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
#define WORD_SIZE(x)			(x * 4)
#define TASK_CONTROL_BLOCK_SIZE		WORD_SIZE(4)
#define PROCESS_BLOCK_SIZE		TASK_CONTROL_BLOCK_SIZE + WORD_SIZE(4)
#define tcb_t				task_control_block_t

/* -------- 		  Types             	-------- */
typedef struct {
	uint8_t storage[PROCESS_BLOCK_SIZE];
} __attribute__((aligned(sizeof(uintptr_t)))) task_control_block_t;

/* -------- Objects:     Global Object          -------- */

/* -------- Objects:     Static Obejct          -------- */

/* -------- Function:   Static Function         -------- */

/* -------- Function:      Public API           -------- */
void task_create(tcb_t *p_task_block, uintptr_t *p_array_stack, 
		size_t stack_size, uintptr_t *p_task_entry);

// Syscall API
void kernel_start(void);
void task_yield(void);
void task_delay(tcb_t *p_tsk, uint32_t ticks);

/* -------- Function: Public Internal API       -------- */

/* -------- Function: Static Implementation     -------- */

#endif /* kernel_H */
