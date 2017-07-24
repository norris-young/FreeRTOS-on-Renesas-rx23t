/*
 * pid_control.h
 *
 *  Created on: 2017年7月24日
 *      Author: Cotyledon
 */

#ifndef COMPONENTS_PID_CONTROL_H_
#define COMPONENTS_PID_CONTROL_H_

#define DEFAULT_KP          1.0f
#define DEFAULT_KI          0.5f
#define DEFAULT_KD          0.01f
#define DEFAULT_ERROR_MIN   5.0f
#define DEFAULT_I_MAX       50.0f
#define DEFAULT_PID_FREQ    50

#define DEFAULT_LPFITER         7.9577e-3
// low pass filter:           0.079577472903393
// f_cut = 1/(2*PI*cutoff_freq)
// f_cut = 2 Hz -> _filter = 79.5774e-3
// f_cut = 10 Hz -> _filter = 15.9155e-3
// f_cut = 15 Hz -> _filter = 10.6103e-3
// f_cut = 20 Hz -> _filter =  7.9577e-3
// f_cut = 25 Hz -> _filter =  6.3662e-3
// f_cut = 30 Hz -> _filter =  5.3052e-3

struct pid_param {
    float kp;
    float ki;
    float kd;
    float dt;   //unit:s
    float d_lpf_alpha;
    float i_max;
};

struct pid_cfg {
    float last_error;
    float error;
    float error_min;
    float proportion;
    float integrator;
    float last_derivative;
    float derivative;
    float pid_out;
};

extern void pid_init(struct pid_param *pp, struct pid_cfg *pc);
extern void pid_update(struct pid_param *pp, struct pid_cfg *pc);

#endif /* COMPONENTS_PID_CONTROL_H_ */
