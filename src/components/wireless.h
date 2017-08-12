/*
 * wireless.h
 *
 *  Created on: 2017年8月12日
 *      Author: Cotyledon
 */

#ifndef COMPONENTS_WIRELESS_H_
#define COMPONENTS_WIRELESS_H_

#define SOUND_LIGHT 0x25
#define START_CMD   0x32
#define EMERGENCY   0xAA
#define LAND        0x38

#define CAR_COMMU_TASK_PRI  4
#define WIRELESS_FREQ       10

extern void car_commu_init(void);

#endif /* COMPONENTS_WIRELESS_H_ */
