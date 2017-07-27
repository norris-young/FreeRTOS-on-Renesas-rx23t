/*
 * mission.c
 *
 *  Created on: 2017年7月23日
 *      Author: Cotyledon
 */

/* RTOS & rx23t include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "platform.h"
#include "r_cg_icu.h"

/*-----------------------------------------------------------*/
/* User include files. */
#include "mavlink_receive.h"
#include "danger_check.h"
#include "ppm_encoder.h"
#include "pos_control.h"
#include "matrix_key.h"
#include "mission.h"

/*-----------------------------------------------------------*/
/* private variables */
/* mission task handle structure, used for IRQ send task notification. */
static TaskHandle_t mission_taskhandle;

/*-----------------------------------------------------------*/
/* private functions declaration. */
static void mission_task_entry(void *pvParameters);
static void arm(uint16_t flight_mode);
static void disarm(void);
static int read_dest_Height(void);
static void alt_hold(const float dest_Height);
static void red_led_warning();

/*-----------------------------------------------------------*/
/* global functions definition. */

/* ----------------------------------------------------------
 *
 * when danger check task(in danger_check.c) finds emergency
 * occurs, this function is called by that task to stop the
 * copter.
 *
 * --------------------------------------------------------*/
void is_emergency_now(void)
{
    LED2 = LED_ON;
    send_ppm(channel_val_MID,channel_val_MID,channel_val_MIN,channel_val_MID,Stabilize,EMERGENCY_ON);
    vTaskDelay(pdMS_TO_TICKS(1000));
    disarm();
}

/* ----------------------------------------------------------
 *
 * create the mission task, and start IRQ.
 *
 * --------------------------------------------------------*/
void mission_init(void)
{
    BaseType_t ret;
    ret = xTaskCreate(mission_task_entry,
                      "mission",
                      configMINIMAL_STACK_SIZE * 2,
                      NULL,
                      MISSION_TASK_PRI,
                      &mission_taskhandle);
    configASSERT(ret == pdPASS);

    R_ICU_IRQ0_Start();
}

/* ----------------------------------------------------------
 *
 * send start signal to the mission task.
 *
 * --------------------------------------------------------*/
void IRQ0_IntHandler(void)
{
    if(U_IRQ0_Pin_Read()) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        vTaskNotifyGiveFromISR(mission_taskhandle, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);;
    }
}

/*-----------------------------------------------------------*/
/* private functions definition. */

/* ----------------------------------------------------------
 *
 * the real task function, which is created by mission_init(),
 * read from matrix keyboard to select & execute different
 * missions.
 *
 * --------------------------------------------------------*/
static void mission_task_entry(void *pvParameters)
{
    int mission = -1;
    float dest_Height;
    while(1) {
        mission = read_one_number();
        switch(mission) {
        case MISSION_1:
            dest_Height = ((float)read_dest_Height())/100.0;
            LED2 = LED_ON;
            /* wait for start signal from IRQ which connected to a remote control. */
            xTaskNotifyStateClear(NULL);
            ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
            red_led_warning();
            start_mission_timer();
            alt_hold(dest_Height);
            stop_mission_timer();
            break;
        case MISSION_2:
            dest_Height = ((float)read_dest_Height())/100.0;
            /* wait for start signal from IRQ which connected to a remote control. */
            xTaskNotifyStateClear(NULL);
            ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
            red_led_warning();
            start_mission_timer();
            stop_mission_timer();
            break;
        case MISSION_3:
            dest_Height = ((float)read_dest_Height())/100.0;
            /* wait for start signal from IRQ which connected to a remote control. */
            xTaskNotifyStateClear(NULL);
            ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
            red_led_warning();
            start_mission_timer();
            stop_mission_timer();
            break;
        default:
            /* select a mission number which does not exist. */
            break;
        }
    }
}

/* ----------------------------------------------------------
 *
 * arm the copter, and only arm & disarm has a channel value
 * for emergency.
 *
 * --------------------------------------------------------*/
