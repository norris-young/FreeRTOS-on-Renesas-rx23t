/*
 * cam_commu.c
 *
 *  Created on: 2017年7月24日
 *      Author: Cotyledon
 */

/* RTOS & rx23t include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "platform.h"
#include "r_cg_sci.h"

/*-----------------------------------------------------------*/
/* User include files. */
#include "cam_commu.h"

/*-----------------------------------------------------------*/
/* private variables, */
static uint8_t rx_buffer[2][CAM_BUFFER_LENGTH];
static int rx_buffer_pointer = 0;
static int rx_pointer = 0;
static volatile bool start_receive = false;

static TaskHandle_t cam_commu_taskhandle;

/*-----------------------------------------------------------*/
/* global variables. */
volatile uint8_t mid_x = CAMERA_MID_X, mid_y = CAMERA_MID_Y;

/*-----------------------------------------------------------*/
/* private functions declaration. */
static void cam_commu_task_entry(void *pvParameters);

/*-----------------------------------------------------------*/
/* global functions definition. */
void cam_commu_init()
{
    BaseType_t ret;
    ret = xTaskCreate(cam_commu_task_entry,
                      "cam_commu",
                      configMINIMAL_STACK_SIZE,
                      NULL,
                      CAM_COMMU_TASK_PRI,
                      &cam_commu_taskhandle);
    configASSERT(ret == pdPASS);

    rx_buffer_pointer = 0;
    R_SCI5_Serial_Receive(rx_buffer[0], CAM_BUFFER_LENGTH);
    R_SCI5_Start();
}

void u_sci5_receiveend_callback(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(cam_commu_taskhandle, &xHigherPriorityTaskWoken);
    if (rx_buffer_pointer == 1)
        R_SCI1_Serial_Receive(rx_buffer[0], CAM_BUFFER_LENGTH);
    else
        R_SCI1_Serial_Receive(rx_buffer[1], CAM_BUFFER_LENGTH);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/*-----------------------------------------------------------*/
/* private functions definition. */
static void cam_commu_task_entry(void *pvParameters)
{
    int i;
    while (1) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        for (i = 0; i < CAM_BUFFER_LENGTH; i++) {
            if (rx_buffer[rx_buffer_pointer][i] == COMMUNI_STX) {
                start_receive = true;
            }
            if (start_receive) {
                rx_pointer++;
                if (rx_pointer == 1) {
                    if (rx_buffer[rx_buffer_pointer][i] < CAMERA_W) {
                        mid_x = rx_buffer[rx_buffer_pointer][i];
                    } else {
                        rx_pointer = 0;
                        start_receive = false;
                    }
                } else if (rx_pointer == 2) {
                    if (rx_buffer[rx_buffer_pointer][i] < CAMERA_H) {
                        mid_y = rx_buffer[rx_buffer_pointer][i];
                    }
                    rx_pointer = 0;
                    start_receive = false;
                }
            }
        }
        rx_buffer_pointer = rx_buffer_pointer ? 0 : 1;
    }
}
