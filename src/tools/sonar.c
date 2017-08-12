/*
 * sonar.c
 *
 *  Created on: 2017年8月12日
 *      Author: Cotyledon
 */

#include "platform.h"
#include "r_cg_mtu3.h"

volatile float current_Height;
static volatile float last_height = 0.0f;
static volatile unsigned short sonar_count = 0;
static const float sonar_lpf = 15.9155e-3;
static volatile bool first_edge = true;

void sonar_init(void)
{
    R_MTU3_C5_Start();
}

void sonar_interrupt(void)
{
    setpsw_i();
    if (first_edge) {
        sonar_count = 0;
        MTU5.TGRU = 0;
        MTU5.TIORU.BYTE = _12_MTU5_IOC_F;
        first_edge = false;
    } else {
        sonar_count = MTU5.TGRU;
        float sonar_dt = (float)sonar_count / 2500000 ;
        current_Height = sonar_dt / 2 * 340;
        current_Height = last_height + sonar_dt / (sonar_lpf + sonar_dt) * (current_Height - last_height);
        last_height = current_Height;
        MTU5.TIORU.BYTE = _11_MTU5_IOC_R;
        first_edge = true;
    }

}
