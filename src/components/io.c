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
static course_bit_e course_bit = UNIT;
static course_state_e course_state = SELECT;
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
                                 {0.5,
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
static void key_left(void);
static void key_right(void);

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
        while (!end) {
            int key_tmp = read_one_number();
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
            case KEY_LEFT:
                key_left();
                break;
            case KEY_RIGHT:
                key_right();
                break;
            case KEY_COMPELETE:
                end_process();
                break;
            default:
                break;
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
    int i;
    pageNum = _pageNum;

    oled_CLS();
    vTaskDelay(pdMS_TO_TICKS(200));

    switch (_pageNum) {
    case MISSION_SELECT_PAGE:
        for (i = 0; i < 8; i++) {
            oled_P6x8Str(8, i, ScreenStr[MISSION_SELECT_PAGE][i]);
        }
        break;
    case MISSION_SETTING_PAGE:
        for (i = 0; i < 8; i++) {
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
        switch (course_state) {
        case SELECT:
            course_state = INPUT;
            course_bit = UNIT;
            break;
        case INPUT:
            course_state = SELECT;
            break;
        default:
            break;
        }
    } else {
        ;
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
    } else {
        ;
    }
}

static void key_up(void)
{
    if (course_state == SELECT) {
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
    } else if (course_state == INPUT) {
        switch (course_bit) {
        case HUNDRED:
            ScreenData[pageNum][course] += 100.0;
            break;
        case DECADE:
            ScreenData[pageNum][course] += 10.0;
            break;
        case UNIT:
            ScreenData[pageNum][course] += 1.0;
            break;
        case DECIMAL_FIRST:
            ScreenData[pageNum][course] += 0.1;
            break;
        case DECIMAL_SECOND:
            ScreenData[pageNum][course] += 0.01;
            break;
        default:
            break;
        }
        oled_P6x8Str(70, (uint8_t)course, "        ");
        oled_PrintValueF(70, (uint8_t)course, ScreenData[pageNum][course], 2);
    } else {
        /* impossible */;
    }
}

static void key_down(void)
{
    if (course_state == SELECT) {
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
    } else if (course_state == INPUT) {
        switch (course_bit) {
        case HUNDRED:
            ScreenData[pageNum][course] -= 100.0;
            break;
        case DECADE:
            ScreenData[pageNum][course] -= 10.0;
            break;
        case UNIT:
            ScreenData[pageNum][course] -= 1.0;
            break;
        case DECIMAL_FIRST:
            ScreenData[pageNum][course] -= 0.1;
            break;
        case DECIMAL_SECOND:
            ScreenData[pageNum][course] -= 0.01;
            break;
        default:
            break;
        }
        oled_P6x8Str(70, (uint8_t)course, "        ");
        oled_PrintValueF(70, (uint8_t)course, ScreenData[pageNum][course], 2);
    } else {
        /* impossible */;
    }
}

static void key_left(void)
{
    if (course_state == INPUT) {
        switch (course_bit) {
        case HUNDRED:
            break;
        case DECADE:
            course_bit = HUNDRED;
            break;
        case UNIT:
            course_bit = DECADE;
            break;
        case DECIMAL_FIRST:
            course_bit = UNIT;
            break;
        case DECIMAL_SECOND:
            course_bit = DECIMAL_FIRST;
            break;
        default:
            break;
        }
    }
}

static void key_right(void)
{
    if (course_state == INPUT) {
        switch (course_bit) {
        case HUNDRED:
            course_bit = DECADE;
            break;
        case DECADE:
            course_bit = UNIT;
            break;
        case UNIT:
            course_bit = DECIMAL_FIRST;
            break;
        case DECIMAL_FIRST:
            course_bit = DECIMAL_SECOND;
            break;
        case DECIMAL_SECOND:
            break;
        default:
            break;
        }
    }
}
