/************************************************************************************
 * Copyright @ 1995-2005 Freescale Semiconductor, Inc. All rights reserved          *
 *                                                                                  *
 *                                                                                  *
 *                                                                                  *
 * DESCRIPTION                                                                      *
 *   Register and bit definitions for the MCF523X.                                  *
 *                                                                                  *
 *                                                                                  *
 *                                                                                  *
 * NOTE                                                                             *
 *                                                                                  *
 *                                                                                  *
 *                                                                                  *
 * HISTORY                                                                          *
 *                                                                                  *	
 ************************************************************************************/

#ifndef __MCF523X_INTC1_H__
#define __MCF523X_INTC1_H__

/*********************************************************************
*
* Interrupt Controller 1 (INTC1)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_INTC1_IPRH         (*(vuint32*)(void*)(&__IPSBAR[0x000D00]))
#define MCF_INTC1_IPRL         (*(vuint32*)(void*)(&__IPSBAR[0x000D04]))
#define MCF_INTC1_IMRH         (*(vuint32*)(void*)(&__IPSBAR[0x000D08]))
#define MCF_INTC1_IMRL         (*(vuint32*)(void*)(&__IPSBAR[0x000D0C]))
#define MCF_INTC1_INTFRCH      (*(vuint32*)(void*)(&__IPSBAR[0x000D10]))
#define MCF_INTC1_INTFRCL      (*(vuint32*)(void*)(&__IPSBAR[0x000D14]))
#define MCF_INTC1_IRLR         (*(vuint8 *)(void*)(&__IPSBAR[0x000D18]))
#define MCF_INTC1_IACKLPR      (*(vuint8 *)(void*)(&__IPSBAR[0x000D19]))
#define MCF_INTC1_ICR0         (*(vuint8 *)(void*)(&__IPSBAR[0x000D40]))
#define MCF_INTC1_ICR1         (*(vuint8 *)(void*)(&__IPSBAR[0x000D41]))
#define MCF_INTC1_ICR2         (*(vuint8 *)(void*)(&__IPSBAR[0x000D42]))
#define MCF_INTC1_ICR3         (*(vuint8 *)(void*)(&__IPSBAR[0x000D43]))
#define MCF_INTC1_ICR4         (*(vuint8 *)(void*)(&__IPSBAR[0x000D44]))
#define MCF_INTC1_ICR5         (*(vuint8 *)(void*)(&__IPSBAR[0x000D45]))
#define MCF_INTC1_ICR6         (*(vuint8 *)(void*)(&__IPSBAR[0x000D46]))
#define MCF_INTC1_ICR7         (*(vuint8 *)(void*)(&__IPSBAR[0x000D47]))
#define MCF_INTC1_ICR8         (*(vuint8 *)(void*)(&__IPSBAR[0x000D48]))
#define MCF_INTC1_ICR9         (*(vuint8 *)(void*)(&__IPSBAR[0x000D49]))
#define MCF_INTC1_ICR10        (*(vuint8 *)(void*)(&__IPSBAR[0x000D4A]))
#define MCF_INTC1_ICR11        (*(vuint8 *)(void*)(&__IPSBAR[0x000D4B]))
#define MCF_INTC1_ICR12        (*(vuint8 *)(void*)(&__IPSBAR[0x000D4C]))
#define MCF_INTC1_ICR13        (*(vuint8 *)(void*)(&__IPSBAR[0x000D4D]))
#define MCF_INTC1_ICR14        (*(vuint8 *)(void*)(&__IPSBAR[0x000D4E]))
#define MCF_INTC1_ICR15        (*(vuint8 *)(void*)(&__IPSBAR[0x000D4F]))
#define MCF_INTC1_ICR16        (*(vuint8 *)(void*)(&__IPSBAR[0x000D50]))
#define MCF_INTC1_ICR17        (*(vuint8 *)(void*)(&__IPSBAR[0x000D51]))
#define MCF_INTC1_ICR18        (*(vuint8 *)(void*)(&__IPSBAR[0x000D52]))
#define MCF_INTC1_ICR19        (*(vuint8 *)(void*)(&__IPSBAR[0x000D53]))
#define MCF_INTC1_ICR20        (*(vuint8 *)(void*)(&__IPSBAR[0x000D54]))
#define MCF_INTC1_ICR21        (*(vuint8 *)(void*)(&__IPSBAR[0x000D55]))
#define MCF_INTC1_ICR22        (*(vuint8 *)(void*)(&__IPSBAR[0x000D56]))
#define MCF_INTC1_ICR23        (*(vuint8 *)(void*)(&__IPSBAR[0x000D57]))
#define MCF_INTC1_ICR24        (*(vuint8 *)(void*)(&__IPSBAR[0x000D58]))
#define MCF_INTC1_ICR25        (*(vuint8 *)(void*)(&__IPSBAR[0x000D59]))
#define MCF_INTC1_ICR26        (*(vuint8 *)(void*)(&__IPSBAR[0x000D5A]))
#define MCF_INTC1_ICR27        (*(vuint8 *)(void*)(&__IPSBAR[0x000D5B]))
#define MCF_INTC1_ICR28        (*(vuint8 *)(void*)(&__IPSBAR[0x000D5C]))
#define MCF_INTC1_ICR29        (*(vuint8 *)(void*)(&__IPSBAR[0x000D5D]))
#define MCF_INTC1_ICR30        (*(vuint8 *)(void*)(&__IPSBAR[0x000D5E]))
#define MCF_INTC1_ICR31        (*(vuint8 *)(void*)(&__IPSBAR[0x000D5F]))
#define MCF_INTC1_ICR32        (*(vuint8 *)(void*)(&__IPSBAR[0x000D60]))
#define MCF_INTC1_ICR33        (*(vuint8 *)(void*)(&__IPSBAR[0x000D61]))
#define MCF_INTC1_ICR34        (*(vuint8 *)(void*)(&__IPSBAR[0x000D62]))
#define MCF_INTC1_ICR35        (*(vuint8 *)(void*)(&__IPSBAR[0x000D63]))
#define MCF_INTC1_ICR36        (*(vuint8 *)(void*)(&__IPSBAR[0x000D64]))
#define MCF_INTC1_ICR37        (*(vuint8 *)(void*)(&__IPSBAR[0x000D65]))
#define MCF_INTC1_ICR38        (*(vuint8 *)(void*)(&__IPSBAR[0x000D66]))
#define MCF_INTC1_ICR39        (*(vuint8 *)(void*)(&__IPSBAR[0x000D67]))
#define MCF_INTC1_ICR40        (*(vuint8 *)(void*)(&__IPSBAR[0x000D68]))
#define MCF_INTC1_ICR41        (*(vuint8 *)(void*)(&__IPSBAR[0x000D69]))
#define MCF_INTC1_ICR42        (*(vuint8 *)(void*)(&__IPSBAR[0x000D6A]))
#define MCF_INTC1_ICR43        (*(vuint8 *)(void*)(&__IPSBAR[0x000D6B]))
#define MCF_INTC1_ICR44        (*(vuint8 *)(void*)(&__IPSBAR[0x000D6C]))
#define MCF_INTC1_ICR45        (*(vuint8 *)(void*)(&__IPSBAR[0x000D6D]))
#define MCF_INTC1_ICR46        (*(vuint8 *)(void*)(&__IPSBAR[0x000D6E]))
#define MCF_INTC1_ICR47        (*(vuint8 *)(void*)(&__IPSBAR[0x000D6F]))
#define MCF_INTC1_ICR48        (*(vuint8 *)(void*)(&__IPSBAR[0x000D70]))
#define MCF_INTC1_ICR49        (*(vuint8 *)(void*)(&__IPSBAR[0x000D71]))
#define MCF_INTC1_ICR50        (*(vuint8 *)(void*)(&__IPSBAR[0x000D72]))
#define MCF_INTC1_ICR51        (*(vuint8 *)(void*)(&__IPSBAR[0x000D73]))
#define MCF_INTC1_ICR52        (*(vuint8 *)(void*)(&__IPSBAR[0x000D74]))
#define MCF_INTC1_ICR53        (*(vuint8 *)(void*)(&__IPSBAR[0x000D75]))
#define MCF_INTC1_ICR54        (*(vuint8 *)(void*)(&__IPSBAR[0x000D76]))
#define MCF_INTC1_ICR55        (*(vuint8 *)(void*)(&__IPSBAR[0x000D77]))
#define MCF_INTC1_ICR56        (*(vuint8 *)(void*)(&__IPSBAR[0x000D78]))
#define MCF_INTC1_ICR57        (*(vuint8 *)(void*)(&__IPSBAR[0x000D79]))
#define MCF_INTC1_ICR58        (*(vuint8 *)(void*)(&__IPSBAR[0x000D7A]))
#define MCF_INTC1_ICR59        (*(vuint8 *)(void*)(&__IPSBAR[0x000D7B]))
#define MCF_INTC1_ICR60        (*(vuint8 *)(void*)(&__IPSBAR[0x000D7C]))
#define MCF_INTC1_ICR61        (*(vuint8 *)(void*)(&__IPSBAR[0x000D7D]))
#define MCF_INTC1_ICR62        (*(vuint8 *)(void*)(&__IPSBAR[0x000D7E]))
#define MCF_INTC1_ICR63        (*(vuint8 *)(void*)(&__IPSBAR[0x000D7F]))
#define MCF_INTC1_ICRn(x)      (*(vuint8 *)(void*)(&__IPSBAR[0x000D40+((x)*0x001)]))
#define MCF_INTC1_SWIACK       (*(vuint8 *)(void*)(&__IPSBAR[0x000DE0]))
#define MCF_INTC1_L1IACK       (*(vuint8 *)(void*)(&__IPSBAR[0x000DE4]))
#define MCF_INTC1_L2IACK       (*(vuint8 *)(void*)(&__IPSBAR[0x000DE8]))
#define MCF_INTC1_L3IACK       (*(vuint8 *)(void*)(&__IPSBAR[0x000DEC]))
#define MCF_INTC1_L4IACK       (*(vuint8 *)(void*)(&__IPSBAR[0x000DF0]))
#define MCF_INTC1_L5IACK       (*(vuint8 *)(void*)(&__IPSBAR[0x000DF4]))
#define MCF_INTC1_L6IACK       (*(vuint8 *)(void*)(&__IPSBAR[0x000DF8]))
#define MCF_INTC1_L7IACK       (*(vuint8 *)(void*)(&__IPSBAR[0x000DFC]))
#define MCF_INTC1_LnIACK(x)    (*(vuint8 *)(void*)(&__IPSBAR[0x000DE4+((x)*0x004)]))

/* Bit definitions and macros for MCF_INTC1_IPRH */
#define MCF_INTC1_IPRH_INT32          (0x00000001)
#define MCF_INTC1_IPRH_INT33          (0x00000002)
#define MCF_INTC1_IPRH_INT34          (0x00000004)
#define MCF_INTC1_IPRH_INT35          (0x00000008)
#define MCF_INTC1_IPRH_INT36          (0x00000010)
#define MCF_INTC1_IPRH_INT37          (0x00000020)
#define MCF_INTC1_IPRH_INT38          (0x00000040)
#define MCF_INTC1_IPRH_INT39          (0x00000080)
#define MCF_INTC1_IPRH_INT40          (0x00000100)
#define MCF_INTC1_IPRH_INT41          (0x00000200)
#define MCF_INTC1_IPRH_INT42          (0x00000400)
#define MCF_INTC1_IPRH_INT43          (0x00000800)
#define MCF_INTC1_IPRH_INT44          (0x00001000)
#define MCF_INTC1_IPRH_INT45          (0x00002000)
#define MCF_INTC1_IPRH_INT46          (0x00004000)
#define MCF_INTC1_IPRH_INT47          (0x00008000)
#define MCF_INTC1_IPRH_INT48          (0x00010000)
#define MCF_INTC1_IPRH_INT49          (0x00020000)
#define MCF_INTC1_IPRH_INT50          (0x00040000)
#define MCF_INTC1_IPRH_INT51          (0x00080000)
#define MCF_INTC1_IPRH_INT52          (0x00100000)
#define MCF_INTC1_IPRH_INT53          (0x00200000)
#define MCF_INTC1_IPRH_INT54          (0x00400000)
#define MCF_INTC1_IPRH_INT55          (0x00800000)
#define MCF_INTC1_IPRH_INT56          (0x01000000)
#define MCF_INTC1_IPRH_INT57          (0x02000000)
#define MCF_INTC1_IPRH_INT58          (0x04000000)
#define MCF_INTC1_IPRH_INT59          (0x08000000)
#define MCF_INTC1_IPRH_INT60          (0x10000000)
#define MCF_INTC1_IPRH_INT61          (0x20000000)
#define MCF_INTC1_IPRH_INT62          (0x40000000)
#define MCF_INTC1_IPRH_INT63          (0x80000000)

