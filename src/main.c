#include "main.h"

void delay_ms(uint8_t sysclk, uint32_t timing);

int main(void)
{
	uint8_t txdata[32] =	   {0x01, 0x01, 0x01, 0x01,
								0x01, 0x01, 0x01, 0x01,
								0x01, 0x01, 0x01, 0x01,
								0x01, 0x01, 0x01, 0x01,
								0x01, 0x01, 0x01, 0x01,
								0x01, 0x01, 0x01, 0x01,
								0x01, 0x01, 0x01, 0x01,
								0x01, 0x01, 0x01, 0x01};

	uint8_t rxdata[32] = {0};

    nRF24L01_InitTypeDef nRF24L01_InitStructure;

	nRF24L01_InitStructure.nRF24L01_Mode = NRF24L01_Mode_Receive;	
	nRF24L01_InitStructure.nRF24L01_Enable_AutoACK = NRF24L01_Enable_AutoACK_All;
	nRF24L01_InitStructure.nRF24L01_Enable_RxAddr = NRF24L01_Enable_RxAddr_All;
	nRF24L01_InitStructure.nRF24L01_RF_Channal = 0x40;
	nRF24L01_InitStructure.nRF24L01_RF_DataRate = NRF24L01_RF_DataRate_2Mbps;
	nRF24L01_InitStructure.nRF24L01_RF_OutputPower = NRF24L01_RF_OutputPower_0dBm;

	if( nRF24L01_Init(&nRF24L01_InitStructure) != NRF24L01_OK)
		goto ERROR;
	else
		nRF24L01_ReceiveData((uint8_t*)&rxdata);




	USART_Config();

	WM8974_Init(I2S_AudioFreq_8k, (uint16_t*)&DMA_ReceiveBuf1, (uint16_t*)&DMA_ReceiveBuf2);
	WM8974_Start();

ERROR:

	while(1)
	{

	}
}
  

