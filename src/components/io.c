/*
 * io.c
 *
 *  Created on: 2017年7月27日
 *      Author: Cotyledon
 */

/* RTOS & rx23t include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "platform.h"
#include "r_cg_sci.h"

/*-----------------------------------------------------------*/
/* User include files. */
#include "io.h"
#include "matrix_key.h"
#include "mission.h"
#include "pos_control.h"
#include "oled.h"
#include "printf-stdarg.h"

/*-----------------------------------------------------------*/
/* private variables */
static TaskHandle_t io_taskhandle;
static BaseType_t end = pdFALSE;
static int8_t course = 0;
static float input_tmp = 0.0f;
static course_state_e course_state = SELECT;
static input_state_e input_state = IDLE;
static uint8_t pageNum = MISSION_SELECT_PAGE;
static uint8_t ScreenStr[3][8][30] = {
                                      {"Mission 1",
                                       "Mission 2",
                                       "Mission 3",},
                                      {"Height:",
                                       "pos kp:",
                                       "pos ki:",
                                       "pos kd:",},
};
static float ScreenData[3][8] = {
                                 {0.0,},
                                 {0.3,
                                  POS_KP,
                                  POS_KI,
                                  POS_KD,},
};

static int8_t selected_mission = -1;

/*-----------------------------------------------------------*/
/* global variables */

/*-----------------------------------------------------------*/
/* private functions declaration. */
static void io_task_entry(void *pvParameters);
static void end_process(void);
static void page_init(uint8_t _pageNum);
static void key_enter(void);
static void key_back(void);
static void key_up(void);
static void key_down(void);
static void process_input(int _key_tmp);
static void process_number(float i);

/*-----------------------------------------------------------*/
/* global functions definition. */

void io_init(void)
{
    BaseType_t ret;

    ret = xTaskCreate(io_task_entry,
                      "io",
                      configMINIMAL_STACK_SIZE * 2,
                      NULL,
                      IO_TASK_PRI,
                      &io_taskhandle);
    configASSERT(ret == pdPASS);
}

void io_input(void)
{
    xTaskNotifyGive(io_taskhandle);
}

/*-----------------------------------------------------------*/
/* private functions definition. */

static void io_task_entry(void *pvParameters)
{
    oled_init();

    while (1) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        page_init(0);
        end = pdFALSE;
        while (!end) {
            int key_tmp = read_one_number();
            if (course_state == SELECT) {
                switch (key_tmp) {
                case KEY_ENTER:
                    key_enter();
                    break;
                case KEY_BACK:
                    key_back();
                    break;
                case KEY_UP:
                    key_up();
                    break;
                case KEY_DOWN:
                    key_down();
                    break;
                case KEY_COMPELETE:
                    end_process();
                    break;
                default:
                    break;
                }
            } else if (course_state == INPUT) {
                process_input(key_tmp);

            }
        }
    }
}

static void end_process(void)
{
    if (pageNum == MISSION_SETTING_PAGE) {
        send_mission_params(selected_mission,
                            ScreenData[MISSION_SETTING_PAGE][0],
                            ScreenData[MISSION_SETTING_PAGE][1],
                            ScreenData[MISSION_SETTING_PAGE][2],
                            ScreenData[MISSION_SETTING_PAGE][3]);

        oled_P6x8Str(8, 7, "Mission Start!!!");
        end = pdTRUE;
    }
}

static void page_init(uint8_t _pageNum)
{
    pageNum = _pageNum;

    oled_CLS();
    vTaskDelay(pdMS_TO_TICKS(200));

    switch (_pageNum) {
    case MISSION_SELECT_PAGE:
        for (int i = 0; i < 8; i++) {
            oled_P6x8Str(8, i, ScreenStr[MISSION_SELECT_PAGE][i]);
        }
        break;
    case MISSION_SETTING_PAGE:
        for (int i = 0; i < 8; i++) {
            oled_P6x8Str(8, i, ScreenStr[MISSION_SETTING_PAGE][i]);
            oled_PrintValueF(70, i, ScreenData[MISSION_SETTING_PAGE][i], 2);
        }
        break;
    default:
        break;
    }
    oled_P6x8Char(0, (uint8_t)course, '*');
}

