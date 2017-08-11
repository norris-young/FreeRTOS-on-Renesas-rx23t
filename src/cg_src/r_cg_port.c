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
* File Name    : r_cg_port.c
* Version      : Code Generator for RX23T V1.00.04.02 [29 Nov 2016]
* Device(s)    : R5F523T5AxFM
* Tool-Chain   : CCRX
* Description  : This file implements device driver for Port module.
* Creation Date: 17.8.11
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
#include "r_cg_port.h"
/* Start user code for include. Do not edit comment generated here */
#include "cam_commu.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_PORT_Create
* Description  : This function initializes the Port I/O.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_PORT_Create(void)
{
    PORT2.PODR.BYTE = _08_Pm3_OUTPUT_1;
    PORT3.PODR.BYTE = _01_Pm0_OUTPUT_1 | _02_Pm1_OUTPUT_1 | _04_Pm2_OUTPUT_1;
    PORT4.PODR.BYTE = _02_Pm1_OUTPUT_1 | _20_Pm5_OUTPUT_1;
    PORT7.PODR.BYTE = _02_Pm1_OUTPUT_1 | _08_Pm3_OUTPUT_1 | _40_Pm6_OUTPUT_1;
    PORTB.PODR.BYTE = _80_Pm7_OUTPUT_1;
    PORTB.ODR1.BYTE = _40_Pm7_NCH_OPEN_DRAIN;
    PORT7.PCR.BYTE = _01_Pm0_PULLUP_ON | _04_Pm2_PULLUP_ON | _10_Pm4_PULLUP_ON | _20_Pm5_PULLUP_ON;
    PORT2.DSCR.BYTE |= _00_Pm3_HIDRV_OFF;
    PORT3.DSCR.BYTE |= _00_Pm0_HIDRV_OFF | _00_Pm1_HIDRV_OFF | _00_Pm2_HIDRV_OFF;
    PORT7.DSCR.BYTE |= _00_Pm0_HIDRV_OFF | _02_Pm1_HIDRV_ON | _00_Pm2_HIDRV_OFF | _08_Pm3_HIDRV_ON | 
                       _00_Pm4_HIDRV_OFF | _00_Pm5_HIDRV_OFF | _40_Pm6_HIDRV_ON;
    PORTB.DSCR.BYTE |= _00_Pm4_HIDRV_OFF | _00_Pm7_HIDRV_OFF;
    PORT2.PDR.BYTE = _08_Pm3_MODE_OUTPUT;
    PORT3.PDR.BYTE = _01_Pm0_MODE_OUTPUT | _02_Pm1_MODE_OUTPUT | _04_Pm2_MODE_OUTPUT;
    PORT4.PDR.BYTE = _02_Pm1_MODE_OUTPUT | _08_Pm3_MODE_OUTPUT | _20_Pm5_MODE_OUTPUT | _80_Pm7_MODE_OUTPUT;
    PORT7.PDR.BYTE = _00_Pm0_MODE_INPUT | _02_Pm1_MODE_OUTPUT | _00_Pm2_MODE_INPUT | _08_Pm3_MODE_OUTPUT | 
                     _00_Pm4_MODE_INPUT | _00_Pm5_MODE_INPUT | _40_Pm6_MODE_OUTPUT;
    PORTB.PDR.BYTE = _10_Pm4_MODE_OUTPUT | _80_Pm7_MODE_OUTPUT;
}

/* Start user code for adding. Do not edit comment generated here */
void U_PORT_Row_Write(int row)
{
    switch(row) {
    case 0:
        PORT2.PODR.BIT.B3 = 0;
        PORT3.PODR.BIT.B0 = 1;
        PORT3.PODR.BIT.B1 = 1;
        PORT3.PODR.BIT.B2 = 1;
        break;
    case 1:
        PORT2.PODR.BIT.B3 = 1;
        PORT3.PODR.BIT.B0 = 0;
        PORT3.PODR.BIT.B1 = 1;
        PORT3.PODR.BIT.B2 = 1;
        break;
    case 2:
        PORT2.PODR.BIT.B3 = 1;
        PORT3.PODR.BIT.B0 = 1;
        PORT3.PODR.BIT.B1 = 0;
        PORT3.PODR.BIT.B2 = 1;
        break;
    case 3:
        PORT2.PODR.BIT.B3 = 1;
        PORT3.PODR.BIT.B0 = 1;
        PORT3.PODR.BIT.B1 = 1;
        PORT3.PODR.BIT.B2 = 0;
        break;
    default:
        break;
    }
}

int U_PORT_Col_Read(void)
{
    unsigned char tmp = (PORT7.PIDR.BIT.B0 << 4) | (PORT7.PIDR.BIT.B2 << 5) | (PORT7.PIDR.BIT.B4 << 6) | (PORT7.PIDR.BIT.B5 << 7);
    int ret;
    switch(tmp) {
    case 0xE0://0B11100000
        ret = 1;
        break;
    case 0xD0://0B11010000
        ret = 2;
        break;
    case 0xB0://0B10110000
        ret = 3;
        break;
    case 0x70://0B01110000
        ret = 4;
        break;
    default:
        ret = 0;
        break;
    }
    return ret;
}

void U_PORT_Oled_SCL_Pin_Write(unsigned char value)
{
    PORT4.PODR.BIT.B1 = value;
}

void U_PORT_Oled_SDA_Pin_Write(unsigned char value)
{
    PORT4.PODR.BIT.B3 = value;
}

void U_PORT_Oled_RST_Pin_Write(unsigned char value)
{
    PORT4.PODR.BIT.B5 = value;
}

void U_PORT_Oled_DC_Pin_Write(unsigned char value)
{
    PORT4.PODR.BIT.B7 = value;
}

void U_PORT_Camera_mode_select(unsigned char mode)
{
    if (mode == CAM_MODE_BLACK || mode == CAM_MODE_GREEN)
        PORTB.PODR.BIT.B7 = mode;
}
/* End user code. Do not edit comment generated here */
