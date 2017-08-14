/*
 * danger_check.h
 *
 *  Created on: 2017年7月23日
 *      Author: Cotyledon
 */

#ifndef COMPONENTS_DANGER_CHECK_H_
#define COMPONENTS_DANGER_CHECK_H_

#define MISSION_TOUT_TIME   pdMS_TO_TICKS(120000)
#define DANGER_CHECK_TIME   pdMS_TO_TICKS(250)
#define DANGER_TASK_PRI     6

extern void danger_check_init(void);
extern void start_mission_timer(void);
extern void stop_mission_timer(void);

#endif /* COMPONENTS_DANGER_CHECK_H_ */
