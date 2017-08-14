/*
 * cam_commu.h
 *
 *  Created on: 2017年7月24日
 *      Author: Cotyledon
 */

#ifndef COMPONENTS_CAM_COMMU_H_
#define COMPONENTS_CAM_COMMU_H_

#define CAMERA_W            160
#define CAMERA_H            120
#define CAMERA_MID_X        79
#define CAMERA_MID_Y        59

//#define MISSION_CAM_DZ_X      40
//#define MISSION_CAM_DZ_Y      30

#define COMMUNI_STX         0xFE
#define CAM_BUFFER_LENGTH   3

#define CAM_COMMU_TASK_PRI  4

#define CAM_MODE_BLACK      1
#define CAM_MODE_GREEN      0

extern volatile uint8_t mid_x, mid_y;
//extern volatile int mission_dz_count;

extern void cam_commu_init(void);
extern void try_to_find(void);

#endif /* COMPONENTS_CAM_COMMU_H_ */
