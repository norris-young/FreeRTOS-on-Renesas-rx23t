/*
 * alt_control.h
 *
 *  Created on: 2017年8月10日
 *      Author: Cotyledon
 */

#ifndef COMPONENTS_ALT_CONTROL_H_
#define COMPONENTS_ALT_CONTROL_H_

#define ALT_CTL_TASK_PRI    5

#define ALT_CTL_DEADZONE    0.05   /* units:m */
#define ALT_CTL_FREQ        10

extern void alt_ctl_start(const float dest_height);
extern void alt_ctl_stop(void);

#endif /* COMPONENTS_ALT_CONTROL_H_ */
