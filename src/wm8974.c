#include "main.h"

void WM8974_InitTask(void* pvParameters)
{
	txBuf[0] = 0x02;
	txBuf[1] = 0x0D;
	txBuf[2] = 0x04;
	txBuf[3] = 0x01;
	txBuf[4] = 0x06;
	txBuf[5] = 0x40;

	dataFrame[0].regData = (uint8_t*)&txBuf[0];
	dataFrame[0].len = 2;

	dataFrame[1].regData = (uint8_t*)&txBuf[2];
	dataFrame[1].len = 2;

	dataFrame[2].regData = (uint8_t*)&txBuf[4];
	dataFrame[2].len = 2;

	dataList.frame = &dataFrame[0];
	dataList.len = 3;
}

void I2S_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	I2S_InitTypeDef I2S_InitStructure;

	/* Enable the I2Sx/I2Sx_ext clock */
	I2Sx_CLK_INIT(I2Sx_CLK, ENABLE);
	//RCC_I2SCLKConfig(0x00);
	RCC_I2SCLKConfig(RCC_I2S2CLKSource_PLLI2S);
	RCC_PLLI2SCmd(ENABLE);

  
	/* Enable GPIO clocks */
	RCC_AHB1PeriphClockCmd(I2Sx_WS_GPIO_CLK | I2Sx_CK_GPIO_CLK | \
                           I2Sx_SD_GPIO_CLK | I2Sxext_SD_GPIO_CLK, ENABLE);

	/* I2S GPIO Configuration --------------------------------------------------*/
	/* Connect I2S pins to Alternate functions */  
	GPIO_PinAFConfig(I2Sx_WS_GPIO_PORT, I2Sx_WS_SOURCE, I2Sx_WS_AF);
	GPIO_PinAFConfig(I2Sx_CK_GPIO_PORT, I2Sx_CK_SOURCE, I2Sx_CK_AF);
	GPIO_PinAFConfig(I2Sx_SD_GPIO_PORT, I2Sx_SD_SOURCE, I2Sx_SD_AF);
	GPIO_PinAFConfig(I2Sx_MCK_GPIO_PORT, I2Sx_MCK_SOURCE, I2Sx_MCK_AF);
	GPIO_PinAFConfig(I2Sxext_SD_GPIO_PORT, I2Sxext_SD_SOURCE, I2Sxext_SD_AF);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

	/* I2S WS pin configuration */
	GPIO_InitStructure.GPIO_Pin = I2Sx_WS_PIN;
	GPIO_Init(I2Sx_WS_GPIO_PORT, &GPIO_InitStructure);

	/* I2S CK pin configuration */
	GPIO_InitStructure.GPIO_Pin =  I2Sx_CK_PIN;
	GPIO_Init(I2Sx_CK_GPIO_PORT, &GPIO_InitStructure);
  
	/* I2S SD pin configuration */
	GPIO_InitStructure.GPIO_Pin = I2Sx_SD_PIN;
	GPIO_Init(I2Sx_SD_GPIO_PORT, &GPIO_InitStructure);

	/* I2S MCK  pin configuration */
	GPIO_InitStructure.GPIO_Pin =  I2Sx_MCK_PIN;
	GPIO_Init(I2Sx_MCK_GPIO_PORT, &GPIO_InitStructure);

	/* I2S Extended SD pin configuration */
	GPIO_InitStructure.GPIO_Pin =  I2Sxext_SD_PIN;
	GPIO_Init(I2Sxext_SD_GPIO_PORT, &GPIO_InitStructure);
 
	/* I2S configuration -------------------------------------------------------*/
	SPI_I2S_DeInit(I2Sx);
	I2S_InitStructure.I2S_Mode = I2S_Mode_MasterRx;
	I2S_InitStructure.I2S_AudioFreq = I2S_AudioFreq_8k;
	I2S_InitStructure.I2S_Standard = I2S_Standard_Phillips;
	I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Enable;
	I2S_InitStructure.I2S_CPOL = I2S_CPOL_Low;
	I2S_InitStructure.I2S_DataFormat = I2S_DataFormat_16b;
	I2S_Init(I2Sx, &I2S_InitStructure);

	SPI_I2S_DMACmd(SPI3, SPI_I2S_DMAReq_Rx, ENABLE);

	I2S_Cmd(I2Sx, ENABLE);

}

void DMA_Config(uint16_t* pdata, uint16_t* pdata2)
{
	DMA_InitTypeDef DMA_InitStructure; 

	/* 使能DMA时钟 */
	RCC_AHB1PeriphClockCmd(DMA_STREAM_CLOCK, ENABLE);
  
	/* 重置DMA寄存器设置 */
	DMA_DeInit(DMA_STREAM);

	/* 在使能DMA流之前检查DMA是否处于关闭状态 
	 * 注意同一个流在不同的时候被使用这一步是很重要的
	 */

	while (DMA_GetCmdStatus(DMA_STREAM) != DISABLE)
	{

	}
  
	/* 设置DMA流 */
	DMA_InitStructure.DMA_Channel = DMA_CHANNEL;  
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)0x40003C0C;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)pdata;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = (uint32_t)32;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA_STREAM, &DMA_InitStructure);

	DMA_ITConfig(DMA_STREAM, DMA_IT_TC, ENABLE);

	/*DMA 双缓冲设置*/
	DMA_DoubleBufferModeConfig(DMA_STREAM, (uint32_t)pdata2, 0);
	DMA_DoubleBufferModeCmd(DMA_STREAM, ENABLE);

	//DMA_Cmd(DMA_STREAM, ENABLE);
}

void NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

