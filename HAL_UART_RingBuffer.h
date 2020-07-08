//*****************************************************************************
//* UART Ring Buffer using HAL Library
//* (for STM32)
//*
//* File : HAL_UART_RingBuffer.h
//*
//* Version 1.0		Jul. 2020.
//* By NamCpp
//* 
//* License : GNU GPL(Free)
//*****************************************************************************

#ifndef __HAL_UART_RINGBUFFER_H
#define __HAL_UART_RINGBUFFER_H

#ifdef __cplusplus
extern "C" {
#endif

#define UART_TX_BUFFER_SIZE   1024
#define UART_RX_BUFFER_SIZE   1024

typedef struct  {
   uint8_t buffer[UART_RX_BUFFER_SIZE];
   uint8_t temp;
   uint8_t rxd;
   volatile uint16_t input_p;
   volatile uint16_t output_p;
} uart_hal_rx_type;

typedef struct  {
   uint8_t buffer[UART_TX_BUFFER_SIZE];
   volatile uint16_t input_p;
   volatile uint16_t output_p;
} uart_hal_tx_type;

extern uart_hal_rx_type uart_hal_rx;
extern uart_hal_tx_type uart_hal_tx;

void uart_hal_buffer_init(void);
uint8_t uart_hal_getchar(UART_HandleTypeDef *huart);
void uart_hal_putchar(UART_HandleTypeDef *huart, uint8_t *data, int len);

#ifdef __cplusplus
}
#endif

#endif /* __HAL_UART_RINGBUFFER_H */
