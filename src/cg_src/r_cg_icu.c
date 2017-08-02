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
* File Name    : r_cg_icu.c
* Version      : Code Generator for RX23T V1.00.04.02 [29 Nov 2016]
* Device(s)    : R5F523T5AxFM
* Tool-Chain   : CCRX
* Description  : This file implements device driver for ICU module.
* Creation Date: 17.8.2
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
#include "r_cg_icu.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_ICU_Create
* Description  : This function initializes ICU module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_ICU_Create(void)
{
    uint32_t * vector_location;

    /* Disable IRQ0~5 interrupts */
    ICU.IER[0x08].BYTE = _00_ICU_IRQ0_DISABLE | _00_ICU_IRQ1_DISABLE | _00_ICU_IRQ2_DISABLE | _00_ICU_IRQ3_DISABLE | 
                         _00_ICU_IRQ4_DISABLE | _00_ICU_IRQ5_DISABLE;

    /* Set fast interrupt */
    ICU.FIR.WORD = _00AE_FAST_VECTOR_NUM;

    /* Find the start of the maskable interrupt vector table */
    vector_location = (uint32_t *)get_intb();

    /* Move to the location of the address of the selected interrupt */
    vector_location += ICU.FIR.BIT.FVCT;

    /* Store the address of this vector in the FINTV register */
    set_fintv((void *) * vector_location);

    /* Enable the fast interrupt feature */
    ICU.FIR.BIT.FIEN = 1U;

    /* Set IRQ settings */
    ICU.IRQCR[0].BYTE = _08_ICU_IRQ_EDGE_RISING;
    ICU.IRQCR[1].BYTE = _08_ICU_IRQ_EDGE_RISING;
    ICU.IRQCR[2].BYTE = _08_ICU_IRQ_EDGE_RISING;

    /* Set IRQ0 priority level */
    IPR(ICU,IRQ0) = _05_ICU_PRIORITY_LEVEL5;

    /* Set IRQ1 priority level */
    IPR(ICU,IRQ1) = _05_ICU_PRIORITY_LEVEL5;

    /* Set IRQ2 priority level */
    IPR(ICU,IRQ2) = _05_ICU_PRIORITY_LEVEL5;

    /* Set IRQ0 pin */
    MPC.P10PFS.BYTE = 0x40U;
    PORT1.PDR.BYTE &= 0xFEU;
    PORT1.PMR.BYTE &= 0xFEU;

    /* Set IRQ1 pin */
    MPC.P11PFS.BYTE = 0x40U;
    PORT1.PDR.BYTE &= 0xFDU;
    PORT1.PMR.BYTE &= 0xFDU;

    /* Set IRQ2 pin */
    MPC.PB1PFS.BYTE = 0x40U;
    PORTB.PDR.BYTE &= 0xFDU;
    PORTB.PMR.BYTE &= 0xFDU;
}
/***********************************************************************************************************************
* Function Name: R_ICU_IRQ0_Start
* Description  : This function enables IRQ0 interrupt.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_ICU_IRQ0_Start(void)
{
    /* Enable IRQ0 interrupt */
    IEN(ICU,IRQ0) = 1U; 
}
/***********************************************************************************************************************
* Function Name: R_ICU_IRQ0_Stop
* Description  : This function disables IRQ0 interrupt.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_ICU_IRQ0_Stop(void)
{
    /* Disable IRQ0 interrupt */
    IEN(ICU,IRQ0) = 0U; 
}
/***********************************************************************************************************************
* Function Name: R_ICU_IRQ1_Start
* Description  : This function enables IRQ1 interrupt.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_ICU_IRQ1_Start(void)
{
    /* Enable IRQ1 interrupt */
    IEN(ICU,IRQ1) = 1U; 
}
/***********************************************************************************************************************
* Function Name: R_ICU_IRQ1_Stop
* Description  : This function disables IRQ1 interrupt.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_ICU_IRQ1_Stop(void)
{
    /* Disable IRQ1 interrupt */
    IEN(ICU,IRQ1) = 0U; 
}
/***********************************************************************************************************************
* Function Name: R_ICU_IRQ2_Start
* Description  : This function enables IRQ2 interrupt.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_ICU_IRQ2_Start(void)
{
    /* Enable IRQ2 interrupt */
    IEN(ICU,IRQ2) = 1U; 
}
/***********************************************************************************************************************
* Function Name: R_ICU_IRQ2_Stop
* Description  : This function disables IRQ2 interrupt.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_ICU_IRQ2_Stop(void)
{
    /* Disable IRQ2 interrupt */
    IEN(ICU,IRQ2) = 0U; 
}

/* Start user code for adding. Do not edit comment generated here */
unsigned char U_IRQ0_Pin_Read(void)
{
    return PORT1.PIDR.BIT.B0;
}

unsigned char U_IRQ1_Pin_Read(void)
{
    return PORT1.PIDR.BIT.B1;
}

unsigned char U_IRQ2_Pin_Read(void)
{
    return PORTB.PIDR.BIT.B1;
}
/* End user code. Do not edit comment generated here */
