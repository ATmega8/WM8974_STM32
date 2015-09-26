#define I2C_SPEED 100000;

#define I2Cx_CLK                      RCC_APB1Periph_I2C1

#define I2Cx_SDA_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define I2Cx_SDA_PIN                  GPIO_Pin_9                
#define I2Cx_SDA_GPIO_PORT            GPIOB                       
#define I2Cx_SDA_SOURCE               GPIO_PinSource9
#define I2Cx_SDA_AF                   GPIO_AF_I2C1

#define I2Cx_SCL_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define I2Cx_SCL_PIN                  GPIO_Pin_8                
#define I2Cx_SCL_GPIO_PORT            GPIOB                    
#define I2Cx_SCL_SOURCE               GPIO_PinSource8
#define I2Cx_SCL_AF                   GPIO_AF_I2C1

#define I2Cx                          I2C1

#define I2Cx_DMA                      DMA1
#define I2Cx_DMA_CHANNEL              DMA_Channel_1
#define I2Cx_DR_ADDRESS               ((uint32_t)0x40005410)
#define I2Cx_DMA_STREAM_TX            DMA1_Stream6
#define I2Cx_DMA_STREAM_RX            DMA1_Stream0
#define I2Cx_TX_DMA_TCFLAG            DMA_FLAG_TCIF6
#define I2Cx_TX_DMA_FEIFLAG           DMA_FLAG_FEIF6
#define I2Cx_TX_DMA_DMEIFLAG          DMA_FLAG_DMEIF6
#define I2Cx_TX_DMA_TEIFLAG           DMA_FLAG_TEIF6
#define I2Cx_TX_DMA_HTIFLAG           DMA_FLAG_HTIF6
#define I2Cx_RX_DMA_TCFLAG            DMA_FLAG_TCIF0
#define I2Cx_RX_DMA_FEIFLAG           DMA_FLAG_FEIF0
#define I2Cx_RX_DMA_DMEIFLAG          DMA_FLAG_DMEIF0
#define I2Cx_RX_DMA_TEIFLAG           DMA_FLAG_TEIF0
#define I2Cx_RX_DMA_HTIFLAG           DMA_FLAG_HTIF0
#define DMAx_CLK                      RCC_AHB1Periph_DMA1 

/***********************************************
                           I2C Data Frame Define
						   ************************************************/

typedef struct
{
	uint8_t* regData;
	uint8_t len;
} I2C_DataFrameTypeDef;

typedef struct
{
	I2C_DataFrameTypeDef* frame;
	uint8_t len;
} I2C_DataListTypeDef;

/***********************************************
                        I2C State Machine Define
						****************************************/
typedef enum
{
	I2C_None,
	I2C_EV5 = 1,
	I2C_EV6,
	I2C_BTF,
	I2C_Timeout,
	I2C_DMA_TC
} I2C_EventTypeDef;

typedef enum
{
	Init,
	Start,
	SendAddress,
	SendData,
	Stop,
	Timeout,
	Error,
	Done
} I2C_StateTypeDef;

typedef struct
{
	I2C_StateTypeDef state;
	I2C_EventTypeDef event;

	I2C_DataFrameTypeDef* frame;

	uint8_t salveAddress;
} I2C_StateMachineTypeDef;



xQueueHandle I2CStateQueue;

void I2C_Config(void);
void I2C_DMAConfig(uint8_t* pTxBuf, uint8_t txLen);
void I2C_AllConfig(uint8_t* pTxBuf, uint8_t txLen);
void I2C_StateMachineDispatch(I2C_StateMachineTypeDef* stateMachine);

void I2C_WriteRegisterTask(void* pvParameters);
void I2C_WriteRegister(uint8_t addr, uint8_t reg, uint16_t data);

