/**
 * @file    : scheduler.c
 * @brief   : round-robin scheduler
 *
 * @Author  : Loiturong
 * @License : GNU GENERAL PUBLIC LICENSE
 */

/* -------- Include: Compiler Static Library    -------- */

/* -------- Include:   Public API Include       -------- */
#include "kernel.h"

/* -------- Include: Kernel Modules Include     -------- */
#include "scheduler.h"

#include "portable.h"

/* -------- 		  Define             	-------- */

/* -------- 		  Types             	-------- */
struct list {
	node_t *head;
	node_t *tail;
};

/* -------- Objects:     Global Object          -------- */
node_t *g_p_task_current;

/* -------- Objects:     Static Obejct          -------- */
struct list s_task_list;
struct list s_wait_list;

/* -------- Function:   Static Function         -------- */
// static void tasklist__remove(node_t *p_node);

static void waitlist__create(node_t *head);
static void waitlist__insert(node_t *item, uint32_t ticks);
static node_t *waitlist__remove(void);

/* -------- Function:      Public API           -------- */

/* -------- Function: Public Internal API       -------- */
int scheduler_pick_new_task(void)
{
	if (g_p_task_current == g_p_task_current->next)
		return 0;
	while (g_p_task_current->next->tcb.status != RUNNING)
		g_p_task_current = g_p_task_current->next;

	g_p_task_current = g_p_task_current->next;
	return 1;
}

void scheduler_update_wait_list(void)
{
	node_t *p_tsk;
	while ((s_wait_list.head != NULL) && (s_wait_list.head->tcb.delayed == 0)) {
		p_tsk = waitlist__remove();
		p_tsk->tcb.status = RUNNING;
	}

	if ((s_wait_list.head != NULL) && (s_wait_list.head->tcb.delayed > 0)) {
		--s_wait_list.head->tcb.delayed;
	}

	return;
}

void scheduler_register_task_static(process_control_block_t *p_process_block)
{
	_Static_assert(
		sizeof(node_t) <= (PROCESS_BLOCK_SIZE), 
		"Space for Scheduler + TaskControlBlock is not big enough");

	node_t *p_task_node = (node_t *)p_process_block;
	if(s_task_list.head == NULL) {
		s_task_list.head = p_task_node;
		s_task_list.tail = p_task_node;

		g_p_task_current = s_task_list.head;
		
		s_task_list.head->prev = s_task_list.tail;
		s_task_list.tail->next = s_task_list.head;
	} else {
		s_task_list.tail->next = p_task_node;
		s_task_list.head->prev = p_task_node;
		p_task_node->prev = s_task_list.tail;
		p_task_node->next = s_task_list.head;

		s_task_list.tail = p_task_node;
	}
	return;
}

void scheduler_delayed_task(process_control_block_t *p_tsk, uint32_t ticks)
{
	node_t *p_node = (p_tsk == NULL) ? g_p_task_current : (node_t *)p_tsk;
	p_node->tcb.status = WAIT;

	if (s_wait_list.head == NULL) {
		p_node->tcb.delayed = ticks;
		waitlist__create(p_node);
	} else { 
		waitlist__insert(p_node, ticks);
	}
	return;
}

/* -------- Function: Static Implementation     -------- */

/* inline void tasklist__remove(node_t *p_node)
{
	// Prevent self destruct (maybe change latter)
	if (p_node == g_p_task_current)
		return;

	node_t *temp = p_node->prev;
	p_node->next->prev = temp;
	temp->next = p_node->next;
} */

inline void waitlist__create(node_t *head)
{
	s_wait_list.head = head;
	s_wait_list.tail = head;
	s_wait_list.head->wait_next = s_wait_list.tail;
	s_wait_list.tail->wait_prev = s_wait_list.head;
}

inline void waitlist__insert(node_t *item, uint32_t ticks)
{
	node_t *index = s_wait_list.head;
	while ((index != s_wait_list.tail) && (ticks > index->tcb.delayed)) {
		ticks -= index->tcb.delayed;
		index = index->wait_next;
	}

	if ((index == s_wait_list.tail) && (ticks >= s_wait_list.tail->tcb.delayed)) {
		item->tcb.delayed = ticks - s_wait_list.tail->tcb.delayed;

		item->wait_prev = s_wait_list.tail;
		item->wait_next = s_wait_list.head;

		s_wait_list.tail->wait_next = item;
		s_wait_list.head->wait_prev = item;
		
		s_wait_list.tail = item;
	} else {
		item->tcb.delayed   = ticks;
		index->tcb.delayed -= ticks;

		item->wait_prev = index->wait_prev;
		item->wait_next = index;
		
		index->wait_prev->wait_next = item;
		index->wait_prev = item;

		if (index == s_wait_list.head)
			s_wait_list.head = item;
	}
	return;
}

inline node_t *waitlist__remove(void)
{
	node_t *rn = s_wait_list.head;
	if (s_wait_list.head == s_wait_list.tail) {
		s_wait_list.head = NULL;
		s_wait_list.tail = NULL;
	} else {
		s_wait_list.tail->wait_next = s_wait_list.head->wait_next;
		s_wait_list.head->wait_next->wait_prev = s_wait_list.tail;
		s_wait_list.head = s_wait_list.head->wait_next;
	}
	return rn;
}

