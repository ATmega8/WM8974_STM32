#include "main.h"

/*-----------------------------------
                           Audio Codec functions 
                                    ------------------------------------------*/
/* High Layer codec functions */
static uint32_t Codec_Init(uint32_t AudioFreq);
static uint32_t Codec_DeInit(void);
static uint32_t Codec_Play(void);
static uint32_t Codec_PauseResume(uint32_t Cmd);
static uint32_t Codec_Stop(uint32_t Cmd);
static uint32_t Codec_VolumeCtrl(uint8_t Volume);
static uint32_t Codec_Mute(uint32_t Cmd);
/* Low layer codec functions */
static void     Codec_CtrlInterface_Init(uint8_t* pTxBuf, uint8_t txLen);
static void     Codec_CtrlInterface_DeInit(void);
static void     Codec_AudioInterface_Init(uint32_t AudioFreq);
static void     Codec_AudioInterface_DeInit(void);
static void     Codec_Reset(void);
static uint32_t Codec_WriteRegister(uint8_t RegisterAddr, uint16_t RegisterValue);
static uint32_t Codec_ReadRegister(uint8_t RegisterAddr);
static void     Codec_GPIO_Init(void);
static void     Codec_GPIO_DeInit(void);

uint32_t     Delay(uint32_t sysclk, uint32_t timing);
/*----------------------------------------------------------------------------*/

/*-----------------------------------
                   MAL (Media Access Layer) functions 
                                    ------------------------------------------*/
/* Peripherals configuration functions */
static void     Audio_MAL_Init(uint16_t* pdata, uint16_t* pdata2);
static void     Audio_MAL_DeInit(void);
static void     Audio_MAL_Play(void);
static void     Audio_MAL_PauseResume(uint32_t Cmd, uint32_t Addr);
static void     Audio_MAL_Stop(void);
/*----------------------------------------------------------------------------*/

uint32_t WM8974_Init(uint32_t AudioFreq, uint16_t* pdata, uint16_t* pdata2)
{
	if(Codec_Init(AudioFreq) != 11)
	{
		return 0;
	}
	else
	{
		Audio_MAL_Init(pdata, pdata2);
		return 1;
	}
}

void WM8974_Start(void)
{
	Audio_MAL_Play();
}

static uint32_t Codec_Init(uint32_t AudioFreq)
{
	uint32_t count = 0;

	Codec_GPIO_Init();

	count += Codec_WriteRegister(0x00, 0x01); /*复位CODEC*/

	count += Codec_WriteRegister(0x01, 0x0D);

	Delay(168, 1000);

	count += Codec_WriteRegister(0x01, 0x1D);
	count += Codec_WriteRegister(0x02, 0x14); /*使能独立放大器，缓冲放大器和ADC*/

	Delay(168, 10);

	count += Codec_WriteRegister(0x02, 0x15);
	count += Codec_WriteRegister(0x04, 0x10); /*I2S 模式 16位数据*/
	count += Codec_WriteRegister(0x06, 0x00); /*从模式，主时钟来源MCLK*/
	count += Codec_WriteRegister(0x07, 0x0A); /*采样率 8K*/
	count += Codec_WriteRegister(0x2C, 0x03);
	count += Codec_WriteRegister(0x2D, 0x23);
	count += Codec_WriteRegister(0x2F, 0x140);

	Codec_AudioInterface_Init(AudioFreq);

	return count;
}

static uint32_t Codec_DeInit(void)
{
	return 0;
}

static uint32_t Codec_Play(void)
{
	return 0;
}

static uint32_t Codec_PauseResume(uint32_t Cmd)
{
	return 0;
}

static uint32_t Codec_Stop(uint32_t Cmd)
{
	return 0;
}

static uint32_t Codec_VolumeCtrl(uint8_t Volume)
{
	return 0;
}

static uint32_t Codec_Mute(uint32_t Cmd)
{
	return 0;
}

