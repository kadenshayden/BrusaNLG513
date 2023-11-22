/*
 * externLines.h
 *
 *  Created on: 2018Äê6ÔÂ7ÈÕ
 *      Author: hasee
 */

#ifndef INCLUDE_EXTERNLINES_H_
#define INCLUDE_EXTERNLINES_H_

extern inline void I2CA_Init(void);
extern void InitCMPSS(void);
extern void InitEPWM(void);
extern void InitADC(void);
extern inline void ConfigureADC(void);
extern void EnableDog(void);
extern void InitRAM(void);
extern void InitI2C(void);
extern inline void SetupADCSoftware(void);
extern void InitTzGpio(void);
extern void InitPowerSupply(void);

extern void InitECanaIDPointer(void);
extern void InitID(void);
void vCanBufferInitial(void);

extern void   vStartTask(void);
extern void	vRefTask(void);
extern void	vAdcTask(void);
extern void	vWarnTask(void);
extern void MyEnableInterrupts(void);
extern void vInitialInterrupts(void);

extern void CLA_initCpu1Cla1(void);
extern void CLA_configClaMemory(void);


__interrupt void Cla1Task1();
__interrupt void Cla1Task2();
__interrupt void Cla1Task3();
__interrupt void Cla1Task4();
__interrupt void Cla1Task5();
__interrupt void Cla1Task6();
__interrupt void Cla1Task7();
__interrupt void Cla1Task8();

extern uint32_t Cla1funcsLoadStart;
extern uint32_t Cla1funcsLoadEnd;
extern uint32_t Cla1funcsRunStart;
extern uint32_t Cla1funcsLoadSize;
extern uint32_t Cla1mathTablesLoadStart;
extern uint32_t Cla1mathTablesRunStart;
extern uint32_t Cla1mathTablesLoadSize;


extern const int16 Boot_Version[2];
#endif /* INCLUDE_EXTERNLINES_H_ */
