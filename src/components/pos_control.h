/*
 * pos_control.h
 *
 *  Created on: 2017年7月24日
 *      Author: Cotyledon
 */

#ifndef COMPONENTS_POS_CONTROL_H_
#define COMPONENTS_POS_CONTROL_H_

#define POS_KP          5.0f
#define POS_KI          0.0f
#define POS_KD          0.0f
#define POS_I_MAX       50.0f
#define POS_OUT_MAX     80.0f
#define POS_PID_FREQ    50
#define POS_X_ERROR_MIN 0
#define POS_Y_ERROR_MIN 0

#define POS_CTL_TASK_PRI    5

extern void position_ctl_start(void);
extern void position_ctl_stop(void);

#endif /* COMPONENTS_POS_CONTROL_H_ */