/* Low layer codec functions */
static void     Codec_CtrlInterface_Init(uint8_t* pTxBuf, uint8_t txLen)
{
	I2C_InitTypeDef I2C_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHB1PeriphClockCmd(I2Cx_SDA_GPIO_CLK, ENABLE);
    RCC_AHB1PeriphClockCmd(I2Cx_SCL_GPIO_CLK, ENABLE);

    RCC_APB1PeriphClockCmd(I2Cx_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd(DMAx_CLK, ENABLE);

	GPIO_InitStructure.GPIO_Pin = I2Cx_SCL_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(I2Cx_SCL_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = I2Cx_SDA_PIN;
	GPIO_Init(I2Cx_SDA_GPIO_PORT, &GPIO_InitStructure);

	GPIO_PinAFConfig(I2Cx_SCL_GPIO_PORT, I2Cx_SCL_SOURCE, I2Cx_SCL_AF);
	GPIO_PinAFConfig(I2Cx_SDA_GPIO_PORT, I2Cx_SDA_SOURCE, I2Cx_SDA_AF);

	I2C_InitStructure.I2C_ClockSpeed = I2C_SPEED;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0x32;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit ;
	I2C_Init(I2C1, &I2C_InitStructure);

	/* 关闭发送DMA IIC流 */
	DMA_Cmd(I2Cx_DMA_STREAM_TX, DISABLE);
	/* 解构发送DMA */
	DMA_DeInit(I2Cx_DMA_STREAM_TX);
	  
	DMA_InitStructure.DMA_Channel = I2Cx_DMA_CHANNEL;
	DMA_InitStructure.DMA_PeripheralBaseAddr = I2Cx_DR_ADDRESS;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

		/* DMA发送初始化 */
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)pTxBuf;
	DMA_InitStructure.DMA_BufferSize = txLen;
	DMA_DeInit(I2Cx_DMA_STREAM_TX);
	DMA_Init(I2Cx_DMA_STREAM_TX, &DMA_InitStructure);
   
	/* 配置IIC滤波器 */
	I2C_AnalogFilterCmd(I2Cx,ENABLE);
	I2C_DigitalFilterConfig(I2Cx,0x0F);
}

static void     Codec_CtrlInterface_DeInit(void)
{

}

static void     Codec_AudioInterface_Init(uint32_t AudioFreq)
{

	I2S_InitTypeDef I2S_InitStructure;

	/* Enable the I2Sx/I2Sx_ext clock */
	I2Sx_CLK_INIT(I2Sx_CLK, ENABLE);

	RCC_I2SCLKConfig(RCC_I2S2CLKSource_PLLI2S);
	RCC_PLLI2SCmd(ENABLE);

	/* I2S configuration -------------------------------------------------------*/
	SPI_I2S_DeInit(I2Sx);
	I2S_InitStructure.I2S_Mode = I2S_Mode_MasterRx;
	I2S_InitStructure.I2S_AudioFreq = AudioFreq;
	I2S_InitStructure.I2S_Standard = I2S_Standard_Phillips;
	I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Enable;
	I2S_InitStructure.I2S_CPOL = I2S_CPOL_Low;
	I2S_InitStructure.I2S_DataFormat = I2S_DataFormat_16b;
	I2S_Init(I2Sx, &I2S_InitStructure);
}

static void     Codec_AudioInterface_DeInit(void)
{

}

static void     Codec_Reset(void)
{

}

static uint32_t Codec_WriteRegister(uint8_t RegisterAddr, uint16_t RegisterValue)
{
	uint8_t txRegBuf[2];

	txRegBuf[0] = (RegisterAddr<< 1) | (RegisterValue >> 8);
	txRegBuf[1] = (uint8_t)(RegisterValue & 0xFF);

	/*主机发送*/
	Codec_CtrlInterface_Init((uint8_t*)&txRegBuf, 2);

	/*启动IIC*/
	I2C_Cmd(I2C1, ENABLE);

	/*产生开始信号*/
	I2C_GenerateSTART(I2C1, ENABLE);

	/*等待事件EV5*/
	while(!(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))); 

	/*发送器件地址*/
	I2C_Send7bitAddress(I2C1, DEVICE_ADDR, I2C_Direction_Transmitter);

	/*等待事件EV6*/
	while(!(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))); 

	/*开启DMA*/
	I2C_DMACmd(I2C1, ENABLE);
	DMA_Cmd(DMA1_Stream6 , ENABLE);

	/*等待DMA启动完成*/
	//while(DMA_GetCmdStatus(DMA1_Stream6) != ENABLE);

	/*等待DMA数据发送完成*/
	while(DMA_GetFlagStatus(DMA1_Stream6, DMA_FLAG_TCIF6) == RESET);

	/*关闭I2C DMA*/
	I2C_DMACmd(I2C1, DISABLE);

	/*等待字节传输完成*/
	while(!(I2C_GetFlagStatus(I2C1, I2C_FLAG_BTF)));

	/*发送停止信号*/
	I2C_GenerateSTOP(I2C1, ENABLE);

	/*关闭DMA*/
	DMA_Cmd(DMA1_Stream6, DISABLE);

	/*等待DMA关闭*/
	while(DMA_GetCmdStatus(DMA1_Stream6) != DISABLE);

	/*清除DMA相关标志*/
	DMA_ClearFlag(DMA1_Stream6, I2Cx_TX_DMA_TCFLAG | 
								I2Cx_TX_DMA_FEIFLAG | 
								I2Cx_TX_DMA_DMEIFLAG | 
			                    I2Cx_TX_DMA_TEIFLAG | 
								I2Cx_TX_DMA_HTIFLAG);
	/*关闭DMA时钟*/
	RCC_AHB1PeriphClockCmd(DMAx_CLK, DISABLE);

	return 1;
}

