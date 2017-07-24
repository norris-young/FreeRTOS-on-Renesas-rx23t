/*
 * danger_check.c
 *
 *  Created on: 2017年7月23日
 *      Author: Cotyledon
 */

/* RTOS & rx23t include files. */
#include "FreeRTOS.h"
#include "timers.h"
#include "task.h"
#include "platform.h"
#include "r_cg_icu.h"

/*-----------------------------------------------------------*/
/* User include files. */
#include "mavlink_receive.h"
#include "danger_check.h"
#include "mission.h"

/*-----------------------------------------------------------*/
/* private variables */
/* danger check task handle structure, used for IRQ & TIMER send task notification. */
static TaskHandle_t danger_check_taskhandle;
static TimerHandle_t mission_tout_timerhandle;

/*-----------------------------------------------------------*/
/* private functions declaration. */
static void mission_timer_callback(TimerHandle_t mission_timer);
static void danger_check_task_entry(void *pvParameters);

/*-----------------------------------------------------------*/
/* global functions definition. */

/* ------------------------------------------------------------
 *
 * create the danger check task, mission time out TIMER,
 * and start IRQ.
 * NOTE:the TIMER is started by mission task later.
 *
 * ----------------------------------------------------------*/
void danger_check_init(void)
{
    BaseType_t ret;
    ret = xTaskCreate(danger_check_task_entry,
                      "danger_check",
                      configMINIMAL_STACK_SIZE,
                      NULL,
                      DANGER_TASK_PRI,
                      &danger_check_taskhandle);
    configASSERT(ret == pdPASS);

    mission_tout_timerhandle = xTimerCreate("mission_tout",
                                            MISSION_TOUT_TIME,
                                            pdFALSE,
                                            (void *)0,
                                            mission_timer_callback);

    R_ICU_IRQ1_Start();
}

/* ------------------------------------------------------------
 *
 * the function provide for mission task to start mission time
 * out TIMER.
 *
 * ----------------------------------------------------------*/
void start_mission_timer(void)
{
    xTimerStart(mission_tout_timerhandle, portMAX_DELAY);
}

/* ------------------------------------------------------------
 *
 * sends emergency remote signal to danger check task.
 *
 * ----------------------------------------------------------*/
void IRQ1_IntHandler(void)
{
    if(U_IRQ1_Pin_Read()) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        vTaskNotifyGiveFromISR(danger_check_taskhandle, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

/*-----------------------------------------------------------*/
/* private functions definition. */

/* ------------------------------------------------------------
 *
 * the real danger check task. check attitude every 100ms, and
 * then wait for emergency signal from mission time out TIMER
 * or remote control.
 *
 * ----------------------------------------------------------*/
static void danger_check_task_entry(void *pvParameters)
{
    while(1) {
        if(current_Roll>45.0 || current_Roll<-45.0 || current_Pitch>45.0 || current_Pitch<-45.0)
            is_emergency_now();
        if(ulTaskNotifyTake(pdTRUE, DANGER_CHECK_TIME) != 0)
            is_emergency_now();
    }
}

/* ------------------------------------------------------------
 *
 * the mission time out TIMER callback, it sends the emergency
 * signal to danger check task.
 *
 * ----------------------------------------------------------*/
static void mission_timer_callback(TimerHandle_t mission_timer)
{
    xTaskNotifyGive(danger_check_taskhandle);
}

