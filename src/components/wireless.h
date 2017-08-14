/*
 * wireless.h
 *
 *  Created on: 2017年8月12日
 *      Author: Cotyledon
 */

#ifndef COMPONENTS_WIRELESS_H_
#define COMPONENTS_WIRELESS_H_

#define SOUND_LIGHT 0x38
#define EMERGENCY   0x17
#define CAR_STOP    0x25
#define M5_START    0x9A
#define M5_STOP     0x9B
#define M5_LEFT     0x9C


#define CAR_COMMU_TASK_PRI  4
#define WIRELESS_FREQ       10

extern void car_commu_init(void);

#endif /* COMPONENTS_WIRELESS_H_ */
