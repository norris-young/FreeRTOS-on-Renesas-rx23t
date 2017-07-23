/*
 * ppm_encoder.h
 *
 *  Created on: 2017年7月23日
 *      Author: Cotyledon
 */

#ifndef COMPONENTS_PPM_ENCODER_H_
#define COMPONENTS_PPM_ENCODER_H_

#include <stdint.h>

#define PPM_ENCODER_CHANNEL_NUM 8
#define PPM_ENCODER_DEFFAULT_CH_VAL 1021
#define PPM_USED_CHANN3EL_NUM 6
#define channel_val_MAX 1451
#define channel_val_RANGE 851
#define channel_val_MID 1021
#define channel_val_MIN 600
#define channel_percent(x) channel_val_MIN + channel_val_RANGE/10*(x)/10

typedef enum
{
    ROLL_CHANNEL = 0,
    PITCH_CHANNEL = 1,
    THROTTLE_CHANNEL = 2,
    YAW_CHANNEL = 3,
    MODE_CHANNEL = 4,
    TUNE_CHANNEL = 6,
}channel_name_e;

typedef enum
{
    Stabilize = 601,
    Alt_Hold = 1021,
    Land = 1450,
}flight_mode_e;

typedef enum
{
    EMERGENCY_ON = channel_val_MAX,
    EMERGENCY_OFF = channel_val_MIN,
}emergency_e;

typedef struct
{
    uint16_t ch_val[PPM_ENCODER_CHANNEL_NUM];
    //user needn't set
    uint16_t idle_val;
}ppm_data_t;

extern void set_ppm_channel(ppm_data_t *ppm_data, uint16_t* channel);
extern void ppm_encoder_init(void);
extern void ppm_encoder_set_data(ppm_data_t *ppm_data);
extern void send_ppm(uint16_t channel_roll ,uint16_t channel_pitch ,uint16_t channel_throttle ,
                        uint16_t channel_yaw ,uint16_t channel_mode ,uint16_t channel_emergency);


#endif /* COMPONENTS_PPM_ENCODER_H_ */
