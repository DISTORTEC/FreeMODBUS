/*
 * File:	mcf523x_etpu.h
 * Purpose:	Register and bit definitions for the MCF523X
 *
 * Notes:
 *
 */

#ifndef __MCF523X_ETPU_H__
#define __MCF523X_ETPU_H__

/*********************************************************************
*
* enhanced Time Processor Unit (ETPU)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_ETPU_EMCR          (*(vuint32*)(void*)(&__IPSBAR[0x1D0000]))
#define MCF_ETPU_ECDCR         (*(vuint32*)(void*)(&__IPSBAR[0x1D0004]))
#define MCF_ETPU_EMISCCR       (*(vuint32*)(void*)(&__IPSBAR[0x1D000C]))
#define MCF_ETPU_ESCMODR       (*(vuint32*)(void*)(&__IPSBAR[0x1D0010]))
#define MCF_ETPU_EECR          (*(vuint32*)(void*)(&__IPSBAR[0x1D0014]))
#define MCF_ETPU_ETBCR         (*(vuint32*)(void*)(&__IPSBAR[0x1D0020]))
#define MCF_ETPU_ETB1R         (*(vuint32*)(void*)(&__IPSBAR[0x1D0024]))
#define MCF_ETPU_ETB2R         (*(vuint32*)(void*)(&__IPSBAR[0x1D0028]))
#define MCF_ETPU_EREDCR        (*(vuint32*)(void*)(&__IPSBAR[0x1D002C]))
#define MCF_ETPU_ECISR         (*(vuint32*)(void*)(&__IPSBAR[0x1D0200]))
#define MCF_ETPU_ECDTRSR       (*(vuint32*)(void*)(&__IPSBAR[0x1D0210]))
#define MCF_ETPU_ECIOSR        (*(vuint32*)(void*)(&__IPSBAR[0x1D0220]))
#define MCF_ETPU_ECDTROSR      (*(vuint32*)(void*)(&__IPSBAR[0x1D0230]))
#define MCF_ETPU_ECIER         (*(vuint32*)(void*)(&__IPSBAR[0x1D0240]))
#define MCF_ETPU_ECDTRER       (*(vuint32*)(void*)(&__IPSBAR[0x1D0250]))
#define MCF_ETPU_ECPSSR        (*(vuint32*)(void*)(&__IPSBAR[0x1D0280]))
#define MCF_ETPU_ECSSR         (*(vuint32*)(void*)(&__IPSBAR[0x1D0290]))
#define MCF_ETPU_EC0SCR        (*(vuint32*)(void*)(&__IPSBAR[0x1D0404]))
#define MCF_ETPU_EC1SCR        (*(vuint32*)(void*)(&__IPSBAR[0x1D0414]))
#define MCF_ETPU_EC2SCR        (*(vuint32*)(void*)(&__IPSBAR[0x1D0424]))
#define MCF_ETPU_EC3SCR        (*(vuint32*)(void*)(&__IPSBAR[0x1D0434]))
#define MCF_ETPU_EC4SCR        (*(vuint32*)(void*)(&__IPSBAR[0x1D0444]))
#define MCF_ETPU_EC5SCR        (*(vuint32*)(void*)(&__IPSBAR[0x1D0454]))
#define MCF_ETPU_EC6SCR        (*(vuint32*)(void*)(&__IPSBAR[0x1D0464]))
#define MCF_ETPU_EC7SCR        (*(vuint32*)(void*)(&__IPSBAR[0x1D0474]))
#define MCF_ETPU_EC8SCR        (*(vuint32*)(void*)(&__IPSBAR[0x1D0484]))
#define MCF_ETPU_EC9SCR        (*(vuint32*)(void*)(&__IPSBAR[0x1D0494]))
#define MCF_ETPU_EC10SCR       (*(vuint32*)(void*)(&__IPSBAR[0x1D04A4]))
#define MCF_ETPU_EC11SCR       (*(vuint32*)(void*)(&__IPSBAR[0x1D04B4]))
#define MCF_ETPU_EC12SCR       (*(vuint32*)(void*)(&__IPSBAR[0x1D04C4]))
#define MCF_ETPU_EC13SCR       (*(vuint32*)(void*)(&__IPSBAR[0x1D04D4]))
#define MCF_ETPU_EC14SCR       (*(vuint32*)(void*)(&__IPSBAR[0x1D04E4]))
#define MCF_ETPU_EC15SCR       (*(vuint32*)(void*)(&__IPSBAR[0x1D04F4]))
#define MCF_ETPU_EC16SCR       (*(vuint32*)(void*)(&__IPSBAR[0x1D0504]))
#define MCF_ETPU_EC17SCR       (*(vuint32*)(void*)(&__IPSBAR[0x1D0514]))
#define MCF_ETPU_EC18SCR       (*(vuint32*)(void*)(&__IPSBAR[0x1D0524]))
#define MCF_ETPU_EC19SCR       (*(vuint32*)(void*)(&__IPSBAR[0x1D0534]))
#define MCF_ETPU_EC20SCR       (*(vuint32*)(void*)(&__IPSBAR[0x1D0544]))
#define MCF_ETPU_EC21SCR       (*(vuint32*)(void*)(&__IPSBAR[0x1D0554]))
#define MCF_ETPU_EC22SCR       (*(vuint32*)(void*)(&__IPSBAR[0x1D0564]))
#define MCF_ETPU_EC23SCR       (*(vuint32*)(void*)(&__IPSBAR[0x1D0574]))
#define MCF_ETPU_EC24SCR       (*(vuint32*)(void*)(&__IPSBAR[0x1D0584]))
#define MCF_ETPU_EC25SCR       (*(vuint32*)(void*)(&__IPSBAR[0x1D0594]))
#define MCF_ETPU_EC26SCR       (*(vuint32*)(void*)(&__IPSBAR[0x1D05A4]))
#define MCF_ETPU_EC27SCR       (*(vuint32*)(void*)(&__IPSBAR[0x1D05B4]))
#define MCF_ETPU_EC28SCR       (*(vuint32*)(void*)(&__IPSBAR[0x1D05C4]))
#define MCF_ETPU_EC29SCR       (*(vuint32*)(void*)(&__IPSBAR[0x1D05D4]))
#define MCF_ETPU_EC30SCR       (*(vuint32*)(void*)(&__IPSBAR[0x1D05E4]))
#define MCF_ETPU_EC31SCR       (*(vuint32*)(void*)(&__IPSBAR[0x1D05F4]))
#define MCF_ETPU_ECnSCR(x)     (*(vuint32*)(void*)(&__IPSBAR[0x1D0404+((x)*0x010)]))
#define MCF_ETPU_EC0CR         (*(vuint32*)(void*)(&__IPSBAR[0x1D0400]))
#define MCF_ETPU_EC1CR         (*(vuint32*)(void*)(&__IPSBAR[0x1D0410]))
#define MCF_ETPU_EC2CR         (*(vuint32*)(void*)(&__IPSBAR[0x1D0420]))
#define MCF_ETPU_EC3CR         (*(vuint32*)(void*)(&__IPSBAR[0x1D0430]))
#define MCF_ETPU_EC4CR         (*(vuint32*)(void*)(&__IPSBAR[0x1D0440]))
#define MCF_ETPU_EC5CR         (*(vuint32*)(void*)(&__IPSBAR[0x1D0450]))
#define MCF_ETPU_EC6CR         (*(vuint32*)(void*)(&__IPSBAR[0x1D0460]))
#define MCF_ETPU_EC7CR         (*(vuint32*)(void*)(&__IPSBAR[0x1D0470]))
#define MCF_ETPU_EC8CR         (*(vuint32*)(void*)(&__IPSBAR[0x1D0480]))
#define MCF_ETPU_EC9CR         (*(vuint32*)(void*)(&__IPSBAR[0x1D0490]))
#define MCF_ETPU_EC10CR        (*(vuint32*)(void*)(&__IPSBAR[0x1D04A0]))
#define MCF_ETPU_EC11CR        (*(vuint32*)(void*)(&__IPSBAR[0x1D04B0]))
#define MCF_ETPU_EC12CR        (*(vuint32*)(void*)(&__IPSBAR[0x1D04C0]))
#define MCF_ETPU_EC13CR        (*(vuint32*)(void*)(&__IPSBAR[0x1D04D0]))
#define MCF_ETPU_EC14CR        (*(vuint32*)(void*)(&__IPSBAR[0x1D04E0]))
#define MCF_ETPU_EC15CR        (*(vuint32*)(void*)(&__IPSBAR[0x1D04F0]))
#define MCF_ETPU_EC16CR        (*(vuint32*)(void*)(&__IPSBAR[0x1D0500]))
#define MCF_ETPU_EC17CR        (*(vuint32*)(void*)(&__IPSBAR[0x1D0510]))
#define MCF_ETPU_EC18CR        (*(vuint32*)(void*)(&__IPSBAR[0x1D0520]))
#define MCF_ETPU_EC19CR        (*(vuint32*)(void*)(&__IPSBAR[0x1D0530]))
#define MCF_ETPU_EC20CR        (*(vuint32*)(void*)(&__IPSBAR[0x1D0540]))
#define MCF_ETPU_EC21CR        (*(vuint32*)(void*)(&__IPSBAR[0x1D0550]))
#define MCF_ETPU_EC22CR        (*(vuint32*)(void*)(&__IPSBAR[0x1D0560]))
#define MCF_ETPU_EC23CR        (*(vuint32*)(void*)(&__IPSBAR[0x1D0570]))
#define MCF_ETPU_EC24CR        (*(vuint32*)(void*)(&__IPSBAR[0x1D0580]))
#define MCF_ETPU_EC25CR        (*(vuint32*)(void*)(&__IPSBAR[0x1D0590]))
#define MCF_ETPU_EC26CR        (*(vuint32*)(void*)(&__IPSBAR[0x1D05A0]))
#define MCF_ETPU_EC27CR        (*(vuint32*)(void*)(&__IPSBAR[0x1D05B0]))
#define MCF_ETPU_EC28CR        (*(vuint32*)(void*)(&__IPSBAR[0x1D05C0]))
#define MCF_ETPU_EC29CR        (*(vuint32*)(void*)(&__IPSBAR[0x1D05D0]))
#define MCF_ETPU_EC30CR        (*(vuint32*)(void*)(&__IPSBAR[0x1D05E0]))
#define MCF_ETPU_EC31CR        (*(vuint32*)(void*)(&__IPSBAR[0x1D05F0]))
#define MCF_ETPU_ECnCR(x)      (*(vuint32*)(void*)(&__IPSBAR[0x1D0400+((x)*0x010)]))
#define MCF_ETPU_EC0HSSR       (*(vuint32*)(void*)(&__IPSBAR[0x1D0408]))
#define MCF_ETPU_EC1HSSR       (*(vuint32*)(void*)(&__IPSBAR[0x1D0418]))
#define MCF_ETPU_EC2HSSR       (*(vuint32*)(void*)(&__IPSBAR[0x1D0428]))
#define MCF_ETPU_EC3HSSR       (*(vuint32*)(void*)(&__IPSBAR[0x1D0438]))
#define MCF_ETPU_EC4HSSR       (*(vuint32*)(void*)(&__IPSBAR[0x1D0448]))
#define MCF_ETPU_EC5HSSR       (*(vuint32*)(void*)(&__IPSBAR[0x1D0458]))
#define MCF_ETPU_EC6HSSR       (*(vuint32*)(void*)(&__IPSBAR[0x1D0468]))
#define MCF_ETPU_EC7HSSR       (*(vuint32*)(void*)(&__IPSBAR[0x1D0478]))
#define MCF_ETPU_EC8HSSR       (*(vuint32*)(void*)(&__IPSBAR[0x1D0488]))
#define MCF_ETPU_EC9HSSR       (*(vuint32*)(void*)(&__IPSBAR[0x1D0498]))
#define MCF_ETPU_EC10HSSR      (*(vuint32*)(void*)(&__IPSBAR[0x1D04A8]))
#define MCF_ETPU_EC11HSSR      (*(vuint32*)(void*)(&__IPSBAR[0x1D04B8]))
#define MCF_ETPU_EC12HSSR      (*(vuint32*)(void*)(&__IPSBAR[0x1D04C8]))
#define MCF_ETPU_EC13HSSR      (*(vuint32*)(void*)(&__IPSBAR[0x1D04D8]))
#define MCF_ETPU_EC14HSSR      (*(vuint32*)(void*)(&__IPSBAR[0x1D04E8]))
#define MCF_ETPU_EC15HSSR      (*(vuint32*)(void*)(&__IPSBAR[0x1D04F8]))
#define MCF_ETPU_EC16HSSR      (*(vuint32*)(void*)(&__IPSBAR[0x1D0508]))
#define MCF_ETPU_EC17HSSR      (*(vuint32*)(void*)(&__IPSBAR[0x1D0518]))
#define MCF_ETPU_EC18HSSR      (*(vuint32*)(void*)(&__IPSBAR[0x1D0528]))
#define MCF_ETPU_EC19HSSR      (*(vuint32*)(void*)(&__IPSBAR[0x1D0538]))
#define MCF_ETPU_EC20HSSR      (*(vuint32*)(void*)(&__IPSBAR[0x1D0548]))
#define MCF_ETPU_EC21HSSR      (*(vuint32*)(void*)(&__IPSBAR[0x1D0558]))
#define MCF_ETPU_EC22HSSR      (*(vuint32*)(void*)(&__IPSBAR[0x1D0568]))
#define MCF_ETPU_EC23HSSR      (*(vuint32*)(void*)(&__IPSBAR[0x1D0578]))
#define MCF_ETPU_EC24HSSR      (*(vuint32*)(void*)(&__IPSBAR[0x1D0588]))
#define MCF_ETPU_EC25HSSR      (*(vuint32*)(void*)(&__IPSBAR[0x1D0598]))
#define MCF_ETPU_EC26HSSR      (*(vuint32*)(void*)(&__IPSBAR[0x1D05A8]))
#define MCF_ETPU_EC27HSSR      (*(vuint32*)(void*)(&__IPSBAR[0x1D05B8]))
#define MCF_ETPU_EC28HSSR      (*(vuint32*)(void*)(&__IPSBAR[0x1D05C8]))
#define MCF_ETPU_EC29HSSR      (*(vuint32*)(void*)(&__IPSBAR[0x1D05D8]))
#define MCF_ETPU_EC30HSSR      (*(vuint32*)(void*)(&__IPSBAR[0x1D05E8]))
#define MCF_ETPU_EC31HSSR      (*(vuint32*)(void*)(&__IPSBAR[0x1D05F8]))
#define MCF_ETPU_ECnHSSR(x)    (*(vuint32*)(void*)(&__IPSBAR[0x1D0408+((x)*0x010)]))

/* Bit definitions and macros for MCF_ETPU_EMCR */
#define MCF_ETPU_EMCR_GTBE               (0x00000001)
#define MCF_ETPU_EMCR_VIS                (0x00000040)
#define MCF_ETPU_EMCR_SCMMISEN           (0x00000200)
#define MCF_ETPU_EMCR_SCMMISF            (0x00000400)
#define MCF_ETPU_EMCR_SCMSIZE(x)         (((x)&0x0000001F)<<16)
#define MCF_ETPU_EMCR_ILF2               (0x01000000)
#define MCF_ETPU_EMCR_ILF1               (0x02000000)
#define MCF_ETPU_EMCR_MGE2               (0x04000000)
#define MCF_ETPU_EMCR_MGE1               (0x08000000)
#define MCF_ETPU_EMCR_GEC                (0x80000000)

