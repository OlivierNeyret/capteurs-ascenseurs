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
#include "CAN1.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "PDD_Includes.h"
#include "Init_Config.h"
/* User includes (#include below this line is not maintained by Processor Expert) */
#include "barometer.h"

#define ACC_ADDRESS 0x1D


#define ACC_REG_ADDRESS 0x00 //to be change
#define ACC_REG_SIZE 0x00 //to be change


static bool writeInSensorRegister(uint8_t* reg, uint8_t* value);
static bool readFromSensorRegister(uint8_t* reg, uint8_t* buffer);

LDD_DeviceData* i2c_component;
volatile bool dataI2CSent = FALSE;
volatile bool dataI2CReceived = FALSE;

volatile bool DataFrameTxFlg;
LDD_TDeviceData *can_component;
LDD_TError Error;
LDD_CAN_TFrame Frame;
uint8_t OutData[4] = {0x00U, 0x01U, 0x02U, 0x03U};                /* Initialization of output data buffer */

/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
	/* Write your local variable definition here */
	uint8_t buffer_acc[];
	uint8_t buffer_bar[3];
	can_component = CAN2_Init(NULL);                                     /* Initialization of CAN2 component */
	/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
	PE_low_level_init();
	/*** End of Processor Expert internal initialization.                    ***/

	/* Write your code here */
	/* For example: for(;;) { } */
	i2c_component = CI2C1_Init(NULL);
	initBarometer();
	while(1)
	{
		CI2C1_SelectSlaveDevice(i2c_component,LDD_I2C_ADDRTYPE_7BITS,ACC_ADDRESS);
		readAcceleration();
		CI2C1_SelectSlaveDevice(i2c_component,LDD_I2C_ADDRTYPE_7BITS,BAR_ADDRESS);
		readAltitude(buffer_bar);
	}

	Frame.MessageID = 0x123U;                                       /* Set Tx ID value - standard */
	Frame.FrameType = LDD_CAN_DATA_FRAME;                           /* Specyfying type of Tx frame - Data frame */
	Frame.Length = sizeof(OutData);                                 /* Set number of bytes in data frame - 4B */
	Frame.Data = OutData;                                           /* Set pointer to OutData buffer */
	DataFrameTxFlg = FALSE;                                         /* Initialization of DataFrameTxFlg */
	Error = CAN2_SendFrame(can_component, 0U, &Frame);                   /* Sends the data frame over buffer 0 */
	while (!DataFrameTxFlg) {                                       /* Wait until data frame is transmitted */
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

static bool writeInSensorRegister(uint8_t* reg, uint8_t* value)
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

static bool readFromSensorRegister(uint8_t* reg, uint8_t* buffer)
{
	LDD_TError error;
	dataI2CSent = FALSE;
	error = CI2C1_MasterSendBlock(i2c_component, reg, 1U, LDD_I2C_NO_SEND_STOP);
	while (!dataI2CSent) {}
	dataI2CSent = FALSE;
	error = CI2C1_MasterReceiveBlock(i2c_component, buffer, 1U, LDD_I2C_SEND_STOP);
	while (!dataI2CReceived) {}
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
