/*
 * mavlink_receive.h
 *
 *  Created on: 2017年7月23日
 *      Author: Cotyledon
 */

#ifndef COMPONENTS_MAVLINK_RECEIVE_H_
#define COMPONENTS_MAVLINK_RECEIVE_H_

#define MAVLINK_STX         0xFE

#define MSG_ALTITUDE_LENGTH 8
#define MSG_ALTITUDE_ID     173
#define MSG_HEIGHT_LENGTH   16

#define MSG_ATTITUDE_LENGTH 28
#define MSG_ATTITUDE_ID     30
#define MSG_ANGLE_LENGTH    36

#define MAV_BUFFER_LENGTH    5
#define MAVLINK_TASK_PRI    5

#define PI  3.1415926535897932384626433832795

enum
{
    MSG_START = 0,
    MSG_LEN = 1,
    MSG_SEQ = 2,
    MSG_ID = 5,
    RANGER_DISTANCE = 6,
    HEIGHT_BUFFER_END = 15,
    ANGLE_BUFFER_END = 35,
    ATTITUDE_current_Roll = 10,
    ATTITUDE_current_Pitch = 14,
    ATTITUDE_current_Yaw = 18,
};

extern volatile float current_Height;
extern volatile float current_Roll, current_Pitch, current_Yaw;

extern void mavlink_receive_init(void);

#endif /* COMPONENTS_MAVLINK_RECEIVE_H_ */