static void arm(uint16_t flight_mode)
{
    send_ppm(channel_val_MID,channel_val_MID,channel_val_MIN,channel_val_MAX,flight_mode,EMERGENCY_OFF);
    vTaskDelay(pdMS_TO_TICKS(4000));
    send_ppm(channel_val_MID,channel_val_MID,channel_val_MIN,channel_val_MID,flight_mode,EMERGENCY_OFF);
    vTaskDelay(pdMS_TO_TICKS(2000));
}

/* ----------------------------------------------------------
 *
 * disarm the copter, and only arm & disarm has a channel
 * value for emergency.
 *
 * --------------------------------------------------------*/
static void disarm(void)
{
    send_ppm(channel_val_MID,channel_val_MID,channel_val_MIN,channel_val_MIN,Stabilize,EMERGENCY_ON);
    vTaskDelay(pdMS_TO_TICKS(5000));
}

/* ----------------------------------------------------------
 *
 * base on the read_one_number, divide the 4*4 matrix keyboard
 * to a different function:
 * 1|2|3|A
 * 4|5|6|B
 * 7|8|9|C
 * ×|0|√|D
 *
 * --------------------------------------------------------*/
static int read_dest_Height(void)
{
    int dest_Height = 0;
    bool end_of_input = false;
    while(!end_of_input){
//        dest_h_int = dest_Height;
        switch(read_one_number()){
        case 1:
            dest_Height += 1;
            dest_Height *= 10;
            break;
        case 2:
            dest_Height += 2;
            dest_Height *= 10;
            break;
        case 3:
            dest_Height += 3;
            dest_Height *= 10;
            break;
        case 5:
            dest_Height += 4;
            dest_Height *= 10;
            break;
        case 6:
            dest_Height += 5;
            dest_Height *= 10;
            break;
        case 7:
            dest_Height += 6;
            dest_Height *= 10;
            break;
        case 9:
            dest_Height += 7;
            dest_Height *= 10;
            break;
        case 10:
            dest_Height += 8;
            dest_Height *= 10;
            break;
        case 11:
            dest_Height += 9;
            dest_Height *= 10;
            break;
        case 13:
            dest_Height = 0;
            break;
        case 14:
            dest_Height += 0;
            dest_Height *= 10;
            break;
        case 15:
            dest_Height /= 10;
            end_of_input = true;
            break;
        default:
            break;
        }
    }
    return dest_Height;
}

/* ----------------------------------------------------------
 *
 * the process in altitude hold mode, it is called by mission
 * task, and it's PPM does not include emergency channel or
 * other unnecessary channels, so that the danger check or
 * PID control won't be influenced.
 *
 * --------------------------------------------------------*/
static void alt_hold(const float dest_Height)
{
    /* arm & clime up. */
    arm(Alt_Hold);
    send_ppm(0,0,channel_percent(20),0,Alt_Hold,0);
    vTaskDelay(pdMS_TO_TICKS(500));
    send_ppm(0,0,channel_percent(50),0,Alt_Hold,0);
    vTaskDelay(pdMS_TO_TICKS(500));
    send_ppm(0,0,channel_percent(55),0,Alt_Hold,0);
    vTaskDelay(pdMS_TO_TICKS(500));
    send_ppm(0,0,channel_percent(60),0,Alt_Hold,0);
    vTaskDelay(pdMS_TO_TICKS(500));
    while(current_Height < dest_Height) {
        send_ppm(0,0,channel_percent(64),0,Alt_Hold,0);
    }

    /* arrives the destination height and hold for x milliseconds. */
    send_ppm(0,0,channel_percent(50),0,Alt_Hold,0);
    LED0 = LED_ON;
    position_ctl_start();
    vTaskDelay(pdMS_TO_TICKS(10000));
    position_ctl_stop();
    LED0 = LED_OFF;
    /* drop down & disarm. */
    while(current_Height > 0.1)
        send_ppm(channel_val_MID,channel_val_MID,channel_percent(37),0,Alt_Hold,0);
    disarm();
}

static void red_led_warning()
{
    LED2 = LED_OFF;
    vTaskDelay(pdMS_TO_TICKS(2000));
    LED2 = LED_ON;
    vTaskDelay(pdMS_TO_TICKS(10000));
    LED2 = LED_OFF;
}
