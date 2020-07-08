//*****************************************************************************
//* UART Ring Buffer using HAL Library
//* (for STM32)
//*
//* File : example.c
//*
//* Version 1.0		Jul. 2020.
//* By NamCpp
//* 
//* License : GNU GPL(Free)
//*****************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include "usart.h"
#include "gpio.h"
#include "HAL_UART_RingBuffer.h"

UART_HandleTypeDef huart1;

int _write(int file, unsigned char *p, int len)
{
	for(int i=0;i<len;i++)
	{
      uart_hal_putchar(p, len);
	}
	return len;
}

void uart_hal_rx_monitor(UART_HandleTypeDef *huart)
{
   while(uart_hal_getchar(huart) != 0)
   {
      if(uart_hal_rx.rxd == '\r')
      {
         // To do...
      }
   }
}

int main(void)
{
   /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
   HAL_Init();

   SystemClock_Config();
   
   MX_GPIO_Init();
   MX_USART1_UART_Init();
   
   uart_hal_buffer_init();
   setvbuf(stdout, NULL, _IONBF, 0);
   
   printf("\r\n");
   printf("UART Ring Buffer using HAL Library\r\n");
   printf("Test Program by NamCpp\r\n");
   printf("\r\n");
   
   HAL_UART_Receive_IT(&huart1, &uart_hal_rx.temp, 1);
   
   uint32_t tick = HAL_GetTick();

   while(1)
   {
      uart_hal_rx_monitor(&huart1);
      if((HAL_GetTick() - tick) >= 1000)
      {
         tick = HAL_GetTick();
         printf("Tick : %lu\r\n", tick);
      }
   }
}
