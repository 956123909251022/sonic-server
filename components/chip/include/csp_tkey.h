/***********************************************************************//** 
 * \file  tkey.c
 * \brief TKEY description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2022 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-1-4 <td>V0.0  <td>ASM AE Team   <td>initial
 * </table>
 * *********************************************************************
*/  

#ifndef _CSP_TKEY_H
#define _CSP_TKEY_H

#include <stdint.h>
#include <sys_clk.h>


#define TKEY_RESET_VALUE  	(0x00000000)


/**
@brief TKEY  bits Structure
*/
typedef volatile struct
{
   __IOM uint32_t   TCH_IDR;               /* Control Register */
   __IOM uint32_t   TCH_CCR;               /* Control Register */
   __IOM uint32_t   TCH_CON0;               /* Control Register */
   __IOM uint32_t   TCH_CON1;               /* Control Register */
   __IOM uint32_t   TCH_RSSR;               /* Hardmacro control    */
   __IOM uint32_t   TCH_THR;            /* Sensing target value */
   __IOM uint32_t   TCH_SCVAL;            /* Sensing counter value*/
   __IOM uint32_t   TCH_TKST;            /* Reference counter value*/
   __IOM uint32_t   TCH_CHINF;                /* Match Status           */
   __IOM uint32_t   TCH_RISR;				
   __IOM uint32_t   TCH_MISR;               /* Interrupt Enable       */
   __IOM uint32_t   TCH_IMCR;               /* Interrupt Clear        */
   __IOM uint32_t   TCH_ICR;            /* Sensing target value */
   __IOM uint32_t   TCH_EVTRG;            /* Sensing target value */
   __IOM uint32_t   TCH_EVPS;            /* Sensing target value */
   __IOM uint32_t   TCH_EVSWF;            /* Sensing target value */
   __IOM uint32_t   Reserved0[1008];
   __IOM uint32_t   TCH_DAT[32];          /* Reference value capture value */
   __IOM uint32_t   Reserved1[992];
   __IOM uint32_t   TCH_SEQxCON[32];         /* SEQ Hardmacro control  */
}csp_tkey_t; 


#endif		