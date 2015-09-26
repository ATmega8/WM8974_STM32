/*********************
				WM8974 Register Define
				*****************************/
/* Register Address */
#define SOFT_RESET_ADDR					0x00
#define PM1_ADDR                        0x01
#define PM2_ADDR                        0x02
#define PM3_ADDR                        0x03
#define AUDIO_INTERFACE_ADDR            0x04
#define COMPANDING_CTRL_ADDR            0x05
#define CLOCK_GEN_CTRL_ADDR             0x06
#define ADDITIONAL_CTRL_ADDR            0x07
#define GPIO_ADDR                       0x08
#define DAC_CONTROL_ADDR                0x0A
#define DAC_DIGITAL_VOL_ADDR            0x0B
#define ADC_CONTROL_ADDR                0x0E
#define EQ1_LOW_SHELF_ADDR              0x12
#define EQ2_PEAK_ADDR                   0x13
#define EQ3_PEAK_ADDR                   0x14
#define EQ4_PEAK_ADDR                   0x15
#define EQ5_HIGH_SHELF_ADDR             0x16

void WM8974_InitTask(void* pvParameters);

/***********************
					I2S Define
					*************************************/
#define I2Sx                           SPI3
#define I2Sxext                        I2S3ext
#define I2Sx_CLK                       RCC_APB1Periph_SPI3
#define I2Sx_CLK_INIT                  RCC_APB1PeriphClockCmd

#define I2Sx_MCK_PIN                   GPIO_Pin_7
#define I2Sx_MCK_GPIO_PORT             GPIOC
#define I2Sx_MCK_GPIO_CLK              RCC_AHB1Periph_GPIOC
#define I2Sx_MCK_SOURCE                GPIO_PinSource7
#define I2Sx_MCK_AF                    GPIO_AF_SPI3

#define I2Sx_WS_PIN                    GPIO_Pin_15
#define I2Sx_WS_GPIO_PORT              GPIOA
#define I2Sx_WS_GPIO_CLK               RCC_AHB1Periph_GPIOA
#define I2Sx_WS_SOURCE                 GPIO_PinSource15
#define I2Sx_WS_AF                     GPIO_AF_SPI3

#define I2Sx_CK_PIN                    GPIO_Pin_3
#define I2Sx_CK_GPIO_PORT              GPIOB
#define I2Sx_CK_GPIO_CLK               RCC_AHB1Periph_GPIOB
#define I2Sx_CK_SOURCE                 GPIO_PinSource3
#define I2Sx_CK_AF                     GPIO_AF_SPI3

#define I2Sx_SD_PIN                    GPIO_Pin_5
#define I2Sx_SD_GPIO_PORT              GPIOB
#define I2Sx_SD_GPIO_CLK               RCC_AHB1Periph_GPIOB
#define I2Sx_SD_SOURCE                 GPIO_PinSource5
#define I2Sx_SD_AF                     GPIO_AF_SPI3

#define I2Sxext_SD_PIN                 GPIO_Pin_11
#define I2Sxext_SD_GPIO_PORT           GPIOC
#define I2Sxext_SD_GPIO_CLK            RCC_AHB1Periph_GPIOC
#define I2Sxext_SD_SOURCE              GPIO_PinSource11
#define I2Sxext_SD_AF                  GPIO_AF_SPI3

/********************************
                          DMA Define
						  ***********************************/
#define DMA_STREAM            DMA1_Stream0
#define DMA_STREAM_CLOCK      RCC_AHB1Periph_DMA1

#define DMA_CHANNEL           DMA_Channel_0

#define SPI3_ADDR             0x40003C0C

void I2S_Config(void);
void DMA_Config(uint16_t* pdata, uint16_t* pdata2);
void NVIC_Config(void);

