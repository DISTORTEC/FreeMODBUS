/*
    FreeRTOS MCF5235 port - Copyright (C) 2006 Christian Walter.
    Copyright @ 1995-2005 Freescale Semiconductor, Inc. All rights reserved.

    FreeRTOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    FreeRTOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FreeRTOS; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes FreeRTOS, without being obliged to provide
    the source code for any proprietary components.  See the licensing section
    of http://www.FreeRTOS.org for full details of how and when the exception
    can be applied.

    ***************************************************************************
    See http://www.FreeRTOS.org for documentation, latest information, license
    and contact details.  Please ensure to read the configuration and relevant
    port sections of the online documentation.
    ***************************************************************************
 */

#ifdef _UNDERSCORE_
#define exception_handler	_exception_handler
#define mcf523x_init		_mcf523x_init
#define _start				__start
#endif

	.extern ___IPSBAR
	.extern ___SRAM
	.extern ___SP_INIT
	.extern ___SRAM_SIZE
	.extern exception_handler
	.extern mcf5xxx_wr_rambar0
	.extern mcf5xxx_wr_rambar1
	.extern mcf523x_init
	.extern _start

	.global asm_startmeup
	.global _asm_startmeup
	.global	asm_exception_handler
	.global	_asm_exception_handler
	.global cpu_cache_flush
	.global _cpu_cache_flush
	.global	mcf5xxx_wr_cacr
	.global _mcf5xxx_wr_cacr
	.global	mcf5xxx_wr_vbr
	.global	_mcf5xxx_wr_vbr
    .global asm_set_ipl
    .global _asm_set_ipl
    .global mcf5xxx_wr_cacr
    .global _mcf5xxx_wr_cacr
    .global mcf5xxx_wr_acr0
    .global _mcf5xxx_wr_acr0
    .global mcf5xxx_wr_acr1
    .global _mcf5xxx_wr_acr1
    .global mcf5xxx_wr_acr2
    .global _mcf5xxx_wr_acr2
    .global mcf5xxx_wr_acr3
    .global _mcf5xxx_wr_acr3
    .global mcf5xxx_wr_other_sp
    .global _mcf5xxx_wr_other_sp
    .global mcf5xxx_wr_other_a7
    .global _mcf5xxx_wr_other_a7
    .global mcf5xxx_wr_vbr
    .global _mcf5xxx_wr_vbr
    .global mcf5xxx_wr_macsr
    .global _mcf5xxx_wr_macsr
    .global mcf5xxx_wr_mask
    .global _mcf5xxx_wr_mask
    .global mcf5xxx_wr_acc0
    .global _mcf5xxx_wr_acc0
    .global mcf5xxx_wr_accext01
    .global _mcf5xxx_wr_accext01
    .global mcf5xxx_wr_accext23
    .global _mcf5xxx_wr_accext23
    .global mcf5xxx_wr_acc1
    .global _mcf5xxx_wr_acc1
    .global mcf5xxx_wr_acc2
    .global _mcf5xxx_wr_acc2
    .global mcf5xxx_wr_acc3
    .global _mcf5xxx_wr_acc3
    .global mcf5xxx_wr_sr
    .global _mcf5xxx_wr_sr
    .global mcf5xxx_wr_rambar0
    .global _mcf5xxx_wr_rambar0
    .global mcf5xxx_wr_rambar1
    .global _mcf5xxx_wr_rambar1
    .global mcf5xxx_wr_mbar
    .global _mcf5xxx_wr_mbar
    .global mcf5xxx_wr_mbar0
    .global _mcf5xxx_wr_mbar0
    .global mcf5xxx_wr_mbar1
    .global _mcf5xxx_wr_mbar1

	.text

/********************************************************************
 * This is the main entry point upon hard reset.
 */
asm_startmeup:
_asm_startmeup:

	move.w	#0x2700,sr

	/* Initialize IPSBAR */
	move.l	#(___IPSBAR + 1),d0
	move.l	d0,0x40000000

	/* Initialize RAMBAR1: locate SRAM and validate it */
	move.l	#(___SRAM + 0x21),d0
    .long   0x4e7b0C05      /* movec d0,RAMBAR1	*/

	/* Point Stack Pointer into SRAM temporarily */
	move.l	#(___SRAM + 0x10000),sp

	/* Initialize mcf523x periphs, etc */
	jsr		mcf523x_init

	/* Relocate Stack Pointer */
	move.l	#___SP_INIT,sp

	/* Jump to the main process */
	jmp		_start

	bra		.
	nop
	nop
	halt


/********************************************************************
/*
 * This routine is the lowest-level exception handler.
 */
asm_exception_handler:
_asm_exception_handler:

	lea     -20(sp), sp
	movem.l d0-d2/a0-a1, (sp)
	pea.l   20(sp)              /* push exception frame address */
	jsr		exception_handler
	movem.l 4(sp), d0-d2/a0-a1
	lea     24(sp), sp
	rte


/********************************************************************
 * The MCF523x cache can be configured as instruction, data or split.
 * Invalidate the entire cache.
 */
cpu_cache_flush:
_cpu_cache_flush:
	nop						/* sync */
	move.l	#0x01000000,d0	/* Invalidate the I-Cache */
	movec 	d0,cacr
	rts

