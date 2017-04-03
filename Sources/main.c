/* ###################################################################
**     Filename    : main.c
**     Project     : Capteurs Ascensseur
**     Processor   : MK64FN1M0VLL12
**     Version     : Driver 01.01
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-02-10, 15:14, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.01
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "CI2C1.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "PDD_Includes.h"
#include "Init_Config.h"
/* User includes (#include below this line is not maintained by Processor Expert) */
#include "barometer.h"
#include "accelerometer.h"

#define ACC_REG_ADDRESS 0x00 //to be change
#define ACC_REG_SIZE 0x00 //to be change


bool writeInSensorRegister(LDD_TDeviceData* i2c_component, uint8_t* sensor, uint8_t* reg, uint8_t* value);
bool readFromSensorRegister(LDD_TDeviceData* i2c_component, uint8_t* sensor, uint8_t* reg, uint8_t* buffer);

volatile bool dataI2CSent = FALSE;
volatile bool dataI2CReceived = FALSE;

/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
	/* Write your local variable definition here */
	LDD_TDeviceData* i2c_component;
	uint8_t buffer_acc[6];
	int8_t buffer_bar[3];

	float altitude; //Altitude in meters
	float acceleration1[3]; //Acceleration in g, [0] -> X, [1] -> Y, [2] -> Z
	float acceleration2[3]; //same

	LDD_RealTime_Tfloat timeBetweenMesurement;

	/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
	PE_low_level_init();
	/*** End of Processor Expert internal initialization.                    ***/

	/* Write your code here */
	/* For example: for(;;) { } */
	LDD_TDeviceData* TimerAcc = RT1_Init(NULL); // timer init
	i2c_component = CI2C1_Init(NULL);
	CI2C1_SelectSlaveDevice(i2c_component,LDD_I2C_ADDRTYPE_7BITS,BAR_ADDRESS);
	initBarometer();
	CI2C1_SelectSlaveDevice(i2c_component,LDD_I2C_ADDRTYPE_7BITS,ACC_ADDRESS);
	initAccelerometer();
	while(1)
	{
		/* Data acquisition */
		CI2C1_SelectSlaveDevice(i2c_component,LDD_I2C_ADDRTYPE_7BITS,BAR_ADDRESS);
		readAltitude(buffer_bar);
		altitude = convertQ164toFloat(buffer_bar);

		CI2C1_SelectSlaveDevice(i2c_component,LDD_I2C_ADDRTYPE_7BITS,ACC_ADDRESS);

		readAcceleration(acceleration1);
		RT1_Enable(TimerAcc);// enable timer
		readAcceleration(acceleration2);
		RT1_Disable(TimerAcc);//disable timer

		RT1_GetTimeReal(TimerAcc, timeBetweenMesurement);//read timer
		RT1_Reset(TimerAcc);//reset timer

		/* Data processing */

		//Correction de l'accélération

		// -> calculer vitesse en fonction du temps
		// -> calculer vitesse instantanée

		// calcul position ?
		// -> integration de la vitesse ou utilisation du barometre ?

		/* Send results on CAN bus */
	}
	/*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */

bool writeInSensorRegister(LDD_TDeviceData* i2c_component, uint8_t* sensor, uint8_t* reg, uint8_t* value)
{
	LDD_TError error;
	dataI2CSent = FALSE;
	error = CI2C1_MasterSendBlock(i2c_component, reg, 1U, LDD_I2C_NO_SEND_STOP); // 1U means 1 unsigned
	while (!dataI2CSent) {}
	dataI2CSent = FALSE;
	error = CI2C1_MasterSendBlock(i2c_component, value, 1U, LDD_I2C_SEND_STOP);
	while (!dataI2CSent) {}
	dataI2CSent = FALSE;
	if(error == ERR_OK) return TRUE;
	return FALSE;
}

bool readFromSensorRegister(LDD_TDeviceData* i2c_component, uint8_t* sensor, uint8_t* reg, uint8_t* buffer)
{
	LDD_TError error;
	dataI2CSent = FALSE;
	error = CI2C1_MasterSendBlock(i2c_component, sensor, 1U, LDD_I2C_NO_SEND_STOP);
	while (!dataI2CSent) {}
	dataI2CSent = FALSE;
	error = CI2C1_MasterSendBlock(i2c_component, reg, 1U, LDD_I2C_NO_SEND_STOP);
	while (!dataI2CSent) {}
	dataI2CSent = FALSE;
	error = CI2C1_MasterReceiveBlock(i2c_component, buffer, 1U, LDD_I2C_SEND_STOP);
	while (!dataI2CReceived) {}
	dataI2CSent = FALSE;
	if(error == ERR_OK) return TRUE;
	return FALSE;
}

bool multipleRead(LDD_TDeviceData* i2c_component, uint8_t* sensor, uint8_t* reg, uint8_t* buffer, uint8_t nbOfRead)
{
	LDD_TError error;
	dataI2CSent = FALSE;
	int i;
	error = CI2C1_MasterSendBlock(i2c_component, sensor, 1U, LDD_I2C_NO_SEND_STOP);
	while (!dataI2CSent) {}
	dataI2CSent = FALSE;
	error = CI2C1_MasterSendBlock(i2c_component, reg, 1U, LDD_I2C_NO_SEND_STOP);
	while (!dataI2CSent) {}
	dataI2CSent = FALSE;
	for(i=0;i<nbOfRead;i++)
	{
		error = CI2C1_MasterReceiveBlock(i2c_component, buffer, 1U, LDD_I2C_SEND_STOP);
		while (!dataI2CReceived) {}
		dataI2CSent = FALSE;
	}
	if(error == ERR_OK) return TRUE;
	return FALSE;
}

/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