/* Bit definitions and macros for MCF_INTC1_IPRL */
#define MCF_INTC1_IPRL_INT1           (0x00000002)
#define MCF_INTC1_IPRL_INT2           (0x00000004)
#define MCF_INTC1_IPRL_INT3           (0x00000008)
#define MCF_INTC1_IPRL_INT4           (0x00000010)
#define MCF_INTC1_IPRL_INT5           (0x00000020)
#define MCF_INTC1_IPRL_INT6           (0x00000040)
#define MCF_INTC1_IPRL_INT7           (0x00000080)
#define MCF_INTC1_IPRL_INT8           (0x00000100)
#define MCF_INTC1_IPRL_INT9           (0x00000200)
#define MCF_INTC1_IPRL_INT10          (0x00000400)
#define MCF_INTC1_IPRL_INT11          (0x00000800)
#define MCF_INTC1_IPRL_INT12          (0x00001000)
#define MCF_INTC1_IPRL_INT13          (0x00002000)
#define MCF_INTC1_IPRL_INT14          (0x00004000)
#define MCF_INTC1_IPRL_INT15          (0x00008000)
#define MCF_INTC1_IPRL_INT16          (0x00010000)
#define MCF_INTC1_IPRL_INT17          (0x00020000)
#define MCF_INTC1_IPRL_INT18          (0x00040000)
#define MCF_INTC1_IPRL_INT19          (0x00080000)
#define MCF_INTC1_IPRL_INT20          (0x00100000)
#define MCF_INTC1_IPRL_INT21          (0x00200000)
#define MCF_INTC1_IPRL_INT22          (0x00400000)
#define MCF_INTC1_IPRL_INT23          (0x00800000)
#define MCF_INTC1_IPRL_INT24          (0x01000000)
#define MCF_INTC1_IPRL_INT25          (0x02000000)
#define MCF_INTC1_IPRL_INT26          (0x04000000)
#define MCF_INTC1_IPRL_INT27          (0x08000000)
#define MCF_INTC1_IPRL_INT28          (0x10000000)
#define MCF_INTC1_IPRL_INT29          (0x20000000)
#define MCF_INTC1_IPRL_INT30          (0x40000000)
#define MCF_INTC1_IPRL_INT31          (0x80000000)

