#include "main.h"

void delay_ms(uint8_t sysclk, uint32_t timing);

int main(void)
{
	USART_Config();

	WM8974_Init(I2S_AudioFreq_8k, (uint16_t*)&DMA_ReceiveBuf1, (uint16_t*)&DMA_ReceiveBuf2);
	WM8974_Start();

	while(1)
	{

	}
}
  

