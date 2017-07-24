/*
 * pid_control.c
 *
 *  Created on: 2017年7月24日
 *      Author: Cotyledon
 */

/* User include files. */
#include "pid_control.h"

/*-----------------------------------------------------------*/
/* global functions definition. */

void pid_init(struct pid_param *pp, struct pid_cfg *pc)
{
    pp->kp          = DEFAULT_KP;
    pp->ki          = DEFAULT_KI;
    pp->kd          = DEFAULT_KD;
    pp->dt          = (float)(1/DEFAULT_PID_FREQ);
    pp->d_lpf_alpha = DEFAULT_LPFITER;
    pp->i_max       = DEFAULT_I_MAX;

    pc->last_error      = 0.0;
    pc->error           = 0.0;
    pc->error_min       = DEFAULT_ERROR_MIN;
    pc->proportion      = 0.0;
    pc->integrator      = 0.0;
    pc->last_derivative = 0.0;
    pc->derivative      = 0.0;
    pc->pid_out         = 0.0;
}

void pid_update(struct pid_param *pp, struct pid_cfg *pc)
{
    if (pc->error < pc->error_min && pc->error > -(pc->error_min)) {
        pc->integrator = 0;
        return;
    }
    pc->proportion = pp->kp * pc->error;

    // calculate integrator constrain in i_max.
    pc->integrator += pp->ki * pc->error * pp->dt;
    if (pc->integrator < -(pp->i_max)) {
        pc->integrator = -(pp->i_max);
    } else if (pc->integrator > pp->i_max) {
        pc->integrator = pp->i_max;
    }

    // calculate instantaneous derivative.
    pc->derivative = pp->kd * (pc->error - pc->last_error) / pp->dt;
    // discrete low pass filter, cuts out the
    // high frequency noise that can drive the controller crazy.
    pc->derivative = pc->last_derivative + pp->d_lpf_alpha * (pc->derivative - pc->last_derivative);
    // update state
    pc->last_error = pc->error;
    pc->last_derivative = pc->derivative;

    pc->pid_out = pc->proportion + pc->integrator + pc->derivative;
}
