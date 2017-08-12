/*
 * pos_control.h
 *
 *  Created on: 2017年7月24日
 *      Author: Cotyledon
 */

#ifndef COMPONENTS_POS_CONTROL_H_
#define COMPONENTS_POS_CONTROL_H_

#define POS_KP          1.5f
#define POS_KI          0.0f
#define POS_KD          0.38f
#define POS_I_MAX       15.0f
#define POS_OUT_MAX     25.0f
#define POS_PID_FREQ    50
#define POS_X_ERROR_MIN 0
#define POS_Y_ERROR_MIN 0

#define HEIGHT_TO_X             0.7333f
#define HEIGHT_TO_Y             0.5111f
#define PIXEL_TO_DISTANCE_X     (current_Height * 100 * HEIGHT_TO_X / (CAMERA_W / 2))
#define PIXEL_TO_DISTANCE_Y     (current_Height * 100 * HEIGHT_TO_Y / (CAMERA_H / 2))

#define POS_CTL_MIN_HEIGHT  0.2

#define POS_CTL_TASK_PRI    5

extern void position_ctl_start(int use_Default_PID, float kp, float ki, float kd);
extern void position_ctl_dest_set(int x_dest, int y_dest);
extern void position_ctl_stop(void);

#endif /* COMPONENTS_POS_CONTROL_H_ */