/* Bit definitions and macros for MCF_ETPU_ECDCR */
#define MCF_ETPU_ECDCR_PARM1(x)          (((x)&0x0000007F)<<0)
#define MCF_ETPU_ECDCR_WR                (0x00000080)
#define MCF_ETPU_ECDCR_PARM0(x)          (((x)&0x0000007F)<<8)
#define MCF_ETPU_ECDCR_PWIDTH            (0x00008000)
#define MCF_ETPU_ECDCR_PBASE(x)          (((x)&0x000003FF)<<16)
#define MCF_ETPU_ECDCR_CTBASE(x)         (((x)&0x0000001F)<<26)
#define MCF_ETPU_ECDCR_STS               (0x80000000)

/* Bit definitions and macros for MCF_ETPU_EECR */
#define MCF_ETPU_EECR_ETB(x)             (((x)&0x0000001F)<<0)
#define MCF_ETPU_EECR_CDFC(x)            (((x)&0x00000003)<<14)
#define MCF_ETPU_EECR_FPSK(x)            (((x)&0x00000007)<<16)
#define MCF_ETPU_EECR_HLTF               (0x00800000)
#define MCF_ETPU_EECR_STF                (0x10000000)
#define MCF_ETPU_EECR_MDIS               (0x40000000)
#define MCF_ETPU_EECR_FEND               (0x80000000)

