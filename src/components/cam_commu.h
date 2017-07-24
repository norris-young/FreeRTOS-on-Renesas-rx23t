/*
 * cam_commu.h
 *
 *  Created on: 2017年7月24日
 *      Author: Cotyledon
 */

#ifndef COMPONENTS_CAM_COMMU_H_
#define COMPONENTS_CAM_COMMU_H_

#define CAMERA_W            80
#define CAMERA_H            58
#define CAMERA_MID_X        39
#define CAMERA_MID_Y        28

#define COMMUNI_STX         0xFE
#define CAM_BUFFER_LENGTH   5

#define CAM_COMMU_TASK_PRI  3

extern volatile uint8_t mid_x, mid_y;

extern void cam_commu_init(void);

#endif /* COMPONENTS_CAM_COMMU_H_ */
