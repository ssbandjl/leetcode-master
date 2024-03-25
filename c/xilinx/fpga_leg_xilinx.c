#include <stdio.h>
#include "xparameters.h"
#include "xil_cache.h"
#include "xgpio.h"
#include "gpio_header.h"
#define LED_CHANNEL 1
XGpio GpioOutput; /* The driver instance for GPIO Device configured as O/P */
XGpio GpioInput;
void delay(u32 ms)
{
	 volatile u32 Delay1=0;
	 volatile u32 Delay2=0;

	for (Delay1 = 0; Delay1 < ms; Delay1++)
	{
		for (Delay2 = 0; Delay2 < 8332; Delay2++);
	}
}
int main () 
{
   int Status;
   int cunt=0;
   Xil_ICacheEnable();
   Xil_DCacheEnable();
   Status = XGpio_Initialize(&GpioOutput, XPAR_AXI_GPIO_0_DEVICE_ID);
   	 if (Status != XST_SUCCESS)  {
   		  return XST_FAILURE;
   	 }
	 XGpio_SetDataDirection(&GpioOutput, LED_CHANNEL, 0x0);
	 XGpio_DiscreteWrite(&GpioOutput, LED_CHANNEL, 0x0);
while(1)
   {
      if(cunt==8)
    	  cunt=0;
      XGpio_DiscreteWrite(&GpioOutput, LED_CHANNEL ,0xf0);
      delay(500);
      XGpio_DiscreteWrite(&GpioOutput, LED_CHANNEL ,0x0f);
      delay(500);
      cunt++;
   }
   Xil_DCacheDisable();
   Xil_ICacheDisable();
   return 0;
}

