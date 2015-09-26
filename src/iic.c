#include "main.h"

void I2C_Config(void)
{
	I2C_InitTypeDef I2C_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

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
	I2C_InitStructure.I2C_OwnAddress1 = 0x10;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit ;
	I2C_Init(I2C1, &I2C_InitStructure);
   
	/* 配置IIC滤波器 */
	I2C_AnalogFilterCmd(I2Cx,ENABLE);
	I2C_DigitalFilterConfig(I2Cx,0x0F);
}

void I2C_AllConfig(uint8_t* pTxBuf, uint8_t txLen)
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


void I2C_DMAConfig(uint8_t* pTxBuf, uint8_t txLen)
{
	DMA_InitTypeDef DMA_InitStructure;

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
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)pTxBuf;
	DMA_InitStructure.DMA_BufferSize = txLen;
	DMA_DeInit(I2Cx_DMA_STREAM_TX);
	DMA_Init(I2Cx_DMA_STREAM_TX, &DMA_InitStructure);

}

void I2C_StateMachineDispatch(I2C_StateMachineTypeDef* stateMachine)
{
	switch(stateMachine->state)
	{
		case Init:
			I2C_DMAConfig(stateMachine->frame->regData, stateMachine->frame->len);
			I2C_GenerateSTART(I2C1, ENABLE);

		    stateMachine->state = Start;	

			break;

		case Start:
			if(stateMachine->event == I2C_EV5)
			{
				I2C_Send7bitAddress(I2C1, stateMachine->salveAddress, I2C_Direction_Transmitter);
				I2C_ITConfig(I2C1, I2C_IT_EVT, ENABLE);
				stateMachine->state = SendAddress;
			}
			else if(stateMachine->event == I2C_Timeout)
			{
				stateMachine->state = Timeout;
			}
			else
			{
				stateMachine->state = Error;	
			}

			break;

		case SendAddress:
			if(stateMachine->event == I2C_EV6)
			{
				I2C_DMACmd(I2C1, ENABLE);
				DMA_Cmd(DMA1_Stream6 , ENABLE);
				I2C_ITConfig(I2C1, I2C_IT_EVT, ENABLE);
				stateMachine->state = SendData;
			}
			else if(stateMachine->event == I2C_Timeout)
			{
				stateMachine->state = Timeout;
			}
			else
			{
				stateMachine->state = Error;	
			}
			break;

		case SendData:
			if(stateMachine->event == I2C_DMA_TC)
			{
				I2C_DMACmd(I2C1, DISABLE);
				stateMachine->state = Stop;
			}
			else if(stateMachine->event == I2C_Timeout)
			{
				stateMachine->state = Timeout;
			}
			else
			{
				stateMachine->state = Error;	
			}
			break;

		case Stop:
			if(stateMachine->event == I2C_BTF)
			{
					I2C_GenerateSTOP(I2C1, ENABLE);
					DMA_Cmd(DMA1_Stream6, DISABLE);

					while(DMA_GetCmdStatus(DMA1_Stream6) != DISABLE);
					DMA_ClearFlag(DMA1_Stream6, I2Cx_TX_DMA_TCFLAG | 
												I2Cx_TX_DMA_FEIFLAG | 
												I2Cx_TX_DMA_DMEIFLAG | 
												I2Cx_TX_DMA_TEIFLAG | 
												I2Cx_TX_DMA_HTIFLAG);
					I2C_ITConfig(I2C1, I2C_IT_EVT, ENABLE);
					stateMachine->state = Done;
			}
			else if(stateMachine->event == I2C_Timeout)
			{
				stateMachine->state = Timeout;
			}
			else
			{
				stateMachine->state = Error;	
			}
			break;

		case Timeout:
			break;

		case Error:
			break;

		case Done:
			break;

		default:
			break;
	}
}

void I2C_WriteRegisterTask(void* pvParameters)
{
	I2C_StateMachineTypeDef I2CStateMachine;	
	I2C_DataListTypeDef* dataList = (I2C_DataListTypeDef*)pvParameters;

	int32_t lReceivedValue, i;
	portBASE_TYPE xStatus;

	for(i = 0; i < dataList->len; i++)
	{
		I2CStateMachine.state = Init;
		I2CStateMachine.event = I2C_None;
		I2CStateMachine.frame = (dataList->frame+i);
		I2CStateMachine.salveAddress = 0x34;
	
		I2CStateQueue = xQueueCreate(5, sizeof(int32_t));

		while(I2CStateMachine.state != Done)
		{

			I2C_StateMachineDispatch(&I2CStateMachine);

			/*获取I2C状态*/
			xStatus = xQueueReceive(I2CStateQueue, &lReceivedValue, 100);

			if( xStatus == pdPASS)
			{
				if(lReceivedValue != 0)
					I2CStateMachine.event = lReceivedValue;
				else
					I2CStateMachine.event = I2C_Timeout;
			}
			else
			{
				I2CStateMachine.event = I2C_Timeout;
			}

		}	
	}
}

void I2C_WriteRegister(uint8_t addr, uint8_t reg, uint16_t data)
{
	uint8_t txRegBuf[2];

	txRegBuf[0] = (reg << 1) | (data >> 8);
	txRegBuf[1] = (uint8_t)(data & 0xFF);

	I2C_AllConfig((uint8_t*)&txRegBuf, 2);

	/*主机发送*/

	/*启动IIC*/
	I2C_Cmd(I2C1, ENABLE);

	/*产生开始信号*/
	I2C_GenerateSTART(I2C1, ENABLE);

	/*等待事件EV5*/
	while(!(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))); 

	/*发送器件地址*/
	I2C_Send7bitAddress(I2C1, addr, I2C_Direction_Transmitter);

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
}