/* Bit definitions and macros for MCF_ETPU_ETBCR */
#define MCF_ETPU_ETBCR_TCR1P(x)          (((x)&0x000000FF)<<0)
#define MCF_ETPU_ETBCR_TCR1CTL(x)        (((x)&0x00000003)<<14)
#define MCF_ETPU_ETBCR_TCR2P(x)          (((x)&0x0000003F)<<16)
#define MCF_ETPU_ETBCR_AM                (0x02000000)
#define MCF_ETPU_ETBCR_TCRCF(x)          (((x)&0x00000003)<<27)
#define MCF_ETPU_ETBCR_TCR2CTL(x)        (((x)&0x00000007)<<29)

/* Bit definitions and macros for MCF_ETPU_ETB1R */
#define MCF_ETPU_ETB1R_TCR1(x)           (((x)&0x00FFFFFF)<<0)

/* Bit definitions and macros for MCF_ETPU_ETB2R */
#define MCF_ETPU_ETB2R_TCR2(x)           (((x)&0x00FFFFFF)<<0)

/* Bit definitions and macros for MCF_ETPU_EREDCR */
#define MCF_ETPU_EREDCR_SRV2(x)          (((x)&0x0000000F)<<0)
#define MCF_ETPU_EREDCR_SERVER_ID2(x)    (((x)&0x0000000F)<<8)
#define MCF_ETPU_EREDCR_RSC2             (0x00004000)
#define MCF_ETPU_EREDCR_REN2             (0x00008000)
#define MCF_ETPU_EREDCR_SRV1(x)          (((x)&0x0000000F)<<16)
#define MCF_ETPU_EREDCR_SERVER_ID1(x)    (((x)&0x0000000F)<<24)
#define MCF_ETPU_EREDCR_RSC1             (0x40000000)
#define MCF_ETPU_EREDCR_REN1             (0x80000000)