/* Bit definitions and macros for MCF_INTC1_IMRH */
#define MCF_INTC1_IMRH_INT_MASK32     (0x00000001)
#define MCF_INTC1_IMRH_INT_MASK33     (0x00000002)
#define MCF_INTC1_IMRH_INT_MASK34     (0x00000004)
#define MCF_INTC1_IMRH_INT_MASK35     (0x00000008)
#define MCF_INTC1_IMRH_INT_MASK36     (0x00000010)
#define MCF_INTC1_IMRH_INT_MASK37     (0x00000020)
#define MCF_INTC1_IMRH_INT_MASK38     (0x00000040)
#define MCF_INTC1_IMRH_INT_MASK39     (0x00000080)
#define MCF_INTC1_IMRH_INT_MASK40     (0x00000100)
#define MCF_INTC1_IMRH_INT_MASK41     (0x00000200)
#define MCF_INTC1_IMRH_INT_MASK42     (0x00000400)
#define MCF_INTC1_IMRH_INT_MASK43     (0x00000800)
#define MCF_INTC1_IMRH_INT_MASK44     (0x00001000)
#define MCF_INTC1_IMRH_INT_MASK45     (0x00002000)
#define MCF_INTC1_IMRH_INT_MASK46     (0x00004000)
#define MCF_INTC1_IMRH_INT_MASK47     (0x00008000)
#define MCF_INTC1_IMRH_INT_MASK48     (0x00010000)
#define MCF_INTC1_IMRH_INT_MASK49     (0x00020000)
#define MCF_INTC1_IMRH_INT_MASK50     (0x00040000)
#define MCF_INTC1_IMRH_INT_MASK51     (0x00080000)
#define MCF_INTC1_IMRH_INT_MASK52     (0x00100000)
#define MCF_INTC1_IMRH_INT_MASK53     (0x00200000)
#define MCF_INTC1_IMRH_INT_MASK54     (0x00400000)
#define MCF_INTC1_IMRH_INT_MASK55     (0x00800000)
#define MCF_INTC1_IMRH_INT_MASK56     (0x01000000)
#define MCF_INTC1_IMRH_INT_MASK57     (0x02000000)
#define MCF_INTC1_IMRH_INT_MASK58     (0x04000000)
#define MCF_INTC1_IMRH_INT_MASK59     (0x08000000)
#define MCF_INTC1_IMRH_INT_MASK60     (0x10000000)
#define MCF_INTC1_IMRH_INT_MASK61     (0x20000000)
#define MCF_INTC1_IMRH_INT_MASK62     (0x40000000)
#define MCF_INTC1_IMRH_INT_MASK63     (0x80000000)

