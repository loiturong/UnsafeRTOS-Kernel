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

/* -------- 		  Define             	-------- */

/* -------- 		  Types             	-------- */
struct list {
	tcb_t *head;
	tcb_t *tail;
};

/* -------- Objects:     Global Object          -------- */
tcb_t *g_p_task_current;

/* -------- Objects:     Static Obejct          -------- */
static tcb_t *s_p_task_next;

struct list s_task_list;
struct list s_wait_list;

/* -------- Function:   Static Function         -------- */
static inline void tasklist__create(tcb_t *p_node);
static inline void tasklist__insert(tcb_t *p_node);
static inline void tasklist__remove(tcb_t *p_node);

static inline void waitlist__create(tcb_t *head);
static inline void waitlist__insert(tcb_t *item, uint32_t ticks);
static inline tcb_t *waitlist__remove(void);

/* -------- Function:      Public API           -------- */

/* -------- Function: Public Internal API       -------- */
int scheduler_pick_new_task(void)
{
	if (s_p_task_next == NULL)	// Unreachable for now
		return 0;
	//if (g_p_task_current == s_p_task_next)
	//	return 0;
	while (s_p_task_next->status != RUNNING)
		s_p_task_next = s_p_task_next->next;

	g_p_task_current = s_p_task_next;
	s_p_task_next = s_p_task_next->next;

	return 1;
}

void scheduler_update_wait_list(void)
{
	tcb_t *p_tsk;
	while ((s_wait_list.head != NULL) && (s_wait_list.head->delayed == 0)) {
		p_tsk = waitlist__remove();
		tasklist__insert(p_tsk);
		p_tsk->status = RUNNING;
	}

	if ((s_wait_list.head != NULL) && (s_wait_list.head->delayed > 0)) {
		--s_wait_list.head->delayed;
	}

	return;
}

void scheduler_register_task_static(tcb_t *p_process_block)
{
	tcb_t *p_task_node = (tcb_t *)p_process_block;
	if(s_task_list.head == NULL) {
		tasklist__create(p_task_node);
	} else {
		tasklist__insert(p_task_node);
	}
	return;
}

void scheduler_delayed_task(tcb_t *p_tsk, uint32_t ticks)
{
	tcb_t *p_node = (p_tsk == NULL) ? g_p_task_current : (tcb_t *)p_tsk;
	tasklist__remove(p_node);
	p_node->status = WAIT;

	if (s_wait_list.head == NULL) {
		p_node->delayed = ticks;
		waitlist__create(p_node);
	} else {
		waitlist__insert(p_node, ticks);
	}
	return;
}

/* -------- Function: Static Implementation     -------- */
static inline void tasklist__create(tcb_t *p_node)
{
	s_task_list.head = p_node;
	s_task_list.tail = p_node;

	g_p_task_current = s_task_list.head;
	s_p_task_next = s_task_list.head;

	s_task_list.head->prev = s_task_list.tail;
	s_task_list.tail->next = s_task_list.head;
}

static inline void tasklist__insert(tcb_t *p_node)
{
	s_task_list.tail->next = p_node;
	s_task_list.head->prev = p_node;
	p_node->prev = s_task_list.tail;
	p_node->next = s_task_list.head;

	s_task_list.tail = p_node;
}

static inline void tasklist__remove(tcb_t *p_node)
{
	// Currently called from task delayed - include case where the p_node is the current task
	// After this, a context switch is performed, so comment this out for now.
	// if (p_node == g_p_task_current)
	//	return;
	if (p_node == s_task_list.head)
		s_task_list.head = s_task_list.head->next;
	if (p_node == s_task_list.tail)
		s_task_list.tail = s_task_list.tail->prev;
	if (p_node == s_p_task_next)
		s_p_task_next = s_p_task_next->next;

	tcb_t *temp = p_node->prev;
	p_node->next->prev = temp;
	temp->next = p_node->next;
}

static inline void waitlist__create(tcb_t *head)
{
	s_wait_list.head = head;
	s_wait_list.tail = head;
	s_wait_list.head->next = s_wait_list.tail;
	s_wait_list.tail->prev = s_wait_list.head;
}

static inline void waitlist__insert(tcb_t *item, uint32_t ticks)
{
	tcb_t *index = s_wait_list.head;
	while ((index != s_wait_list.tail) && (ticks > index->delayed)) {
		ticks -= index->delayed;
		index = index->next;
	}

	if ((index == s_wait_list.tail) && (ticks >= s_wait_list.tail->delayed)) {
		item->delayed = ticks - s_wait_list.tail->delayed;

		item->prev = s_wait_list.tail;
		item->next = s_wait_list.head;

		s_wait_list.tail->next = item;
		s_wait_list.head->prev = item;
		
		s_wait_list.tail = item;
	} else {
		item->delayed   = ticks;
		index->delayed -= ticks;

		item->prev = index->prev;
		item->next = index;
		
		index->prev->next = item;
		index->prev = item;

		if (index == s_wait_list.head)
			s_wait_list.head = item;
	}
	return;
}

static inline tcb_t *waitlist__remove(void)
{
	tcb_t *rn = s_wait_list.head;
	if (s_wait_list.head == s_wait_list.tail) {
		s_wait_list.head = NULL;
		s_wait_list.tail = NULL;
	} else {
		s_wait_list.tail->next = s_wait_list.head->next;
		s_wait_list.head->next->prev = s_wait_list.tail;
		s_wait_list.head = s_wait_list.head->next;
	}
	return rn;
}