static void key_enter(void)
{
    if (pageNum == MISSION_SELECT_PAGE) {
        selected_mission = course;
        pageNum = MISSION_SETTING_PAGE;
        page_init(pageNum);
    } else if (pageNum == MISSION_SETTING_PAGE) {
        oled_P6x8Char(0, (uint8_t)course, ' ');
        oled_P6x8Char(0, (uint8_t)course, '>');
        course_state = INPUT;
    }
}

static void key_back(void)
{
    if (pageNum == MISSION_SETTING_PAGE) {
        switch (course_state) {
        case SELECT:
            pageNum = MISSION_SELECT_PAGE;
            page_init(pageNum);
            break;
        case INPUT:
            course_state = SELECT;
            break;
        }
    }
}

static void key_up(void)
{
    oled_P6x8Char(0, (uint8_t)course, ' ');
    switch (pageNum) {
    case MISSION_SELECT_PAGE:
        course--;
        if (course < 0) course = MISSION_NUM - 1;
        break;
    case MISSION_SETTING_PAGE:
        course--;
        if (course < 0) course = 7;
        break;
    default:
        break;
    }
    oled_P6x8Char(0, (uint8_t)course, '*');
}

static void key_down(void)
{
    oled_P6x8Char(0, (uint8_t)course, ' ');
    switch (pageNum) {
    case MISSION_SELECT_PAGE:
        course++;
        if (course > MISSION_NUM - 1) course = 0;
        break;
    case MISSION_SETTING_PAGE:
        course++;
        if (course > 7) course = 0;
        break;
    default:
        break;
    }
    oled_P6x8Char(0, (uint8_t)course, '*');

}

static void process_input(int _key_tmp)
{
    switch (_key_tmp) {
    case 1: process_number(1.0f); break;
    case 2: process_number(2.0f); break;
    case 3: process_number(3.0f); break;
    case 5: process_number(4.0f); break;
    case 6: process_number(5.0f); break;
    case 7: process_number(6.0f); break;
    case 9: process_number(7.0f); break;
    case 10:process_number(8.0f); break;
    case 11:process_number(9.0f); break;
    case 14:process_number(0.0f); break;
    case 13:input_state = CANCEL; break;
    case 15:input_state = END;    break;
    case 4:case 8:case 12: input_state = DECIMAL_1; break;
    default: break;
    }
    oled_P6x8Str(70, (uint8_t)course, "         ");
    oled_PrintValueF(70, (uint8_t)course, input_tmp, 2);
    if (input_state == END) {
        ScreenData[pageNum][course] = input_tmp;
        input_tmp = 0.0f;
        input_state = IDLE;
        course_state = SELECT;
        oled_P6x8Char(0, (uint8_t)course, '*');
    } else if (input_state == CANCEL) {
        input_tmp = 0.0f;
        input_state = IDLE;
        course_state = SELECT;
        oled_P6x8Char(0, (uint8_t)course, '*');
        oled_P6x8Str(70, (uint8_t)course, "         ");
        oled_PrintValueF(70, (uint8_t)course, ScreenData[pageNum][course], 2);
    }
}

static void process_number(float i)
{
    switch (input_state) {
    case IDLE:
        input_tmp = i;
        input_state = INTEGER;
        break;
    case INTEGER:
        input_tmp = input_tmp * 10.0f + i;
        break;
    case DECIMAL_1:
        input_tmp += i / 10.0;
        input_state = DECIMAL_2;
        break;
    case DECIMAL_2:
        input_tmp += i / 100.0;
        input_state = END;
        break;
    }
}
