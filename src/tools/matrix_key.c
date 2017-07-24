/*
 * matrix_key.c
 *
 *  Created on: 2017年7月23日
 *      Author: Cotyledon
 */

#include "FreeRTOS.h"
#include "task.h"
#include "platform.h"
#include "r_cg_port.h"

/* --------------------------------------------------------
 *
 * read one number from a 4*4 matrix keyboard (value:1~16),
 * NOTE!!!: vTaskDelay() is used in this function, which
 * would push the calling task into block state.
 *
 * ------------------------------------------------------*/
int read_one_number(void)
{
    int i = 0;
    int row_val;
    int col_val;
    while(1) {
        row_val = i%4;
        U_PORT_Row_Write(row_val);

        col_val = U_PORT_Col_Read();
        if (col_val) {
            vTaskDelay(pdMS_TO_TICKS(200));
            if (col_val == U_PORT_Col_Read()) {
                /* wait for button is loosen. */
                while(U_PORT_Col_Read() == col_val);
                break;
            }
        }
        i++;
    }
    return row_val*4 + col_val;
}
