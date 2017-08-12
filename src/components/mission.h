/*
 * mission.h
 *
 *  Created on: 2017年7月23日
 *      Author: Cotyledon
 */

#ifndef COMPONENTS_MISSION_H_
#define COMPONENTS_MISSION_H_

#define DEST_HEIGHT_CUSHION 0.0f    /* units: meter */

#define MISSION_NUM         4
#define MISSION_TASK_PRI    3

#define NOTIFY_START_MISSION    0x01
#define NOTIFY_CAR_STOP         0x02
#define NOTIFY_INPUT_OVER       0x04

enum {
    MISSION_1 = 0,
    MISSION_2 = 1,
    MISSION_3 = 2,
    MISSION_4 = 3,
};

extern void is_emergency_now();
extern void mission_init(void);
extern void send_mission_params(int8_t _mission, float _dest_Height, float kp, float ki, float kd);
extern void camera_finded(void);
extern void wireless_start_mission(void);
extern void car_stop(void);

#endif /* COMPONENTS_MISSION_H_ */
