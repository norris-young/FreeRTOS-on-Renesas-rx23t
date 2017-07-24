/*
 * mission.h
 *
 *  Created on: 2017年7月23日
 *      Author: Cotyledon
 */

#ifndef COMPONENTS_MISSION_H_
#define COMPONENTS_MISSION_H_

#define MISSION_TASK_PRI 3

enum {
    MISSION_1 = 4,
    MISSION_2 = 8,
    MISSION_3 = 12,
};

extern void is_emergency_now();
extern void mission_init(void);

#endif /* COMPONENTS_MISSION_H_ */