/* Bit definitions and macros for MCF_ETPU_ECISR */
#define MCF_ETPU_ECISR_CIS0              (0x00000001)
#define MCF_ETPU_ECISR_CIS1              (0x00000002)
#define MCF_ETPU_ECISR_CIS2              (0x00000004)
#define MCF_ETPU_ECISR_CIS3              (0x00000008)
#define MCF_ETPU_ECISR_CIS4              (0x00000010)
#define MCF_ETPU_ECISR_CIS5              (0x00000020)
#define MCF_ETPU_ECISR_CIS6              (0x00000040)
#define MCF_ETPU_ECISR_CIS7              (0x00000080)
#define MCF_ETPU_ECISR_CIS8              (0x00000100)
#define MCF_ETPU_ECISR_CIS9              (0x00000200)
#define MCF_ETPU_ECISR_CIS10             (0x00000400)
#define MCF_ETPU_ECISR_CIS11             (0x00000800)
#define MCF_ETPU_ECISR_CIS12             (0x00001000)
#define MCF_ETPU_ECISR_CIS13             (0x00002000)
#define MCF_ETPU_ECISR_CIS14             (0x00004000)
#define MCF_ETPU_ECISR_CIS15             (0x00008000)
#define MCF_ETPU_ECISR_CIS16             (0x00010000)
#define MCF_ETPU_ECISR_CIS17             (0x00020000)
#define MCF_ETPU_ECISR_CIS18             (0x00040000)
#define MCF_ETPU_ECISR_CIS19             (0x00080000)
#define MCF_ETPU_ECISR_CIS20             (0x00100000)
#define MCF_ETPU_ECISR_CIS21             (0x00200000)
#define MCF_ETPU_ECISR_CIS22             (0x00400000)
#define MCF_ETPU_ECISR_CIS23             (0x00800000)
#define MCF_ETPU_ECISR_CIS24             (0x01000000)
#define MCF_ETPU_ECISR_CIS25             (0x02000000)
#define MCF_ETPU_ECISR_CIS26             (0x04000000)
#define MCF_ETPU_ECISR_CIS27             (0x08000000)
#define MCF_ETPU_ECISR_CIS28             (0x10000000)
#define MCF_ETPU_ECISR_CIS29             (0x20000000)
#define MCF_ETPU_ECISR_CIS30             (0x40000000)
#define MCF_ETPU_ECISR_CIS31             (0x80000000)

