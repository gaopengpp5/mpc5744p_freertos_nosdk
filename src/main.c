/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

#include "derivative.h" /* include peripheral declarations */
//#include "FS65xx_driver.h"
#include "MPC5744P_drv.h"
//#include "DSPI.h"
#include "ME.h"
//#include "ADC.h"
//#include "FCCU.h"
#include "SIUL.h"
#include "PIT.h"
//#include "CAN.h"

#include "FreeRTOS.h"



volatile int counter = 0;

void pit_ch1_ISR(void)
{
   	counter++;
   	if(counter>50)
   	{
   		counter = 0;
   		//SIUL_ToggleIO(SIUL_PC12);
   	}
    PIT_ClearFlag(1);		//clear interrupt TIF flag
}

extern void xcptn_xmpl(void);


#include <stdint.h>
#include <stdbool.h>
#include "task.h"

#define mainLED_DELAY                        ( ( TickType_t ) 500 / portTICK_PERIOD_MS )
#define mainLED_TASK_PRIORITY                ( tskIDLE_PRIORITY + 2 )

#define START_OS_DEMO 1


void vLEDTask1 (void *pvParameters)
{
	unsigned int ID = (unsigned int)pvParameters;
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = pdMS_TO_TICKS(1000);

	xLastWakeTime = xTaskGetTickCount();

	for(;;)
	{
		SIUL_ToggleIO(SIUL_PC11);
		vTaskDelayUntil( &xLastWakeTime, xFrequency );
	}
}


void vLEDTask2 (void *pvParameters)
{
	unsigned int ID = (unsigned int)pvParameters;
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = pdMS_TO_TICKS(2000);

	xLastWakeTime = xTaskGetTickCount();

	for(;;)
	{
		SIUL_ToggleIO(SIUL_PC12);
		vTaskDelayUntil( &xLastWakeTime, xFrequency );
	}
}

void vLEDTask3 (void *pvParameters)
{
	unsigned int ID = (unsigned int)pvParameters;
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = pdMS_TO_TICKS(3000);

	xLastWakeTime = xTaskGetTickCount();

	for(;;)
	{
		SIUL_ToggleIO(SIUL_PC13);
		vTaskDelayUntil( &xLastWakeTime, xFrequency );
	}
}



int main(void)
{
	//volatile int counter = 0;
	
	/* Enable all PBridge Masters for Reads, Writes, and Master Privilege Mode. */
	AIPS_0.MPRA.R = 0x77777777;
	AIPS_1.MPRA.R = 0x77777777;

	/* Configure FCCU */
	FCCU_SetProtocol(FCCU_BI_STABLE);		//set Bi-Stable protocol if not in the SAFE mode

	/* Init MODE ENTRY module */
	MC_MODE_INIT();                             /* (pll_init) Setup the MCU clocks and modes */

	xcptn_xmpl ();              /* Configure and Enable Interrupts */

	/* LED1 */
	SIUL_DigitalOutput(SIUL_PC11,SIUL_GPIO);
	SIUL_SetPad(SIUL_PC11);
	//SIUL_ClearPad(SIUL_PC11);
	/* LED2 */
	SIUL_DigitalOutput(SIUL_PC12,SIUL_GPIO);
	SIUL_SetPad(SIUL_PC12);
	//SIUL_ClearPad(SIUL_PC12);
	/* LED3 */
	SIUL_DigitalOutput(SIUL_PC13,SIUL_GPIO);
	SIUL_SetPad(SIUL_PC13);
	//SIUL_ClearPad(SIUL_PC13);

	/* Init PIT module for watchdog refresh */
	PIT_Init();
	PIT_Setup(1, PIT_CLK/1000000, 10000);  //1msec period
	PIT_EnableInt(1);

	PIT_EnableChannel(1);


#if START_OS_DEMO
	xTaskCreate( vLEDTask3, ( const char * const ) "LedTask3", configMINIMAL_STACK_SIZE, (void*)0x0, mainLED_TASK_PRIORITY, NULL );
	xTaskCreate( vLEDTask2, ( const char * const ) "LedTask2", configMINIMAL_STACK_SIZE, (void*)0x0, mainLED_TASK_PRIORITY, NULL );
	xTaskCreate( vLEDTask1, ( const char * const ) "LedTask1", configMINIMAL_STACK_SIZE, (void*)0x0, mainLED_TASK_PRIORITY, NULL );

  /* Start the scheduler. */
  vTaskStartScheduler();
#endif


	for(;;)
	{

	}
}
