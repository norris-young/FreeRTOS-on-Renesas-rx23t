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

#define RX_BUFFER_LENGTH    5
#define MAVLINK_TASK_PRI    6

#define PI  3.1415926535897932384626433832795

extern volatile float current_Height;
extern volatile float current_Roll, current_Pitch, current_Yaw;

extern void mavlink_receive_init(void);

#endif /* COMPONENTS_MAVLINK_RECEIVE_H_ */
