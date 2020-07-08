//*****************************************************************************
//* UART Ring Buffer using HAL Library
//* (for STM32)
//*
//* File : HAL_UART_RingBuffer.c
//*
//* Version 1.0		Jul. 2020.
//* By NamCpp
//* 
//* License : GNU GPL(Free)
//*****************************************************************************

#include <string.h>
#include "usart.h"
#include "gpio.h"
#include "HAL_UART_RingBuffer.h"

uart_hal_rx_type uart_hal_rx;
uart_hal_tx_type uart_hal_tx;

void uart_hal_buffer_init(void)
{	

   uart_hal_rx.input_p = 0;
   uart_hal_rx.output_p = 0;
   uart_hal_tx.input_p = 0;
   uart_hal_tx.output_p = 0;
}

uint8_t uart_hal_getchar(UART_HandleTypeDef *huart)
{
   uint32_t reg = READ_REG(huart->Instance->CR1);

   __HAL_UART_DISABLE_IT(huart, UART_IT_RXNE);
   if(uart_hal_rx.input_p == uart_hal_rx.output_p)
   {
      WRITE_REG(huart->Instance->CR1, reg);
      return 0;
   }
   WRITE_REG(huart->Instance->CR1, reg);
   uart_hal_rx.rxd = uart_hal_rx.buffer[uart_hal_rx.output_p++];
   if(uart_hal_rx.output_p >= UART_RX_BUFFER_SIZE)
   {
      uart_hal_rx.output_p = 0;
   }

   return 1;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
   uart_hal_rx.buffer[uart_hal_rx.input_p++] = uart_hal_rx.temp;
   if(uart_hal_rx.input_p >= UART_RX_BUFFER_SIZE)
   {
      uart_hal_rx.input_p = 0;
   }
   HAL_UART_Receive_IT(huart, &uart_hal_rx.temp, 1);
}

void uart_hal_putchar(UART_HandleTypeDef *huart, uint8_t *data, int len)
{
   uint16_t size = len;
   uint32_t reg = READ_REG(huart->Instance->CR1);

   __HAL_UART_DISABLE_IT(huart, UART_IT_TXE);
   __HAL_UART_DISABLE_IT(huart, UART_IT_TC);
   if((uart_hal_tx.input_p + len) >= UART_TX_BUFFER_SIZE)
   {
      size = UART_TX_BUFFER_SIZE - uart_hal_tx.input_p;
      memcpy(&uart_hal_tx.buffer[uart_hal_tx.input_p], data, size);
      memcpy(&uart_hal_tx.buffer[0], &data[size], len - size);
      uart_hal_tx.input_p = len - size;
   }
   else
   {
      memcpy(&uart_hal_tx.buffer[uart_hal_tx.input_p], data, size);
      uart_hal_tx.input_p += size;
   }
   WRITE_REG(huart->Instance->CR1, reg);
   HAL_UART_Transmit_IT(huart, &uart_hal_tx.buffer[uart_hal_tx.output_p], size);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
   uint16_t size;

   uart_hal_tx.output_p += huart->TxXferSize;
   if(uart_hal_tx.output_p >= UART_TX_BUFFER_SIZE)
   {
      uart_hal_tx.output_p -= UART_TX_BUFFER_SIZE;
   }
   if(uart_hal_tx.input_p != uart_hal_tx.output_p)
   {
      if(uart_hal_tx.input_p > uart_hal_tx.output_p)
      {
         size = uart_hal_tx.input_p - uart_hal_tx.output_p;
      }
      else
      {
         size = UART_TX_BUFFER_SIZE - uart_hal_tx.output_p;
      }
      HAL_UART_Transmit_IT(huart, &uart_hal_tx.buffer[uart_hal_tx.output_p], size);
   }
}