/**
 * @file    : portable.h
 * @brief   : Architecture Portable API.
 * This is portable for Cortex-M4, which is base on ARMv7-M Architecture
 * Reference: Arm®v7-M Architecture Reference Manual
 *
 * @Author  : Loiturong
 * @License : GNU GENERAL PUBLIC LICENSE
 */

#ifndef PORTABLE_H
#define PORTABLE_H

/* -------- Include: Compiler Static Library    -------- */

/* -------- Include:   Public API Include       -------- */

/* -------- Include: Kernel Modules Include     -------- */

/* -------- 		  Define             	-------- */

/* -------- 		  Types             	-------- */

/* -------- Objects:     Global Object          -------- */

/* -------- Objects:     Static Obejct          -------- */

/* -------- Function:   Static Function         -------- */

/* -------- Function:      Public API           -------- */

/* -------- Function: Public Internal API       -------- */
#define SYS_CALL(index)		do { \
		__asm volatile ("svc %[input]" :: [input] "i" (index) :); \
	} while(0)

/* -------- Function: Static Implementation     -------- */

#endif /* PORTABLE_H */
