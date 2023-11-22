/*
 * Inverter_Base.h
 *
 *  Created on: 2018Äê12ÔÂ31ÈÕ
 *      Author: hasee
 */

#ifndef INCLUDE_INVERTER_BASE_H_
#define INCLUDE_INVERTER_BASE_H_


struct EPLL
{
	float32  f32Verr;			// Error
	float32  f32Vamp_Est;
	float32  f32deltaW_Est;
	float32  f32Theta;
	float32  f32Theta_pu;
	float32  f32Theta_Active;
	float32  f32Theta_Act_pu;
	float32  f32Sin_Theta;
	float32  f32Cos_Theta;
	int16	i16Vac_ADC;
	int16	i16Vac_Est;
	Uint16	u16ErrCnt;
};
extern	struct	EPLL	ePLL;
#endif /* INCLUDE_INVERTER_BASE_H_ */
