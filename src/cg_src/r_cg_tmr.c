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
* File Name    : r_cg_tmr.c
* Version      : Code Generator for RX23T V1.00.04.02 [29 Nov 2016]
* Device(s)    : R5F523T5AxFM
* Tool-Chain   : CCRX
* Description  : This file implements device driver for TMR module.
* Creation Date: 17.7.23
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
#include "r_cg_tmr.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_TMR_Create
* Description  : This function initializes the TMR module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TMR_Create(void)
{
    /* Disable TMR2 interrupts */
    IEN(TMR2, CMIA2) = 0U;

    /* Cancel TMR module stop state */
    MSTP(TMR23) = 0U;

    /* Set timer counter control setting */
    TMR2.TCCR.BYTE = _18_TMR_CLK_TMR3_OVRF | _00_TMR_CLK_DISABLED;

    /* Set counter clear and interrupt */
    TMR2.TCR.BYTE = _40_TMR_CMIA_INT_ENABLE | _08_TMR_CNT_CLR_COMP_MATCH_A;

    /* Set output */
    TMR2.TCSR.BYTE = _00_TMR_COMP_MATCH_B_OUTPUT_RETAIN | _02_TMR_COMP_MATCH_A_OUTPUT_HIGH | _E0_TMR02_TCSR_DEFAULT;

    /* Set compare match value */
    TMR23.TCORA = _09C3_TMR23_COMP_MATCH_VALUE_A;
    TMR23.TCORB = _0031_TMR23_COMP_MATCH_VALUE_B;

    /* Configure TMR2 interrupts */
    IPR(TMR2, CMIA2) = _0F_TMR_PRIORITY_LEVEL15;

    /* Set TMO2 pin */
    MPC.P23PFS.BYTE = 0x05U;
    PORT2.PMR.BYTE |= 0x08U;
}
/***********************************************************************************************************************
* Function Name: R_TMR2_Start
* Description  : This function starts TMR2 channel.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TMR2_Start(void)
{
    /* Enable TMR2 interrupts */
    IR(TMR2, CMIA2) = 0U;
    IEN(TMR2, CMIA2) = 1U;

    /* Start counting */
    TMR3.TCCR.BYTE = _08_TMR_CLK_SRC_PCLK | _00_TMR_PCLK_DIV_1;
}
/***********************************************************************************************************************
* Function Name: R_TMR2_Stop
* Description  : This function stops TMR2 channel.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_TMR2_Stop(void)
{
    /* Disable TMR2 interrupts */
    IEN(TMR2, CMIA2) = 0U;

    /* Stop counting */
    TMR3.TCCR.BYTE = _00_TMR_CLK_DISABLED;
}

/* Start user code for adding. Do not edit comment generated here */
void U_TMR2_SetCMPA(uint16_t compare_a_value)
{
    /* Set compare match value */
    TMR23.TCORA = compare_a_value;
}

void U_TMR2_SetOUTA(bool is_high)
{
    /* Set output */
    if (is_high) {
        TMR2.TCSR.BYTE = _00_TMR_COMP_MATCH_B_OUTPUT_RETAIN | _02_TMR_COMP_MATCH_A_OUTPUT_HIGH | _E0_TMR02_TCSR_DEFAULT;
    } else {
        TMR2.TCSR.BYTE = _00_TMR_COMP_MATCH_B_OUTPUT_RETAIN | _01_TMR_COMP_MATCH_A_OUTPUT_LOW | _E0_TMR02_TCSR_DEFAULT;
    }
}
/* End user code. Do not edit comment generated here */