/* Bit definitions and macros for MCF_INTC1_IMRL */
#define MCF_INTC1_IMRL_MASKALL        (0x00000001)
#define MCF_INTC1_IMRL_INT_MASK1      (0x00000002)
#define MCF_INTC1_IMRL_INT_MASK2      (0x00000004)
#define MCF_INTC1_IMRL_INT_MASK3      (0x00000008)
#define MCF_INTC1_IMRL_INT_MASK4      (0x00000010)
#define MCF_INTC1_IMRL_INT_MASK5      (0x00000020)
#define MCF_INTC1_IMRL_INT_MASK6      (0x00000040)
#define MCF_INTC1_IMRL_INT_MASK7      (0x00000080)
#define MCF_INTC1_IMRL_INT_MASK8      (0x00000100)
#define MCF_INTC1_IMRL_INT_MASK9      (0x00000200)
#define MCF_INTC1_IMRL_INT_MASK10     (0x00000400)
#define MCF_INTC1_IMRL_INT_MASK11     (0x00000800)
#define MCF_INTC1_IMRL_INT_MASK12     (0x00001000)
#define MCF_INTC1_IMRL_INT_MASK13     (0x00002000)
#define MCF_INTC1_IMRL_INT_MASK14     (0x00004000)
#define MCF_INTC1_IMRL_INT_MASK15     (0x00008000)
#define MCF_INTC1_IMRL_INT_MASK16     (0x00010000)
#define MCF_INTC1_IMRL_INT_MASK17     (0x00020000)
#define MCF_INTC1_IMRL_INT_MASK18     (0x00040000)
#define MCF_INTC1_IMRL_INT_MASK19     (0x00080000)
#define MCF_INTC1_IMRL_INT_MASK20     (0x00100000)
#define MCF_INTC1_IMRL_INT_MASK21     (0x00200000)
#define MCF_INTC1_IMRL_INT_MASK22     (0x00400000)
#define MCF_INTC1_IMRL_INT_MASK23     (0x00800000)
#define MCF_INTC1_IMRL_INT_MASK24     (0x01000000)
#define MCF_INTC1_IMRL_INT_MASK25     (0x02000000)
#define MCF_INTC1_IMRL_INT_MASK26     (0x04000000)
#define MCF_INTC1_IMRL_INT_MASK27     (0x08000000)
#define MCF_INTC1_IMRL_INT_MASK28     (0x10000000)
#define MCF_INTC1_IMRL_INT_MASK29     (0x20000000)
#define MCF_INTC1_IMRL_INT_MASK30     (0x40000000)
#define MCF_INTC1_IMRL_INT_MASK31     (0x80000000)

