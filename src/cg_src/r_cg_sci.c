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
* File Name    : r_cg_sci.c
* Version      : Code Generator for RX23T V1.00.04.02 [29 Nov 2016]
* Device(s)    : R5F523T5AxFM
* Tool-Chain   : CCRX
* Description  : This file implements device driver for SCI module.
* Creation Date: 17.8.1
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
uint8_t * gp_sci1_rx_address;               /* SCI1 receive buffer address */
uint16_t  g_sci1_rx_count;                  /* SCI1 receive data number */
uint16_t  g_sci1_rx_length;                 /* SCI1 receive data length */
uint8_t * gp_sci5_tx_address;               /* SCI5 transmit buffer address */
uint16_t  g_sci5_tx_count;                  /* SCI5 transmit data number */
uint8_t * gp_sci5_rx_address;               /* SCI5 receive buffer address */
uint16_t  g_sci5_rx_count;                  /* SCI5 receive data number */
uint16_t  g_sci5_rx_length;                 /* SCI5 receive data length */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_SCI1_Create
* Description  : This function initializes SCI1.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_SCI1_Create(void)
{
    /* Cancel SCI1 module stop state */
    MSTP(SCI1) = 0U;

    /* Set interrupt priority */
    IPR(SCI1, ERI1) = _04_SCI_PRIORITY_LEVEL4;

    /* Clear the control register */
    SCI1.SCR.BYTE = 0x00U;

    /* Set clock enable */
    SCI1.SCR.BYTE = _00_SCI_INTERNAL_SCK_UNUSED;

    /* Clear the SIMR1.IICM, SPMR.CKPH, and CKPOL bit, and set SPMR */
    SCI1.SIMR1.BIT.IICM = 0U;
    SCI1.SPMR.BYTE = _00_SCI_RTS | _00_SCI_CLOCK_NOT_INVERTED | _00_SCI_CLOCK_NOT_DELAYED;

    /* Set control registers */
    SCI1.SMR.BYTE = _00_SCI_CLOCK_PCLK | _00_SCI_PARITY_DISABLE | _00_SCI_DATA_LENGTH_8 | 
                    _00_SCI_MULTI_PROCESSOR_DISABLE | _00_SCI_ASYNCHRONOUS_MODE;
    SCI1.SCMR.BYTE = _00_SCI_SERIAL_MODE | _00_SCI_DATA_INVERT_NONE | _00_SCI_DATA_LSB_FIRST | 
                     _10_SCI_DATA_LENGTH_8_OR_7 | _62_SCI_SCMR_DEFAULT;
    SCI1.SEMR.BYTE = _00_SCI_LOW_LEVEL_START_BIT | _00_SCI_NOISE_FILTER_DISABLE | _10_SCI_8_BASE_CLOCK | 
                     _40_SCI_BAUDRATE_DOUBLE | _04_SCI_BIT_MODULATION_ENABLE;

    /* Set bitrate */
    SCI1.BRR = 0x00U;
    SCI1.MDDR = 0xCDU;

    /* Set RXD1 pin */
    MPC.PD5PFS.BYTE = 0x0AU;
    PORTD.PMR.BYTE |= 0x20U;
}
/***********************************************************************************************************************
* Function Name: R_SCI1_Start
* Description  : This function starts SCI1.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_SCI1_Start(void)
{
    /* Clear interrupt flag */
    IR(SCI1, RXI1) = 0U;

    /* Enable SCI interrupt */
    IEN(SCI1, RXI1) = 1U;
    IEN(SCI1, ERI1) = 1U;
}
/***********************************************************************************************************************
* Function Name: R_SCI1_Stop
* Description  : This function stops SCI1.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_SCI1_Stop(void)
{
    SCI1.SCR.BIT.RE = 0U;      /* Disable serial receive */

    /* Disable SCI interrupt */
    SCI1.SCR.BIT.RIE = 0U;     /* Disable RXI and ERI interrupt */
    IEN(SCI1, RXI1) = 0U;
    IEN(SCI1, ERI1) = 0U;
    IR(SCI1, RXI1) = 0U;
}
/***********************************************************************************************************************
* Function Name: R_SCI1_Serial_Receive
* Description  : This function receives SCI1 data.
* Arguments    : rx_buf -
*                    receive buffer pointer (Not used when receive data handled by DTC or DMAC)
*                rx_num -
*                    buffer size (Not used when receive data handled by DTC or DMAC)
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/
MD_STATUS R_SCI1_Serial_Receive(uint8_t * const rx_buf, uint16_t rx_num)
{
    MD_STATUS status = MD_OK;

    if (1U > rx_num)
    {
        status = MD_ARGERROR;
    }
    else
    {
        g_sci1_rx_count = 0U;
        g_sci1_rx_length = rx_num;
        gp_sci1_rx_address = rx_buf;
        SCI1.SCR.BIT.RIE = 1U;
        SCI1.SCR.BIT.RE = 1U;
    }

    return (status);
}
/***********************************************************************************************************************
* Function Name: R_SCI5_Create
* Description  : This function initializes SCI5.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_SCI5_Create(void)
{
    /* Cancel SCI5 module stop state */
    MSTP(SCI5) = 0U;

    /* Set interrupt priority */
    IPR(SCI5, ERI5) = _04_SCI_PRIORITY_LEVEL4;

    /* Clear the control register */
    SCI5.SCR.BYTE = 0x00U;

    /* Set clock enable */
    SCI5.SCR.BYTE = _00_SCI_INTERNAL_SCK_UNUSED;

    /* Clear the SIMR1.IICM, SPMR.CKPH, and CKPOL bit, and set SPMR */
    SCI5.SIMR1.BIT.IICM = 0U;
    SCI5.SPMR.BYTE = _00_SCI_RTS | _00_SCI_CLOCK_NOT_INVERTED | _00_SCI_CLOCK_NOT_DELAYED;

    /* Set control registers */
    SCI5.SMR.BYTE = _00_SCI_CLOCK_PCLK | _00_SCI_STOP_1 | _00_SCI_PARITY_DISABLE | _00_SCI_DATA_LENGTH_8 | 
                    _00_SCI_MULTI_PROCESSOR_DISABLE | _00_SCI_ASYNCHRONOUS_MODE;
    SCI5.SCMR.BYTE = _00_SCI_SERIAL_MODE | _00_SCI_DATA_INVERT_NONE | _00_SCI_DATA_LSB_FIRST | 
                     _10_SCI_DATA_LENGTH_8_OR_7 | _62_SCI_SCMR_DEFAULT;
    SCI5.SEMR.BYTE = _00_SCI_LOW_LEVEL_START_BIT | _00_SCI_NOISE_FILTER_DISABLE | _10_SCI_8_BASE_CLOCK | 
                     _40_SCI_BAUDRATE_DOUBLE | _04_SCI_BIT_MODULATION_ENABLE;

    /* Set bitrate */
    SCI5.BRR = 0x04U;
    SCI5.MDDR = 0xECU;

    /* Set RXD5 pin */
    MPC.PB6PFS.BYTE = 0x0AU;
    PORTB.PMR.BYTE |= 0x40U;

    /* Set TXD5 pin */
    MPC.PB5PFS.BYTE = 0x0AU;
    PORTB.PODR.BYTE |= 0x20U;
    PORTB.PDR.BYTE |= 0x20U;
    PORTB.PMR.BYTE |= 0x20U;
}
/***********************************************************************************************************************
* Function Name: R_SCI5_Start
* Description  : This function starts SCI5.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_SCI5_Start(void)
{
    /* Clear interrupt flag */
    IR(SCI5, TXI5) = 0U;
    IR(SCI5, RXI5) = 0U;

    /* Enable SCI interrupt */
    IEN(SCI5, TXI5) = 1U;
    IEN(SCI5, TEI5) = 1U;
    IEN(SCI5, RXI5) = 1U;
    IEN(SCI5, ERI5) = 1U;
}
/***********************************************************************************************************************
* Function Name: R_SCI5_Stop
* Description  : This function stops SCI5.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_SCI5_Stop(void)
{
    /* Set TXD5 pin */
    PORTB.PMR.BYTE &= 0xDFU;
    SCI5.SCR.BIT.TE = 0U;      /* Disable serial transmit */
    SCI5.SCR.BIT.RE = 0U;      /* Disable serial receive */

    /* Disable SCI interrupt */
    SCI5.SCR.BIT.TIE = 0U;     /* Disable TXI interrupt */
    SCI5.SCR.BIT.RIE = 0U;     /* Disable RXI and ERI interrupt */
    IEN(SCI5, TXI5) = 0U;
    IEN(SCI5, TEI5) = 0U;
    IR(SCI5, TXI5) = 0U;
    IEN(SCI5, RXI5) = 0U;
    IEN(SCI5, ERI5) = 0U;
    IR(SCI5, RXI5) = 0U;
}
/***********************************************************************************************************************
* Function Name: R_SCI5_Serial_Receive
* Description  : This function receives SCI5 data.
* Arguments    : rx_buf -
*                    receive buffer pointer (Not used when receive data handled by DTC or DMAC)
*                rx_num -
*                    buffer size (Not used when receive data handled by DTC or DMAC)
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/
MD_STATUS R_SCI5_Serial_Receive(uint8_t * const rx_buf, uint16_t rx_num)
{
    MD_STATUS status = MD_OK;

    if (1U > rx_num)
    {
        status = MD_ARGERROR;
    }
    else
    {
        g_sci5_rx_count = 0U;
        g_sci5_rx_length = rx_num;
        gp_sci5_rx_address = rx_buf;
        SCI5.SCR.BIT.RIE = 1U;
        SCI5.SCR.BIT.RE = 1U;
    }

    return (status);
}
/***********************************************************************************************************************
* Function Name: R_SCI5_Serial_Send
* Description  : This function transmits SCI5 data.
* Arguments    : tx_buf -
*                    transfer buffer pointer (Not used when transmit data handled by DTC)
*                tx_num -
*                    buffer size (Not used when transmit data handled by DTC or DMAC)
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/
MD_STATUS R_SCI5_Serial_Send(uint8_t * const tx_buf, uint16_t tx_num)
{
    MD_STATUS status = MD_OK;

    if (1U > tx_num)
    {
        status = MD_ARGERROR;
    }
    else
    {
        gp_sci5_tx_address = tx_buf;
        g_sci5_tx_count = tx_num;

        /* Set TXD5 pin */
        PORTB.PMR.BYTE |= 0x20U;
        SCI5.SCR.BIT.TIE = 1U;
        SCI5.SCR.BIT.TE = 1U;
    }

    return (status);
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
