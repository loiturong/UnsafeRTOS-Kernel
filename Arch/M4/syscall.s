.syntax unified

.section .text.system_call
.type syscall_kernel_load_first_task, %function

/* Load first task stack only
 * as in scheduler, on setup phase, current task and next task 
 * are pointing to the same task, after first scheduler pick next task, 
 * it updates current task point to next task, and circles the next task.
 * This renders first task to run 2 times. So this function here just load the first stack,
 * pop its r4-r11 frame, waiting for PendSV to "save" them again.
 */
syscall_kernel_load_first_task:
	ldr r1, =g_p_task_current	// pointer to node_t
	ldr r1, [r1]			// node_t
	ldr r0, [r1]			// node_t->tcb (first member is already stack pointer)

	ldr r11, [r0, 0x04]
	ldr r10, [r0, 0x08]
	ldr r9,  [r0, 0x0c]
	ldr r8,  [r0, 0x10]
	ldr r7,  [r0, 0x14]
	ldr r6,  [r0, 0x18]
	ldr r5,  [r0, 0x1c]
	ldr r4,  [r0, 0x20]
	add r0, 0x20

	msr psp, r0
	
	bx lr
/* literal pool */
	.balign 4
.literal_pool_load_first_task:
	.ltorg

.section .rodata.syscall_jump_table
.global syscall_table
.type syscall_table, %object
syscall_table:
	.word syscall_kernel_load_first_task
	.word syscall_task_yield
	.word syscall_task_delay

