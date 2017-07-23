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
* File Name    : r_cg_sci_user.c
* Version      : Code Generator for RX23T V1.00.04.02 [29 Nov 2016]
* Device(s)    : R5F523T5AxFM
* Tool-Chain   : CCRX
* Description  : This file implements device driver for SCI module.
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
#include "r_cg_sci.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
extern uint8_t * gp_sci1_rx_address;                /* SCI1 receive buffer address */
extern uint16_t  g_sci1_rx_count;                   /* SCI1 receive data number */
extern uint16_t  g_sci1_rx_length;                  /* SCI1 receive data length */
/* Start user code for global. Do not edit comment generated here */
extern void u_sci1_receiveend_callback(void);
/* End user code. Do not edit comment generated here */


/***********************************************************************************************************************
* Function Name: r_sci1_receive_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#if FAST_INTERRUPT_VECTOR == VECT_SCI1_RXI1
#pragma interrupt r_sci1_receive_interrupt(vect=VECT(SCI1,RXI1),fint)
#else
#pragma interrupt r_sci1_receive_interrupt(vect=VECT(SCI1,RXI1))
#endif
static void r_sci1_receive_interrupt(void)
{
    if (g_sci1_rx_length > g_sci1_rx_count)
    {
        *gp_sci1_rx_address = SCI1.RDR;
        gp_sci1_rx_address++;
        g_sci1_rx_count++;

        if (g_sci1_rx_length <= g_sci1_rx_count)
        {
            r_sci1_callback_receiveend();
        }
    }
}
/***********************************************************************************************************************
* Function Name: r_sci1_callback_receiveend
* Description  : This function is a callback function when SCI1 finishes reception.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_sci1_callback_receiveend(void)
{
    /* Start user code. Do not edit comment generated here */
    u_sci1_receiveend_callback();
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
