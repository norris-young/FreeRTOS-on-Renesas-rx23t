/*
 * alt_control.c
 *
 *  Created on: 2017年8月10日
 *      Author: Cotyledon
 */

/* RTOS & rx23t include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "platform.h"

/*-----------------------------------------------------------*/
/* User include files. */
#include "alt_control.h"
#include "sonar.h"
#include "ppm_encoder.h"

static TaskHandle_t alt_ctl_taskhandle;
static float des_height;
static int out_of_range_times = 0;
static bool recorrect_height = false;


static void alt_ctl_task_entry(void *pvParameters);

/*-----------------------------------------------------------*/
/* global functions definition. */
void alt_ctl_start(float dest_height)
{
    BaseType_t ret;
    des_height = dest_height;
    ret = xTaskCreate(alt_ctl_task_entry,
                      "alt_ctl",
                      configMINIMAL_STACK_SIZE,
                      NULL,
                      ALT_CTL_TASK_PRI,
                      &alt_ctl_taskhandle);
    configASSERT(ret == pdPASS);
}

void alt_ctl_stop(void)
{
    vTaskDelete(alt_ctl_taskhandle);
}

/*-----------------------------------------------------------*/
/* private functions definition. */
static void alt_ctl_task_entry(void *pvParameters)
{
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    while (1) {
        if (recorrect_height) {
            if (current_Height < des_height - ALT_CTL_DEADZONE) {
                send_ppm(0,0,channel_percent(61),0,Alt_Hold,0);
            } else if (current_Height > des_height + ALT_CTL_DEADZONE) {
                send_ppm(0,0,channel_percent(39),0,Alt_Hold,0);
            } else {
                send_ppm(0,0,channel_percent(50),0,Alt_Hold,0);
                out_of_range_times = 0;
                recorrect_height = false;
            }
        } else if (current_Height < des_height - ALT_CTL_DEADZONE || current_Height > des_height + ALT_CTL_DEADZONE) {
            out_of_range_times++;
            if (out_of_range_times > 10) {
                recorrect_height = true;
            }
        }

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1000/ALT_CTL_FREQ));
    }
}

