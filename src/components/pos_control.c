/*
 * pos_control.c
 *
 *  Created on: 2017年7月24日
 *      Author: Cotyledon
 */

/* RTOS & rx23t include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "platform.h"

/*-----------------------------------------------------------*/
/* User include files. */
#include "cam_commu.h"
#include "ppm_encoder.h"
#include "pid_control.h"
#include "pos_control.h"

/*-----------------------------------------------------------*/
/* private parameters */
static struct pid_param position_x_pp;
static struct pid_cfg   position_x_pc;
static struct pid_param position_y_pp;
static struct pid_cfg   position_y_pc;
static TaskHandle_t pos_ctl_taskhandle;

/*-----------------------------------------------------------*/
/* private functions declaration. */
static void pos_ctl_task_entry(void *pvParameters);
static void pos_pid_init(struct pid_param *pp, struct pid_cfg *pc);

/*-----------------------------------------------------------*/
/* global functions definition. */
void position_ctl_start(void)
{
    BaseType_t ret;

    pos_pid_init(&position_x_pp, &position_x_pc);
    pos_pid_init(&position_y_pp, &position_y_pc);
    position_x_pc.error_min = (float)POS_X_ERROR_MIN;
    position_y_pc.error_min = (float)POS_Y_ERROR_MIN;

    ret = xTaskCreate(pos_ctl_task_entry,
                      "pos_ctl",
                      configMINIMAL_STACK_SIZE * 2,
                      NULL,
                      POS_CTL_TASK_PRI,
                      &pos_ctl_taskhandle);
    configASSERT(ret == pdPASS);
}

void position_ctl_stop(void)
{
    vTaskDelete(pos_ctl_taskhandle);
}

/*-----------------------------------------------------------*/
/* private functions definition. */
static void pos_ctl_task_entry(void *pvParameters)
{
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    while (1) {
        LED0 = LED_ON;
        position_x_pc.error = (float)(CAMERA_MID_X - mid_x);
        position_y_pc.error = (float)(CAMERA_MID_Y - mid_y);

        pid_update(&position_x_pp, &position_x_pc);
        pid_update(&position_y_pp, &position_y_pc);

        send_ppm((uint16_t)(channel_val_MID + (int)position_x_pc.pid_out),
                 (uint16_t)(channel_val_MID + (int)position_y_pc.pid_out),
                 0, 0, 0, 0);

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1000/POS_PID_FREQ));
        LED0 = LED_OFF;
    }
}

static void pos_pid_init(struct pid_param *pp, struct pid_cfg *pc)
{
    pid_init(pp, pc);
    pp->kp = POS_KP;
    pp->ki = POS_KI;
    pp->kd = POS_KD;
    pp->dt = 1.0/((float)POS_PID_FREQ);
    pp->i_max = POS_I_MAX;
    pp->out_max = POS_OUT_MAX;
}

