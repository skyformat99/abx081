#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "usart.h"
#include "app_error.h"
#include "at_cmd_set.h"
#include "at_cmd_port_serial.h"

#define APP_LOG_MODULE_NAME   "[at_cmd_serial]"
#define APP_LOG_MODULE_LEVEL   APP_LOG_LEVEL_ERROR   
#include "app_log.h"

extern UART_HandleTypeDef huart4;
UART_HandleTypeDef *ptr_at_cmd_port_serial_handle;

/*默认115200 8 N 1*/
void at_cmd_port_serial_init()
{ 
  ptr_at_cmd_port_serial_handle=&huart4;
  ptr_at_cmd_port_serial_handle->Instance=UART4;

  ptr_at_cmd_port_serial_handle->Init.BaudRate = AT_CMD_PORT_SERIAL_BAUDRATE;
  ptr_at_cmd_port_serial_handle->Init.WordLength =UART_WORDLENGTH_8B;
  ptr_at_cmd_port_serial_handle->Init.StopBits = UART_STOPBITS_1;
  ptr_at_cmd_port_serial_handle->Init.Parity = UART_PARITY_NONE;
  ptr_at_cmd_port_serial_handle->Init.Mode = UART_MODE_TX_RX;
  ptr_at_cmd_port_serial_handle->Init.HwFlowCtl = UART_HWCONTROL_NONE;
  ptr_at_cmd_port_serial_handle->Init.OverSampling = UART_OVERSAMPLING_16;
  if(HAL_UART_Init(ptr_at_cmd_port_serial_handle)!=HAL_OK)
  {
   APP_ERROR_HANDLER(0);
  }
}

void at_cmd_port_serial_enable(at_cmd_bool_t rx_bool,at_cmd_bool_t tx_bool)
{
 if(rx_bool)
  {
 /*使能接收中断*/
  __HAL_UART_ENABLE_IT(ptr_at_cmd_port_serial_handle,UART_IT_RXNE);
  APP_LOG_DEBUG("使能接收中断.\r\n"); 
  }
 else
 {
  __HAL_UART_DISABLE_IT(ptr_at_cmd_port_serial_handle,UART_IT_RXNE); 
  APP_LOG_DEBUG("禁止接收中断.\r\n"); 
 }
 if(tx_bool)
 {
  /*使能发送中断*/
  __HAL_UART_ENABLE_IT(ptr_at_cmd_port_serial_handle,/*UART_IT_TXE*/UART_IT_TC);   
  APP_LOG_DEBUG("使能发送中断.\r\n"); 
 }
 else
 {
 /*禁止发送中断*/
 __HAL_UART_DISABLE_IT(ptr_at_cmd_port_serial_handle, /*UART_IT_TXE*/UART_IT_TC);   
 APP_LOG_DEBUG("禁止发送中断.\r\n"); 
 }
}

void at_cmd_port_serial_send_byte(uint8_t send_byte)
{
 ptr_at_cmd_port_serial_handle->Instance->DR = send_byte;
 APP_LOG_ARRAY("S%d.\r\n",send_byte);
}

void at_cmd_port_serial_get_byte(uint8_t *ptr_byte)
{
 *ptr_byte = (uint8_t)(ptr_at_cmd_port_serial_handle->Instance->DR & (uint8_t)0x00FF);
  APP_LOG_ARRAY("R%d.\r\n",*ptr_byte);
}

void at_cmd_port_serial_isr(void)
{
  uint32_t tmp_flag = 0, tmp_it_source = 0; 
  
  tmp_flag = __HAL_UART_GET_FLAG(ptr_at_cmd_port_serial_handle, UART_FLAG_RXNE);
  tmp_it_source = __HAL_UART_GET_IT_SOURCE(ptr_at_cmd_port_serial_handle, UART_IT_RXNE);
  /*接收中断*/
  if((tmp_flag != RESET) && (tmp_it_source != RESET))
  { 
   at_cmd_byte_receive();
  }

  tmp_flag = __HAL_UART_GET_FLAG(ptr_at_cmd_port_serial_handle, /*UART_FLAG_TXE*/UART_FLAG_TC);
  tmp_it_source = __HAL_UART_GET_IT_SOURCE(ptr_at_cmd_port_serial_handle, /*UART_IT_TXE*/UART_IT_TC);
  /*发送中断*/
  if((tmp_flag != RESET) && (tmp_it_source != RESET))
  {
   at_cmd_byte_send();
  }  
}