/* Bit definitions and macros for MCF_ETPU_ECDTRSR */
#define MCF_ETPU_ECDTRSR_DTRS0           (0x00000001)
#define MCF_ETPU_ECDTRSR_DTRS1           (0x00000002)
#define MCF_ETPU_ECDTRSR_DTRS2           (0x00000004)
#define MCF_ETPU_ECDTRSR_DTRS3           (0x00000008)
#define MCF_ETPU_ECDTRSR_DTRS4           (0x00000010)
#define MCF_ETPU_ECDTRSR_DTRS5           (0x00000020)
#define MCF_ETPU_ECDTRSR_DTRS6           (0x00000040)
#define MCF_ETPU_ECDTRSR_DTRS7           (0x00000080)
#define MCF_ETPU_ECDTRSR_DTRS8           (0x00000100)
#define MCF_ETPU_ECDTRSR_DTRS9           (0x00000200)
#define MCF_ETPU_ECDTRSR_DTRS10          (0x00000400)
#define MCF_ETPU_ECDTRSR_DTRS11          (0x00000800)
#define MCF_ETPU_ECDTRSR_DTRS12          (0x00001000)
#define MCF_ETPU_ECDTRSR_DTRS13          (0x00002000)
#define MCF_ETPU_ECDTRSR_DTRS14          (0x00004000)
#define MCF_ETPU_ECDTRSR_DTRS15          (0x00008000)
#define MCF_ETPU_ECDTRSR_DTRS16          (0x00010000)
#define MCF_ETPU_ECDTRSR_DTRS17          (0x00020000)
#define MCF_ETPU_ECDTRSR_DTRS18          (0x00040000)
#define MCF_ETPU_ECDTRSR_DTRS19          (0x00080000)
#define MCF_ETPU_ECDTRSR_DTRS20          (0x00100000)
#define MCF_ETPU_ECDTRSR_DTRS21          (0x00200000)
#define MCF_ETPU_ECDTRSR_DTRS22          (0x00400000)
#define MCF_ETPU_ECDTRSR_DTRS23          (0x00800000)
#define MCF_ETPU_ECDTRSR_DTRS24          (0x01000000)
#define MCF_ETPU_ECDTRSR_DTRS25          (0x02000000)
#define MCF_ETPU_ECDTRSR_DTRS26          (0x04000000)
#define MCF_ETPU_ECDTRSR_DTRS27          (0x08000000)
#define MCF_ETPU_ECDTRSR_DTRS28          (0x10000000)
#define MCF_ETPU_ECDTRSR_DTRS29          (0x20000000)
#define MCF_ETPU_ECDTRSR_DTRS30          (0x40000000)
#define MCF_ETPU_ECDTRSR_DTRS31          (0x80000000)

/* Bit definitions and macros for MCF_ETPU_ECIOSR */
#define MCF_ETPU_ECIOSR_CIOS0            (0x00000001)
#define MCF_ETPU_ECIOSR_CIOS1            (0x00000002)
#define MCF_ETPU_ECIOSR_CIOS2            (0x00000004)
#define MCF_ETPU_ECIOSR_CIOS3            (0x00000008)
#define MCF_ETPU_ECIOSR_CIOS4            (0x00000010)
#define MCF_ETPU_ECIOSR_CIOS5            (0x00000020)
#define MCF_ETPU_ECIOSR_CIOS6            (0x00000040)
#define MCF_ETPU_ECIOSR_CIOS7            (0x00000080)
#define MCF_ETPU_ECIOSR_CIOS8            (0x00000100)
#define MCF_ETPU_ECIOSR_CIOS9            (0x00000200)
#define MCF_ETPU_ECIOSR_CIOS10           (0x00000400)
#define MCF_ETPU_ECIOSR_CIOS11           (0x00000800)
#define MCF_ETPU_ECIOSR_CIOS12           (0x00001000)
#define MCF_ETPU_ECIOSR_CIOS13           (0x00002000)
#define MCF_ETPU_ECIOSR_CIOS14           (0x00004000)
#define MCF_ETPU_ECIOSR_CIOS15           (0x00008000)
#define MCF_ETPU_ECIOSR_CIOS16           (0x00010000)
#define MCF_ETPU_ECIOSR_CIOS17           (0x00020000)
#define MCF_ETPU_ECIOSR_CIOS18           (0x00040000)
#define MCF_ETPU_ECIOSR_CIOS19           (0x00080000)
#define MCF_ETPU_ECIOSR_CIOS20           (0x00100000)
#define MCF_ETPU_ECIOSR_CIOS21           (0x00200000)
#define MCF_ETPU_ECIOSR_CIOS22           (0x00400000)
#define MCF_ETPU_ECIOSR_CIOS23           (0x00800000)
#define MCF_ETPU_ECIOSR_CIOS24           (0x01000000)
#define MCF_ETPU_ECIOSR_CIOS25           (0x02000000)
#define MCF_ETPU_ECIOSR_CIOS26           (0x04000000)
#define MCF_ETPU_ECIOSR_CIOS27           (0x08000000)
#define MCF_ETPU_ECIOSR_CIOS28           (0x10000000)
#define MCF_ETPU_ECIOSR_CIOS29           (0x20000000)
#define MCF_ETPU_ECIOSR_CIOS30           (0x40000000)
#define MCF_ETPU_ECIOSR_CIOS31           (0x80000000)

