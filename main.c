/*
 * main.c
 * Author: dmanchon
 * Date:   22-Jan-2013
 * Brief:  4x4 keypad scan, using Stellaris Launchpad. Interruption method is used, so no continous polling is needed to detect key-press
 *
 */




#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"

//temporal variable where to store the key press number
static int num;

void setup(void){
	//Enable the driver layer
	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
	                       SYSCTL_XTAL_16MHZ);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

	//Pinout connections:
	//
	GPIOPinTypeGPIOInput(GPIO_PORTA_BASE,GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
	GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_STRENGTH_2MA,
	                     GPIO_PIN_TYPE_STD_WPU);
	GPIOIntTypeSet(GPIO_PORTA_BASE, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7,GPIO_FALLING_EDGE);
	GPIOPinTypeGPIOInput(GPIO_PORTB_BASE,GPIO_PIN_4);
	GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA,
		                     GPIO_PIN_TYPE_STD_WPU);
	GPIOIntTypeSet(GPIO_PORTB_BASE,GPIO_PIN_4,GPIO_FALLING_EDGE);
	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE,GPIO_PIN_5|GPIO_PIN_4);
	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE,GPIO_PIN_1|GPIO_PIN_0);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_3);

	GPIOPinIntEnable(GPIO_PORTA_BASE, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
	GPIOPinIntEnable(GPIO_PORTB_BASE,GPIO_PIN_4);
	IntMasterEnable();
	IntEnable(INT_GPIOA);
	IntEnable(INT_GPIOB);
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0);
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0);
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 0);

}

/* ISR to key-scan, i have been thinking to just raise a flag and then later on check keypad */
void scan_key(void)
{
	/* convention *='e' #='f' */
	int i;
	int key=0;

	/* switch case is heavily hard-coded to actual pin connection, i need to think how to parametrize
	 * this */

	IntMasterDisable(); //disable interrupts
	//we have 4 rows so check one by one
	for (i=0;i<4;i++)
	{
		GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, (i==0)?0:GPIO_PIN_0);
		GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, (i==1)?0:GPIO_PIN_1);
		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, (i==2)?0:GPIO_PIN_4);
		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, (i==3)?0:GPIO_PIN_5);

		if ( GPIOPinRead(GPIO_PORTA_BASE,GPIO_PIN_7) == 0)
		{
				switch (i)
				{
					case 0: key=0x0a;break;
					case 1: key=0x0b;break;
					case 2: key=0x0c;break;
					case 3: key=0x0d;break;
					default: key=-1;
				}
				break;
		}
		else if (GPIOPinRead(GPIO_PORTA_BASE,GPIO_PIN_6) == 0)
		{
				switch (i)
				{
					case 0: key=3;break;
					case 1: key=6;break;
					case 2: key=9;break;
					case 3: key=0x0f;break;
					default: key=-1;
				}
				break;
		}
		else if (GPIOPinRead(GPIO_PORTA_BASE,GPIO_PIN_5) == 0)
		{
				switch (i)
				{
					case 0: key=2;break;
					case 1: key=5;break;
					case 2: key=8;break;
					case 3: key=0;break;
					default: key=-1;
				}
				break;
		}
		else if (GPIOPinRead(GPIO_PORTB_BASE,GPIO_PIN_4) == 0)
		{
				switch (i)
				{
					case 0: key=1;break;
					case 1: key=4;break;
					case 2: key=7;break;
					case 3: key=0x0e;break;
					default: key=-1;
				}
				break;
		}
	}

	//get ready for next interruption
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0);
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0);
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 0);

	//store the value
	num = key;
	GPIOPinIntClear(GPIO_PORTB_BASE,GPIO_PIN_4);
	GPIOPinIntClear(GPIO_PORTA_BASE,GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
	IntMasterEnable();
}

void main(void) {
	//int num = 0;
	int i = 0;

	setup();

	
	for(;;)
	{
		//num = scan_key();
		if (num != 0)
		{
			for (i=0;i<num;i++)
			{
				GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
				SysCtlDelay(SysCtlClockGet()/10);
				GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);
				SysCtlDelay(SysCtlClockGet()/10);
			}
			num = 0;
		}
	}
}
