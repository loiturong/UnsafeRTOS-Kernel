/**
 * @file    : core.h
 * @brief   : Header define for Private Peripheral Bus
 *
 * @Author  : Loiturong
 * @License : GNU GENERAL PUBLIC LICENSE
 */

#ifndef CORE_H
#define CORE_H

/* -------- Include: Compiler Static Library    -------- */
#include <stdint.h>

#define	SCS_BASE		0xE000E000

/* --------    System control and ID registers  -------- */
#define SCB_ICSR		(*((uint32_t *)0xE000ED04))
#define SCB_ICSR_PENDSTCLR	(1 << 25)
#define SCB_ICSR_PENDSVCLR	(1 << 27)
#define SCB_ICSR_PENDSVSET	(1 << 28)

#define SCB_CCR			(*((uint32_t *)0xE000ED14))
#define SCB_CCR_USERSETMPEND	(1 << 2)	// Not use
#define SCB_CCR_UNALIGN_TRP	(1 << 3)
#define SCB_CCR_DIV_0_TRP	(1 << 4)	// Don't know why this is an option (should be forced)
#define SCB_CCR_STKALIGN	(1 << 9)

static inline void port__systick_clear_pend(void)	{ SCB_ICSR |= SCB_ICSR_PENDSTCLR; }
static inline void port__pendsv_set_pend(void)		{ SCB_ICSR |= SCB_ICSR_PENDSVSET; }
static inline void port__pendsv_clear_pend(void)	{ SCB_ICSR |= SCB_ICSR_PENDSVCLR; }
static inline int  port__pendsv_is_pend(void)
{ 
	return ((SCB_ICSR & SCB_ICSR_PENDSVSET) != 0); 
}

static inline void port__unalign_trap_set(void)		{ SCB_CCR |= SCB_CCR_UNALIGN_TRP; }
static inline void port__div_by_0_set(void)		{ SCB_CCR |= SCB_CCR_DIV_0_TRP; }
static inline void port__stack_align_set(void)		{ SCB_CCR |= SCB_CCR_STKALIGN; }

#define SCB_SHPR2		(*((uint32_t *)0xE000ED1C))
#define SCB_SHPR2_PRI_11_POS	24
#define SCB_SHPR2_PRI_11_MSK	(0xFF << SCB_SHPR2_PRI_11_POS)

static inline uint8_t port__svcall_pri_get(void)
{ 
	return ((SCB_SHPR2 & SCB_SHPR2_PRI_11_MSK) >> SCB_SHPR2_PRI_11_POS); 
}

/* -------- 		SysTick Region          -------- */
#define SYST_CSR		(*((uint32_t *)0xE000E010))
#define SYST_CSR_ENABLE		(1 << 0)
#define SYST_CSR_TICKINT	(1 << 1)
#define SYST_CSR_CLKSOURCE	(1 << 2)	// Just inlcude, dont use
#define SYST_CSR_COUNTFLAG	(1 << 16)	// Just include, dont use

static inline void port__systick_enable(void)		{ SYST_CSR |= SYST_CSR_ENABLE; }
static inline void port__systick_disable(void)		{ SYST_CSR &= ~(SYST_CSR_ENABLE); }
static inline void port__systick_int_enable(void)	{ SYST_CSR |= SYST_CSR_TICKINT; }
static inline void port__systick_int_disable(void)	{ SYST_CSR &= ~(SYST_CSR_TICKINT); }

/* -------- Function: Public Internal API       -------- */

// Raise execution priority to -1, meaning, only NMI and Reset can preempt
static inline void port__fault_mask(void)	{ __asm volatile ("cpsid f\n\t"); }
static inline void port__fault_unmask(void)	{ __asm volatile ("cpsie f\n\t"); }
// Raise execution priority to 0, meaning, only NMI, Hardfault, and Reset can preempt
static inline void port__prior_mask(void)	{ __asm volatile ("cpsid i\n\t"); }
static inline void port__prior_unmask(void)	{ __asm volatile ("cpsid i\n\t"); }

/* Enter critical region
 * A critical region is when kernel is handle request and needs to disable all exception that
 * has higher priority than SVCall
 */
static inline void port__critical_enter(void)
{
	uint8_t primask = port__svcall_pri_get();
	__asm volatile ("msr basepri, %0" :: "r" (primask) :);
}

static inline void port__critical_exit(void)
{
	uint8_t primask = 0;
	__asm volatile ("msr basepri, %0" :: "r" (primask) :);
}

#endif /* CORE_H */