/* Bit definitions and macros for MCF_INTC1_INTFRCH */
#define MCF_INTC1_INTFRCH_INTFRC32    (0x00000001)
#define MCF_INTC1_INTFRCH_INTFRC33    (0x00000002)
#define MCF_INTC1_INTFRCH_INTFRC34    (0x00000004)
#define MCF_INTC1_INTFRCH_INTFRC35    (0x00000008)
#define MCF_INTC1_INTFRCH_INTFRC36    (0x00000010)
#define MCF_INTC1_INTFRCH_INTFRC37    (0x00000020)
#define MCF_INTC1_INTFRCH_INTFRC38    (0x00000040)
#define MCF_INTC1_INTFRCH_INTFRC39    (0x00000080)
#define MCF_INTC1_INTFRCH_INTFRC40    (0x00000100)
#define MCF_INTC1_INTFRCH_INTFRC41    (0x00000200)
#define MCF_INTC1_INTFRCH_INTFRC42    (0x00000400)
#define MCF_INTC1_INTFRCH_INTFRC43    (0x00000800)
#define MCF_INTC1_INTFRCH_INTFRC44    (0x00001000)
#define MCF_INTC1_INTFRCH_INTFRC45    (0x00002000)
#define MCF_INTC1_INTFRCH_INTFRC46    (0x00004000)
#define MCF_INTC1_INTFRCH_INTFRC47    (0x00008000)
#define MCF_INTC1_INTFRCH_INTFRC48    (0x00010000)
#define MCF_INTC1_INTFRCH_INTFRC49    (0x00020000)
#define MCF_INTC1_INTFRCH_INTFRC50    (0x00040000)
#define MCF_INTC1_INTFRCH_INTFRC51    (0x00080000)
#define MCF_INTC1_INTFRCH_INTFRC52    (0x00100000)
#define MCF_INTC1_INTFRCH_INTFRC53    (0x00200000)
#define MCF_INTC1_INTFRCH_INTFRC54    (0x00400000)
#define MCF_INTC1_INTFRCH_INTFRC55    (0x00800000)
#define MCF_INTC1_INTFRCH_INTFRC56    (0x01000000)
#define MCF_INTC1_INTFRCH_INTFRC57    (0x02000000)
#define MCF_INTC1_INTFRCH_INTFRC58    (0x04000000)
#define MCF_INTC1_INTFRCH_INTFRC59    (0x08000000)
#define MCF_INTC1_INTFRCH_INTFRC60    (0x10000000)
#define MCF_INTC1_INTFRCH_INTFRC61    (0x20000000)
#define MCF_INTC1_INTFRCH_INTFRC62    (0x40000000)
#define MCF_INTC1_INTFRCH_INTFRC63    (0x80000000)

