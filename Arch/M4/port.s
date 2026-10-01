.syntax unified
/* TODO: Adding task state indicating the use of hardware floating point in context swithcing */

.section .text.exceptions

.global SVCall_Handler
.type SVCall_Handler, %function

SVCall_Handler:
	bx lr

.global PendSV_Handler
.type PendSV_Handler, %function
/* When PendSV is servered, we are always in the state of tail-chaining
 * Which is: SysTick_Handler load EXEC_RETURN to PC, the core check for pending
 * Exception, and if there is no other higher priority exception preempting, PendSV
 * get serverd. The state of PendSV now is:
 * - We are in Handler mode, using MSP as stack pointer
 * - Context before SysTick is save to task PSP
 * to save task context, we can not use push instruction. Instead, we must manually get
 * the PSP from task, save r4-r11 to that pointer, using write operation to address.
 * Then calling scheduler scheme to pick next task, which update tasklistcurrent pointer.
 * Then do the same as SVCall_Handler to switch to new task.
 */
PendSV_Handler:
	/* Handle Load first task */
	mrs r0, psp
	cmp r0, 0x00
	beq .load_first_task

	/* Save current task context */
	mrs r0, psp

	sub r0,  0x20
	str r4,  [r0, 0x1c]
	str r5,  [r0, 0x18]
	str r6,  [r0, 0x14]
	str r7,  [r0, 0x10]
	str r8,  [r0, 0x0c]
	str r9,  [r0, 0x08]
	str r10, [r0, 0x04]
	str r11, [r0, 0x00]
	
	ldr r2, =g_p_task_current	// pointer to node_t current task pointer

	ldr r1, [r2]			// node_t
	str r0, [r1]			// store node_t->tcb (first member is already stack pointer)
	
	/* Update current task pointer */
	push {r0-r2, lr}
	bl scheduler_pick_new_task	// Incase of 0 return, its means this function is not pick new task (handle latter)
	pop {r0-r2, lr}

	/* Switch to new task */
	ldr r1, [r2]			// node_t
	ldr r0, [r1]			// node_t->p_tcb (stack is the first member)

	/* "restore" context of new task */
	ldr r11, [r0, 0x00]
	ldr r10, [r0, 0x04]
	ldr r9,  [r0, 0x08]
	ldr r8,  [r0, 0x0c]
	ldr r7,  [r0, 0x10]
	ldr r6,  [r0, 0x14]
	ldr r5,  [r0, 0x18]
	ldr r4,  [r0, 0x1c]
	add r0, 0x20
	
	msr psp, r0

.ret_from_isr:
	orr lr, lr, 0x0D
	bx lr

.load_first_task:
	ldr r1, =g_p_task_current	// pointer to node_t
	ldr r1, [r1]			// node_t
	ldr r0, [r1]			// node_t->tcb (first member is already stack pointer)

	ldr r11, [r0, 0x00]
	ldr r10, [r0, 0x04]
	ldr r9,  [r0, 0x08]
	ldr r8,  [r0, 0x0c]
	ldr r7,  [r0, 0x10]
	ldr r6,  [r0, 0x14]
	ldr r5,  [r0, 0x18]
	ldr r4,  [r0, 0x1c]
	add r0, 0x20

	msr psp, r0
	
	b .ret_from_isr

/* literal pool */
	.balign 4
.literal_pool_pendsv:
	.ltorg

