#include "main.h"

void delay_ms(uint8_t sysclk, uint32_t timing);

int main(void)
{
	USART_Config();

	I2C_WriteRegister(0x34, 0x00, 0x01); /*软件复位*/

	I2C_WriteRegister(0x34, 0x01, 0x0D);

	/*while(i--)
	{
		while(j--);
	}*/

	delay_ms(168, 1000);

	I2C_WriteRegister(0x34, 0x01, 0x1D);
	I2C_WriteRegister(0x34, 0x02, 0x14); /*使能独立放大器，缓冲放大器和ADC*/
	delay_ms(168, 10);
	I2C_WriteRegister(0x34, 0x02, 0x15);
	I2C_WriteRegister(0x34, 0x04, 0x10); /*I2S 模式 16位数据*/
	I2C_WriteRegister(0x34, 0x06, 0x00); /*从模式，主时钟来源MCLK*/
	I2C_WriteRegister(0x34, 0x07, 0x0A); /*采样率 8K*/
	I2C_WriteRegister(0x34, 0x2C, 0x03);
	I2C_WriteRegister(0x34, 0x2D, 0x23);
	I2C_WriteRegister(0x34, 0x2F, 0x140);

	I2S_Config();

	DMA_Config((uint16_t*)&DMA_ReceiveBuf1, (uint16_t*)&DMA_ReceiveBuf2);
	NVIC_Config();

	DMA_Cmd(DMA1_Stream0, ENABLE);

	//xTaskCreate(WM8974_InitTask, "Task 1", 1000, NULL, 2, NULL);

	//vTaskStartScheduler();
	/* Infinite loop */

	while (1)
	{

	}

}


void delay_ms(uint8_t sysclk, uint32_t timing)
{
	uint32_t temp, fac_us, fac_ms; 

	SysTick->CTRL&=0xfffffffb;  //bit2清空,选择外部时钟*/  
	fac_us = sysclk/8;          
	fac_ms = fac_us*1000;

	SysTick->LOAD=(uint32_t)timing*fac_ms;      // 当倒数至零时，将被重装载的值 */
	SysTick->VAL =0x00;                 // VAL寄存器中存放着当前倒计数的值  */
	SysTick->CTRL=0x01 ;                // Systick定时器的使能位   开始计时 */
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));

	SysTick->CTRL=0x00;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器   
}  