/* Bit definitions and macros for MCF_ETPU_ECDTROSR */
#define MCF_ETPU_ECDTROSR_DTROS0         (0x00000001)
#define MCF_ETPU_ECDTROSR_DTROS1         (0x00000002)
#define MCF_ETPU_ECDTROSR_DTROS2         (0x00000004)
#define MCF_ETPU_ECDTROSR_DTROS3         (0x00000008)
#define MCF_ETPU_ECDTROSR_DTROS4         (0x00000010)
#define MCF_ETPU_ECDTROSR_DTROS5         (0x00000020)
#define MCF_ETPU_ECDTROSR_DTROS6         (0x00000040)
#define MCF_ETPU_ECDTROSR_DTROS7         (0x00000080)
#define MCF_ETPU_ECDTROSR_DTROS8         (0x00000100)
#define MCF_ETPU_ECDTROSR_DTROS9         (0x00000200)
#define MCF_ETPU_ECDTROSR_DTROS10        (0x00000400)
#define MCF_ETPU_ECDTROSR_DTROS11        (0x00000800)
#define MCF_ETPU_ECDTROSR_DTROS12        (0x00001000)
#define MCF_ETPU_ECDTROSR_DTROS13        (0x00002000)
#define MCF_ETPU_ECDTROSR_DTROS14        (0x00004000)
#define MCF_ETPU_ECDTROSR_DTROS15        (0x00008000)
#define MCF_ETPU_ECDTROSR_DTROS16        (0x00010000)
#define MCF_ETPU_ECDTROSR_DTROS17        (0x00020000)
#define MCF_ETPU_ECDTROSR_DTROS18        (0x00040000)
#define MCF_ETPU_ECDTROSR_DTROS19        (0x00080000)
#define MCF_ETPU_ECDTROSR_DTROS20        (0x00100000)
#define MCF_ETPU_ECDTROSR_DTROS21        (0x00200000)
#define MCF_ETPU_ECDTROSR_DTROS22        (0x00400000)
#define MCF_ETPU_ECDTROSR_DTROS23        (0x00800000)
#define MCF_ETPU_ECDTROSR_DTROS24        (0x01000000)
#define MCF_ETPU_ECDTROSR_DTROS25        (0x02000000)
#define MCF_ETPU_ECDTROSR_DTROS26        (0x04000000)
#define MCF_ETPU_ECDTROSR_DTROS27        (0x08000000)
#define MCF_ETPU_ECDTROSR_DTROS28        (0x10000000)
#define MCF_ETPU_ECDTROSR_DTROS29        (0x20000000)
#define MCF_ETPU_ECDTROSR_DTROS30        (0x40000000)
#define MCF_ETPU_ECDTROSR_DTROS31        (0x80000000)

/* Bit definitions and macros for MCF_ETPU_ECIER */
#define MCF_ETPU_ECIER_CIE0              (0x00000001)
#define MCF_ETPU_ECIER_CIE1              (0x00000002)
#define MCF_ETPU_ECIER_CIE2              (0x00000004)
#define MCF_ETPU_ECIER_CIE3              (0x00000008)
#define MCF_ETPU_ECIER_CIE4              (0x00000010)
#define MCF_ETPU_ECIER_CIE5              (0x00000020)
#define MCF_ETPU_ECIER_CIE6              (0x00000040)
#define MCF_ETPU_ECIER_CIE7              (0x00000080)
#define MCF_ETPU_ECIER_CIE8              (0x00000100)
#define MCF_ETPU_ECIER_CIE9              (0x00000200)
#define MCF_ETPU_ECIER_CIE10             (0x00000400)
#define MCF_ETPU_ECIER_CIE11             (0x00000800)
#define MCF_ETPU_ECIER_CIE12             (0x00001000)
#define MCF_ETPU_ECIER_CIE13             (0x00002000)
#define MCF_ETPU_ECIER_CIE14             (0x00004000)
#define MCF_ETPU_ECIER_CIE15             (0x00008000)
#define MCF_ETPU_ECIER_CIE16             (0x00010000)
#define MCF_ETPU_ECIER_CIE17             (0x00020000)
#define MCF_ETPU_ECIER_CIE18             (0x00040000)
#define MCF_ETPU_ECIER_CIE19             (0x00080000)
#define MCF_ETPU_ECIER_CIE20             (0x00100000)
#define MCF_ETPU_ECIER_CIE21             (0x00200000)
#define MCF_ETPU_ECIER_CIE22             (0x00400000)
#define MCF_ETPU_ECIER_CIE23             (0x00800000)
#define MCF_ETPU_ECIER_CIE24             (0x01000000)
#define MCF_ETPU_ECIER_CIE25             (0x02000000)
#define MCF_ETPU_ECIER_CIE26             (0x04000000)
#define MCF_ETPU_ECIER_CIE27             (0x08000000)
#define MCF_ETPU_ECIER_CIE28             (0x10000000)
#define MCF_ETPU_ECIER_CIE29             (0x20000000)
#define MCF_ETPU_ECIER_CIE30             (0x40000000)
#define MCF_ETPU_ECIER_CIE31             (0x80000000)