/********************************************************************/
/*
 * This routines changes the IPL to the value passed into the routine.
 * It also returns the old IPL value back.
 * Calling convention from C:
 *   old_ipl = asm_set_ipl(new_ipl);
 * For the Diab Data C compiler, it passes return value thru D0.
 * Note that only the least significant three bits of the passed
 * value are used.
 */

asm_set_ipl:
_asm_set_ipl:
  link  a6,#-8
  movem.l d6-d7,(sp)

  move.w  sr,d7   /* current sr  */

  move.l  d7,d0   /* prepare return value  */
  andi.l  #0x0700,d0  /* mask out IPL  */
  lsr.l #8,d0   /* IPL   */

  move.l  8(a6),d6  /* get argument  */
  andi.l  #0x07,d6    /* least significant three bits  */
  lsl.l #8,d6   /* move over to make mask  */

  andi.l  #0x0000F8FF,d7  /* zero out current IPL  */
  or.l  d6,d7     /* place new IPL in sr   */
  move.w  d7,sr

  movem.l (sp),d6-d7
  lea   8(sp),sp
  unlk  a6
  rts

/********************************************************************/
/*
 * These routines write to the special purpose registers in the ColdFire
 * core.  Since these registers are write-only in the supervisor model,
 * no corresponding read routines exist.
 */

mcf5xxx_wr_cacr:
_mcf5xxx_wr_cacr:
    move.l  4(sp),d0
    .long   0x4e7b0002      /* movec d0,cacr   */
    nop
    rts

mcf5xxx_wr_acr0:
_mcf5xxx_wr_acr0:
    move.l  4(sp),d0
    .long   0x4e7b0004      /* movec d0,ACR0  */
    nop
    rts

mcf5xxx_wr_acr1:
_mcf5xxx_wr_acr1:
    move.l  4(sp),d0
    .long   0x4e7b0005      /* movec d0,ACR1  */
    nop
    rts

mcf5xxx_wr_acr2:
_mcf5xxx_wr_acr2:
    move.l  4(sp),d0
    .long   0x4e7b0006      /* movec d0,ACR2  */
    nop
    rts

mcf5xxx_wr_acr3:
_mcf5xxx_wr_acr3:
    move.l  4(sp),d0
    .long   0x4e7b0007      /* movec d0,ACR3  */
    nop
    rts

mcf5xxx_wr_other_sp:
_mcf5xxx_wr_other_sp:
mcf5xxx_wr_other_a7:
_mcf5xxx_wr_other_a7:
  move.l  4(sp),d0
  .long 0x4e7b0800    /* movec d0,OTHER_A7 */
  nop
  rts

mcf5xxx_wr_vbr:
_mcf5xxx_wr_vbr:
  move.l  4(sp),d0
  .long 0x4e7b0801    /* movec d0,VBR */
  nop
  rts

mcf5xxx_wr_macsr:
_mcf5xxx_wr_macsr:
    move.l  4(sp),d0
    .long   0x4e7b0804      /* movec d0,MACSR */
    nop
    rts

mcf5xxx_wr_mask:
_mcf5xxx_wr_mask:
    move.l  4(sp),d0
    .long   0x4e7b0805      /* movec d0,MASK  */
    nop
    rts

mcf5xxx_wr_acc0:
_mcf5xxx_wr_acc0:
    move.l  4(sp),d0
    .long   0x4e7b0806      /* movec d0,ACC0  */
    nop
    rts

mcf5xxx_wr_accext01:
_mcf5xxx_wr_accext01:
    move.l  4(sp),d0
    .long   0x4e7b0807      /* movec d0,ACCEXT01  */
    nop
    rts

mcf5xxx_wr_accext23:
_mcf5xxx_wr_accext23:
    move.l  4(sp),d0
    .long   0x4e7b0808      /* movec d0,ACCEXT23  */
    nop
    rts

mcf5xxx_wr_acc1:
_mcf5xxx_wr_acc1:
    move.l  4(sp),d0
    .long   0x4e7b0809      /* movec d0,ACC1  */
    nop
    rts

mcf5xxx_wr_acc2:
_mcf5xxx_wr_acc2:
    move.l  4(sp),d0
    .long   0x4e7b080A      /* movec d0,ACC2  */
    nop
    rts

mcf5xxx_wr_acc3:
_mcf5xxx_wr_acc3:
    move.l  4(sp),d0
    .long   0x4e7b080B      /* movec d0,ACC3  */
    nop
    rts

mcf5xxx_wr_sr:
_mcf5xxx_wr_sr:
  move.l  4(sp),d0
  move.w  d0,SR
  rts

mcf5xxx_wr_rambar0:
_mcf5xxx_wr_rambar0:
    move.l  4(sp),d0
    .long   0x4e7b0C04      /* movec d0,RAMBAR0 */
    nop
    rts

mcf5xxx_wr_rambar1:
_mcf5xxx_wr_rambar1:
    move.l  4(sp),d0
    .long   0x4e7b0C05      /* movec d0,RAMBAR1 */
    nop
    rts

mcf5xxx_wr_mbar:
_mcf5xxx_wr_mbar:
mcf5xxx_wr_mbar0:
_mcf5xxx_wr_mbar0:
    move.l  4(sp),d0
    .long   0x4e7b0C0F      /* movec d0,MBAR0 */
    nop
    rts

mcf5xxx_wr_mbar1:
_mcf5xxx_wr_mbar1:
    move.l  4(sp),d0
    .long   0x4e7b0C0E      /* movec d0,MBAR1 */
    nop
    rts

	.end
