/*
 * wireless.c
 *
 *  Created on: 2017年8月12日
 *      Author: Cotyledon
 */

#include <math.h>
/*-----------------------------------------------------------*/
/* RTOS & rx23t include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "platform.h"
#include "r_cg_sci.h"
#include "r_cg_mtu3.h"

#include "wireless.h"
#include "cam_commu.h"
#include "pos_control.h"
#include "mission.h"
#include "sonar.h"

static TaskHandle_t car_commu_taskhandle;
static unsigned char car_rx_buffer = 0;
static unsigned char car_tx_buffer = LAND;
static bool car_in_sight = pdFALSE;
static float distance;

/*-----------------------------------------------------------*/
/* private functions declaration. */
static void car_commu_task_entry(void *pvParameters);
static void sound_light(int open);

/*-----------------------------------------------------------*/
/* global functions definition. */

void car_commu_init(void)
{
    BaseType_t ret;

    ret = xTaskCreate(car_commu_task_entry,
                      "car_commu",
                      configMINIMAL_STACK_SIZE,
                      NULL,
                      CAR_COMMU_TASK_PRI,
                      &car_commu_taskhandle);
    configASSERT(ret == pdPASS);

    R_SCI5_Serial_Receive(&car_rx_buffer, 1);
    R_SCI5_Start();
}

void camera_finded(void)
{
    car_in_sight = pdTRUE;
}

void u_sci5_receiveend_callback(void)
{
    R_SCI5_Serial_Receive(&car_rx_buffer, 1);
}

void u_sci5_transmitend_callback(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(car_commu_taskhandle, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

static void car_commu_task_entry(void *pvParameters)
{
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    while (1) {
        if (car_rx_buffer == LAND) {
            car_stop();
        } else if (car_rx_buffer == EMERGENCY) {
            is_emergency_now();
        } else if (car_rx_buffer == START_CMD) {
            wireless_start_mission();
        }
        car_rx_buffer = 0;

        if (car_in_sight) {
            distance = sqrt(current_Height * current_Height
                                      + ((float)(mid_x - CAMERA_MID_X) * PIXEL_TO_DISTANCE_X / 100) * ((float)(mid_x - CAMERA_MID_X) * PIXEL_TO_DISTANCE_X / 100)
                                      + ((float)(mid_y - CAMERA_MID_Y) * PIXEL_TO_DISTANCE_Y / 100) * ((float)(mid_y - CAMERA_MID_Y) * PIXEL_TO_DISTANCE_Y / 100));
            if (distance < 1.51f && distance > 0.49f) {
                R_SCI5_Serial_Send(&car_tx_buffer, 1);
                ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(50));
                sound_light(1);
            } else {
                sound_light(0);
            }
        } else {
            sound_light(0);
        }
        car_in_sight = pdFALSE;
        try_to_find();

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1000/WIRELESS_FREQ));
    }
}

static void sound_light(int open)
{
    if (open) {
        LED3 = LED_ON;
        R_MTU3_C0_Start();
    } else {
        LED3 = LED_OFF;
        R_MTU3_C0_Stop();
    }
}