/* Bit definitions and macros for MCF_ETPU_ECDTRER */
#define MCF_ETPU_ECDTRER_DTRE0           (0x00000001)
#define MCF_ETPU_ECDTRER_DTRE1           (0x00000002)
#define MCF_ETPU_ECDTRER_DTRE2           (0x00000004)
#define MCF_ETPU_ECDTRER_DTRE3           (0x00000008)
#define MCF_ETPU_ECDTRER_DTRE4           (0x00000010)
#define MCF_ETPU_ECDTRER_DTRE5           (0x00000020)
#define MCF_ETPU_ECDTRER_DTRE6           (0x00000040)
#define MCF_ETPU_ECDTRER_DTRE7           (0x00000080)
#define MCF_ETPU_ECDTRER_DTRE8           (0x00000100)
#define MCF_ETPU_ECDTRER_DTRE9           (0x00000200)
#define MCF_ETPU_ECDTRER_DTRE10          (0x00000400)
#define MCF_ETPU_ECDTRER_DTRE11          (0x00000800)
#define MCF_ETPU_ECDTRER_DTRE12          (0x00001000)
#define MCF_ETPU_ECDTRER_DTRE13          (0x00002000)
#define MCF_ETPU_ECDTRER_DTRE14          (0x00004000)
#define MCF_ETPU_ECDTRER_DTRE15          (0x00008000)
#define MCF_ETPU_ECDTRER_DTRE16          (0x00010000)
#define MCF_ETPU_ECDTRER_DTRE17          (0x00020000)
#define MCF_ETPU_ECDTRER_DTRE18          (0x00040000)
#define MCF_ETPU_ECDTRER_DTRE19          (0x00080000)
#define MCF_ETPU_ECDTRER_DTRE20          (0x00100000)
#define MCF_ETPU_ECDTRER_DTRE21          (0x00200000)
#define MCF_ETPU_ECDTRER_DTRE22          (0x00400000)
#define MCF_ETPU_ECDTRER_DTRE23          (0x00800000)
#define MCF_ETPU_ECDTRER_DTRE24          (0x01000000)
#define MCF_ETPU_ECDTRER_DTRE25          (0x02000000)
#define MCF_ETPU_ECDTRER_DTRE26          (0x04000000)
#define MCF_ETPU_ECDTRER_DTRE27          (0x08000000)
#define MCF_ETPU_ECDTRER_DTRE28          (0x10000000)
#define MCF_ETPU_ECDTRER_DTRE29          (0x20000000)
#define MCF_ETPU_ECDTRER_DTRE30          (0x40000000)
#define MCF_ETPU_ECDTRER_DTRE31          (0x80000000)

/* Bit definitions and macros for MCF_ETPU_ECPSSR */
#define MCF_ETPU_ECPSSR_SR0              (0x00000001)
#define MCF_ETPU_ECPSSR_SR1              (0x00000002)
#define MCF_ETPU_ECPSSR_SR2              (0x00000004)
#define MCF_ETPU_ECPSSR_SR3              (0x00000008)
#define MCF_ETPU_ECPSSR_SR4              (0x00000010)
#define MCF_ETPU_ECPSSR_SR5              (0x00000020)
#define MCF_ETPU_ECPSSR_SR6              (0x00000040)
#define MCF_ETPU_ECPSSR_SR7              (0x00000080)
#define MCF_ETPU_ECPSSR_SR8              (0x00000100)
#define MCF_ETPU_ECPSSR_SR9              (0x00000200)
#define MCF_ETPU_ECPSSR_SR10             (0x00000400)
#define MCF_ETPU_ECPSSR_SR11             (0x00000800)
#define MCF_ETPU_ECPSSR_SR12             (0x00001000)
#define MCF_ETPU_ECPSSR_SR13             (0x00002000)
#define MCF_ETPU_ECPSSR_SR14             (0x00004000)
#define MCF_ETPU_ECPSSR_SR15             (0x00008000)
#define MCF_ETPU_ECPSSR_SR16             (0x00010000)
#define MCF_ETPU_ECPSSR_SR17             (0x00020000)
#define MCF_ETPU_ECPSSR_SR18             (0x00040000)
#define MCF_ETPU_ECPSSR_SR19             (0x00080000)
#define MCF_ETPU_ECPSSR_SR20             (0x00100000)
#define MCF_ETPU_ECPSSR_SR21             (0x00200000)
#define MCF_ETPU_ECPSSR_SR22             (0x00400000)
#define MCF_ETPU_ECPSSR_SR23             (0x00800000)
#define MCF_ETPU_ECPSSR_SR24             (0x01000000)
#define MCF_ETPU_ECPSSR_SR25             (0x02000000)
#define MCF_ETPU_ECPSSR_SR26             (0x04000000)
#define MCF_ETPU_ECPSSR_SR27             (0x08000000)
#define MCF_ETPU_ECPSSR_SR28             (0x10000000)
#define MCF_ETPU_ECPSSR_SR29             (0x20000000)
#define MCF_ETPU_ECPSSR_SR30             (0x40000000)
#define MCF_ETPU_ECPSSR_SR31             (0x80000000)

