#ifndef _BSP_INT_H
#define _BSP_INT_H
#include "MCIMX6Y2.h"
#include "core_ca7.h"
#include "uart.h"
#include <stddef.h>
typedef void (*system_irq_handler_t) (unsigned int giccIar, void *param);

typedef struct _sys_irq_handle
{
    system_irq_handler_t irqHandler;
    void *userParam;
} sys_irq_handle_t;

void int_init(void);
void system_irqtable_init(void);
void system_register_irqhandler(IRQn_Type irq, system_irq_handler_t handler, void *userParam);
void system_irqhandler(unsigned int giccIar) ; 
void default_irqhandler(unsigned int giccIar, void *userParam); 



#endif
