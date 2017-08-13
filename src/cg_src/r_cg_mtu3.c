/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2015, 2016 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_mtu3.c
* Version      : Code Generator for RX23T V1.00.04.02 [29 Nov 2016]
* Device(s)    : R5F523T5AxFM
* Tool-Chain   : CCRX
* Description  : This file implements device driver for MTU3 module.
* Creation Date: 17.8.13
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_mtu3.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_MTU3_Create
* Description  : This function initializes the MTU3 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MTU3_Create(void)
{
    /* Cancel MTU stop state in LPC */
    MSTP(MTU) = 0U;

    /* Enable read/write to MTU registers */
    MTU.TRWERA.BYTE = _01_MTU_RWE_ENABLE;

    /* Stop all channels */
    MTU.TSTRA.BYTE = _00_MTU_CST0_OFF | _00_MTU_CST1_OFF | _00_MTU_CST2_OFF | _00_MTU_CST3_OFF | _00_MTU_CST4_OFF;
    MTU5.TSTR.BYTE = _00_MTU_CSTW5_OFF | _00_MTU_CSTV5_OFF | _00_MTU_CSTU5_OFF;

    /* Set interrupt priority level */
    IPR(MTU5, TGIU5) = _0E_MTU_PRIORITY_LEVEL14;

    /* Channel 0 is used as PWM1 mode */
    MTU0.TCR.BYTE = _00_MTU_PCLK_1 | _00_MTU_CKEG_RISE | _A0_MTU_CKCL_C;
    MTU0.TCR2.BYTE = _00_MTU_PCLK_1;
    MTU.TSYRA.BIT.SYNC0 = 0U;
    MTU0.TMDR1.BYTE = _02_MTU_PWM1;
    MTU0.TIORH.BYTE = _70_MTU_IOB_HT | _03_MTU_IOA_LT;
    MTU0.TIORL.BYTE = _00_MTU_IOC_DISABLE;
    MTU0.TGRA = _0009_TGRA_VALUE;
    MTU0.TGRB = _1F49_TGRB_VALUE;
    MTU0.TGRC = _3E7F_TGRC_VALUE;
    MTU0.TGRD = _0063_TGRD_VALUE;
    MTU0.TGRE = _0063_TGRE_VALUE;
    MTU0.TGRF = _0064_TGRF_VALUE;
    MTU0.TIER.BYTE = _00_MTU_TGIEA_DISABLE | _00_MTU_TGIEB_DISABLE | _00_MTU_TGIEC_DISABLE | _00_MTU_TGIED_DISABLE | 
                     _00_MTU_TCIEU_DISABLE | _00_MTU_TCIEV_DISABLE | _00_MTU_TTGE_DISABLE;
    MTU0.TIER2.BYTE = _00_MTU_TGIEE_DISABLE | _00_MTU_TGIEF_DISABLE | _00_MTU_TTGE2_DISABLE;

    /* Channel 5 is used as dead time compensation */
    MTU5.TIER.BYTE = _04_MTU_TGIE5U_ENABLE | _00_MTU_TGIE5V_DISABLE | _00_MTU_TGIE5W_DISABLE;
    MTU5.TCRU.BYTE = _02_MTU_PCLK_16;
    MTU5.TCRV.BYTE = _00_MTU_PCLK_1;
    MTU5.TCRW.BYTE = _00_MTU_PCLK_1;
    MTU5.TCR2U.BYTE = _00_MTU_PCLK_1;
    MTU5.TCR2V.BYTE = _00_MTU_PCLK_1;
    MTU5.TCR2W.BYTE = _00_MTU_PCLK_1;
    MTU5.TCNTCMPCLR.BYTE = _04_MTU_CMPCLR5U_ENABLE | _00_MTU_CMPCLR5V_DISABLE | _00_MTU_CMPCLR5W_DISABLE;
    MTU5.NFCR5.BYTE = _01_MTU_NFUEN_ENABLE | _00_MTU_NFVEN_DISABLE | _00_MTU_NFWEN_DISABLE | _00_MTU_NFCS_PCLK_1;
    MTU5.TIORU.BYTE = _11_MTU5_IOC_R;
    MTU5.TIORV.BYTE = _00_MTU5_IOC_CP;
    MTU5.TIORW.BYTE = _00_MTU5_IOC_CP;
    MTU5.TGRV = _0F9F_TGRV_VALUE;
    MTU5.TGRW = _0F9F_TGRW_VALUE;

    /* Disable read/write to MTU registers */
    MTU.TRWERA.BYTE = _00_MTU_RWE_DISABLE;

    /* Set MTIOC0A pin */
    MPC.PB3PFS.BYTE = 0x01U;
    PORTB.PMR.BYTE |= 0x08U;
    /* Set MTIC5U pin */
    MPC.P24PFS.BYTE = 0x01U;
    PORT2.PMR.BYTE |= 0x10U;
}
/***********************************************************************************************************************
* Function Name: R_MTU3_C0_Start
* Description  : This function starts MTU3 channel 0 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MTU3_C0_Start(void)
{
    MTU.TSTRA.BYTE |= _01_MTU_CST0_ON;
}
/***********************************************************************************************************************
* Function Name: R_MTU3_C0_Stop
* Description  : This function stops MTU3 channel 0 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MTU3_C0_Stop(void)
{
    MTU.TSTRA.BIT.CST0 = 0U;
}
/***********************************************************************************************************************
* Function Name: R_MTU3_C5_Start
* Description  : This function starts MTU3 channel 5 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MTU3_C5_Start(void)
{
    /* Enable TGIU5 interrupt in ICU */
    IEN(MTU5,TGIU5) = 1U;

    MTU5.TSTR.BYTE = _01_MTU_CSTW5_ON | _02_MTU_CSTV5_ON | _04_MTU_CSTU5_ON;
}
/***********************************************************************************************************************
* Function Name: R_MTU3_C5_Stop
* Description  : This function stops MTU3 channel 5 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MTU3_C5_Stop(void)
{
    /* Disable TGIU5 interrupt in ICU */
    IEN(MTU5,TGIU5) = 0U;

    MTU5.TSTR.BYTE = _00_MTU_CSTW5_OFF | _00_MTU_CSTV5_OFF | _00_MTU_CSTU5_OFF;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