/* Bit definitions and macros for MCF_ETPU_ECSSR */
#define MCF_ETPU_ECSSR_SS0               (0x00000001)
#define MCF_ETPU_ECSSR_SS1               (0x00000002)
#define MCF_ETPU_ECSSR_SS2               (0x00000004)
#define MCF_ETPU_ECSSR_SS3               (0x00000008)
#define MCF_ETPU_ECSSR_SS4               (0x00000010)
#define MCF_ETPU_ECSSR_SS5               (0x00000020)
#define MCF_ETPU_ECSSR_SS6               (0x00000040)
#define MCF_ETPU_ECSSR_SS7               (0x00000080)
#define MCF_ETPU_ECSSR_SS8               (0x00000100)
#define MCF_ETPU_ECSSR_SS9               (0x00000200)
#define MCF_ETPU_ECSSR_SS10              (0x00000400)
#define MCF_ETPU_ECSSR_SS11              (0x00000800)
#define MCF_ETPU_ECSSR_SS12              (0x00001000)
#define MCF_ETPU_ECSSR_SS13              (0x00002000)
#define MCF_ETPU_ECSSR_SS14              (0x00004000)
#define MCF_ETPU_ECSSR_SS15              (0x00008000)
#define MCF_ETPU_ECSSR_SS16              (0x00010000)
#define MCF_ETPU_ECSSR_SS17              (0x00020000)
#define MCF_ETPU_ECSSR_SS18              (0x00040000)
#define MCF_ETPU_ECSSR_SS19              (0x00080000)
#define MCF_ETPU_ECSSR_SS20              (0x00100000)
#define MCF_ETPU_ECSSR_SS21              (0x00200000)
#define MCF_ETPU_ECSSR_SS22              (0x00400000)
#define MCF_ETPU_ECSSR_SS23              (0x00800000)
#define MCF_ETPU_ECSSR_SS24              (0x01000000)
#define MCF_ETPU_ECSSR_SS25              (0x02000000)
#define MCF_ETPU_ECSSR_SS26              (0x04000000)
#define MCF_ETPU_ECSSR_SS27              (0x08000000)
#define MCF_ETPU_ECSSR_SS28              (0x10000000)
#define MCF_ETPU_ECSSR_SS29              (0x20000000)
#define MCF_ETPU_ECSSR_SS30              (0x40000000)
#define MCF_ETPU_ECSSR_SS31              (0x80000000)

/* Bit definitions and macros for MCF_ETPU_ECnSCR */
#define MCF_ETPU_ECnSCR_FM(x)            (((x)&0x00000003)<<0)
#define MCF_ETPU_ECnSCR_OBE              (0x00002000)
#define MCF_ETPU_ECnSCR_OPS              (0x00004000)
#define MCF_ETPU_ECnSCR_IPS              (0x00008000)
#define MCF_ETPU_ECnSCR_DTROS            (0x00400000)
#define MCF_ETPU_ECnSCR_DTRS             (0x00800000)
#define MCF_ETPU_ECnSCR_CIOS             (0x40000000)
#define MCF_ETPU_ECnSCR_CIS              (0x80000000)

/* Bit definitions and macros for MCF_ETPU_ECnCR */
#define MCF_ETPU_ECnCR_CPBA(x)           (((x)&0x000007FF)<<0)
#define MCF_ETPU_ECnCR_OPOL              (0x00004000)
#define MCF_ETPU_ECnCR_ODIS              (0x00008000)
#define MCF_ETPU_ECnCR_CFS(x)            (((x)&0x0000001F)<<16)
#define MCF_ETPU_ECnCR_ETCS              (0x01000000)
#define MCF_ETPU_ECnCR_CPR(x)            (((x)&0x00000003)<<28)
#define MCF_ETPU_ECnCR_DTRE              (0x40000000)
#define MCF_ETPU_ECnCR_CIE               (0x80000000)

/* Bit definitions and macros for MCF_ETPU_ECnHSSR */
#define MCF_ETPU_ECnHSSR_HSR(x)          (((x)&0x00000007)<<0)

/********************************************************************/

#endif /* __MCF523X_ETPU_H__ */