/* Bit definitions and macros for MCF_INTC1_INTFRCL */
#define MCF_INTC1_INTFRCL_INTFRC1     (0x00000002)
#define MCF_INTC1_INTFRCL_INTFRC2     (0x00000004)
#define MCF_INTC1_INTFRCL_INTFRC3     (0x00000008)
#define MCF_INTC1_INTFRCL_INTFRC4     (0x00000010)
#define MCF_INTC1_INTFRCL_INTFRC5     (0x00000020)
#define MCF_INTC1_INTFRCL_INT6        (0x00000040)
#define MCF_INTC1_INTFRCL_INT7        (0x00000080)
#define MCF_INTC1_INTFRCL_INT8        (0x00000100)
#define MCF_INTC1_INTFRCL_INT9        (0x00000200)
#define MCF_INTC1_INTFRCL_INT10       (0x00000400)
#define MCF_INTC1_INTFRCL_INTFRC11    (0x00000800)
#define MCF_INTC1_INTFRCL_INTFRC12    (0x00001000)
#define MCF_INTC1_INTFRCL_INTFRC13    (0x00002000)
#define MCF_INTC1_INTFRCL_INTFRC14    (0x00004000)
#define MCF_INTC1_INTFRCL_INT15       (0x00008000)
#define MCF_INTC1_INTFRCL_INTFRC16    (0x00010000)
#define MCF_INTC1_INTFRCL_INTFRC17    (0x00020000)
#define MCF_INTC1_INTFRCL_INTFRC18    (0x00040000)
#define MCF_INTC1_INTFRCL_INTFRC19    (0x00080000)
#define MCF_INTC1_INTFRCL_INTFRC20    (0x00100000)
#define MCF_INTC1_INTFRCL_INTFRC21    (0x00200000)
#define MCF_INTC1_INTFRCL_INTFRC22    (0x00400000)
#define MCF_INTC1_INTFRCL_INTFRC23    (0x00800000)
#define MCF_INTC1_INTFRCL_INTFRC24    (0x01000000)
#define MCF_INTC1_INTFRCL_INTFRC25    (0x02000000)
#define MCF_INTC1_INTFRCL_INTFRC26    (0x04000000)
#define MCF_INTC1_INTFRCL_INTFRC27    (0x08000000)
#define MCF_INTC1_INTFRCL_INTFRC28    (0x10000000)
#define MCF_INTC1_INTFRCL_INTFRC29    (0x20000000)
#define MCF_INTC1_INTFRCL_INTFRC30    (0x40000000)
#define MCF_INTC1_INTFRCL_INTFRC31    (0x80000000)

/* Bit definitions and macros for MCF_INTC1_IRLR */
#define MCF_INTC1_IRLR_IRQ(x)         (((x)&0x7F)<<1)

/* Bit definitions and macros for MCF_INTC1_IACKLPR */
#define MCF_INTC1_IACKLPR_PRI(x)      (((x)&0x0F)<<0)
#define MCF_INTC1_IACKLPR_LEVEL(x)    (((x)&0x07)<<4)

/* Bit definitions and macros for MCF_INTC1_ICRn */
#define MCF_INTC1_ICRn_IP(x)          (((x)&0x07)<<0)
#define MCF_INTC1_ICRn_IL(x)          (((x)&0x07)<<3)

/********************************************************************/

#endif /* __MCF523X_INTC1_H__ */
