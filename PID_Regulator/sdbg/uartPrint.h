
//options

#define UART_USE_DMA_TRANSFER
//#define UART_USE_IT_TRANSFER
//#define UART_USE_BLOCK_TRANSFER
#define UART_BUFF_LEN	(50U)
#define UART_TIMEOUT	(0U)/* Only used when block transfer is defined */

extern uint16_t UART_print(UART_HandleTypeDef *huart , char* format,...);
