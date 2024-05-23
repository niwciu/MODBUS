/**
 * @file usart.h
 * @author niwciu (niwciu@gmail.com)
 * @brief
 * @date 2024-05-23
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef _USART_H_
#define _USART_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

    void usart_init(void);
    void usart_send(void);
    void usart_recv(void);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _USART_H_ */
