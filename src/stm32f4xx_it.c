/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Template/stm32f4xx_it.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    30-September-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "main.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  *
void SVC_Handler(void)
{
}*/

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  *
void PendSV_Handler(void)
{
}

**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  *
void SysTick_Handler(void)
{

}*/

/**
  * @brief  This function handles EXTI 3 interrupt request.
  * @param  None
  * @retval None
  */

/**
  * @brief  This function handles EXTI 15-10 interrupt request.
  * @param  None
  * @retval None
  */

void DMA1_Stream0_IRQHandler(void)
{
	uint32_t i, target;

	if(DMA_GetITStatus(DMA1_Stream0, DMA_IT_TCIF0) != RESET)
	{
		DMA_ClearITPendingBit(DMA1_Stream0, DMA_IT_TCIF0);

		/*发送帧头*/
		USART_SendData(USART2, 0x46);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
		USART_SendData(USART2, 0xB9);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
		USART_SendData(USART2, 0x68);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);

		target = DMA_GetCurrentMemoryTarget(DMA1_Stream0);

		if( target == 1)
		{
			for(i = 0; i < 16; i++)
			{
				USART_SendData(USART2, (uint8_t)(DMA_ReceiveBuf1[2*i] >> 8));
				while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);		
				USART_SendData(USART2, (uint8_t)(DMA_ReceiveBuf1[2*i] & 0x000000FF));
				while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
			}
		}
		if( target == 0)
		{
			for(i = 0; i < 16; i++)
			{
				USART_SendData(USART2, (uint8_t)(DMA_ReceiveBuf2[2*i] >> 8));
				while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);		
				USART_SendData(USART2, (uint8_t)(DMA_ReceiveBuf2[2*i] & 0x000000FF));
				while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
			}
		}


		USART_SendData(USART2, 0x16);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	}
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