static uint32_t Codec_ReadRegister(uint8_t RegisterAddr)
{
	return 0;
}

static void     Codec_GPIO_Init(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;

	/* Enable GPIO clocks */
	RCC_AHB1PeriphClockCmd(I2Sx_WS_GPIO_CLK | I2Sx_CK_GPIO_CLK | \
                           I2Sx_SD_GPIO_CLK | I2Sxext_SD_GPIO_CLK | \
						   I2Cx_SDA_GPIO_CLK | I2Cx_SCL_GPIO_CLK, ENABLE);

	/* I2C GPIO Configuration --------------------------------------------------*/
	/* Connect I2C pins to Alternate functions */
	GPIO_PinAFConfig(I2Cx_SCL_GPIO_PORT, I2Cx_SCL_SOURCE, I2Cx_SCL_AF);
	GPIO_PinAFConfig(I2Cx_SDA_GPIO_PORT, I2Cx_SDA_SOURCE, I2Cx_SDA_AF);

	GPIO_InitStructure.GPIO_Pin = I2Cx_SCL_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(I2Cx_SCL_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = I2Cx_SDA_PIN;
	GPIO_Init(I2Cx_SDA_GPIO_PORT, &GPIO_InitStructure);


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
}

static void     Codec_GPIO_DeInit(void)
{

}

uint32_t     Delay(uint32_t sysclk, uint32_t timing)
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

	return 1;
}
/*----------------------------------------------------------------------------*/

/*-----------------------------------
                   MAL (Media Access Layer) functions 
                                    ------------------------------------------*/
/* Peripherals configuration functions */
static void     Audio_MAL_Init(uint16_t* pdata, uint16_t* pdata2)
{
	DMA_InitTypeDef DMA_InitStructure; 
    NVIC_InitTypeDef NVIC_InitStructure;

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

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	SPI_I2S_DMACmd(SPI3, SPI_I2S_DMAReq_Rx, ENABLE);
}

static void     Audio_MAL_DeInit(void)
{

}

static void     Audio_MAL_Play(void)
{
	DMA_Cmd(DMA_STREAM, ENABLE);
	I2S_Cmd(I2Sx, ENABLE);
}

static void     Audio_MAL_PauseResume(uint32_t Cmd, uint32_t Addr)
{

}

static void     Audio_MAL_Stop(void)
{

}
/*----------------------------------------------------------------------------*/

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

