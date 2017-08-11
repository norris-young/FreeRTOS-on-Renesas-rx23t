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
#include "mission.h"

/*-----------------------------------------------------------*/
/* private variables, */
static uint8_t cam_rx_buffer[2][CAM_BUFFER_LENGTH];
static int cam_rx_buffer_pointer = 0;
static int cam_rx_pointer = 0;
static volatile bool start_receive = pdFALSE;
static volatile bool try_to_find_new = pdFALSE;

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

    cam_rx_buffer_pointer = 0;
    R_SCI5_Serial_Receive(cam_rx_buffer[0], CAM_BUFFER_LENGTH);
    R_SCI5_Start();
}

void try_to_find(void)
{
    try_to_find_new = pdTRUE;
}

void u_sci5_receiveend_callback(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(cam_commu_taskhandle, &xHigherPriorityTaskWoken);
    if (cam_rx_buffer_pointer == 1)
        R_SCI5_Serial_Receive(cam_rx_buffer[0], CAM_BUFFER_LENGTH);
    else
        R_SCI5_Serial_Receive(cam_rx_buffer[1], CAM_BUFFER_LENGTH);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/*-----------------------------------------------------------*/
/* private functions definition. */
static void cam_commu_task_entry(void *pvParameters)
{
    while (1) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        for (int i = 0; i < CAM_BUFFER_LENGTH; i++) {
            if (cam_rx_buffer[cam_rx_buffer_pointer][i] == COMMUNI_STX) {
                start_receive = pdTRUE;
                continue;
            }
            if (start_receive) {
                cam_rx_pointer++;
                if (cam_rx_pointer == 1) {
                    if (cam_rx_buffer[cam_rx_buffer_pointer][i] < CAMERA_W) {
                        mid_x = cam_rx_buffer[cam_rx_buffer_pointer][i];
                    } else {
                        cam_rx_pointer = 0;
                        start_receive = pdFALSE;
                    }
                } else if (cam_rx_pointer == 2) {
                    if (cam_rx_buffer[cam_rx_buffer_pointer][i] < CAMERA_H) {
                        mid_y = cam_rx_buffer[cam_rx_buffer_pointer][i];
                        if (try_to_find_new) {
                            camera_finded();
                            try_to_find_new = pdFALSE;
                        }
                    }
                    cam_rx_pointer = 0;
                    start_receive = pdFALSE;
                }
            }
        }
        cam_rx_buffer_pointer = cam_rx_buffer_pointer ? 0 : 1;
    }
}
