#include "bsp/uart.h"
#include <stdarg.h>

volatile unsigned int *const TxRxUART0 = UART0_TxRxFIFO0;
volatile unsigned int *const ctrlUART0 = UART0_CTRL;

void enable_uart0()
{
  *ctrlUART0 = XUARTPS_CR_TXEN | XUARTPS_CR_RXEN | XUARTPS_CR_TXRES | XUARTPS_CR_RXRES;
}

void print_uart0(const char *s)
{
  while (*s != '\0')
  {                                  /* Loop until end of string */
    *TxRxUART0 = (unsigned int)(*s); /* Transmit char */
    s++;                             /* Next char */
  }
}

static char *bf;
static char buf[12];
static unsigned int num;
static char uc;
static char zs;

static void out(char c)
{
  *bf++ = c;
}

static void outDgt(char dgt)
{
  out(dgt + (dgt < 10 ? '0' : (uc ? 'A' : 'a') - 10));
  zs = 1;
}

static void divOut(unsigned int div)
{
  unsigned char dgt = 0;
  num &= 0xffff; // just for testing the code  with 32 bit ints
  while (num >= div)
  {
    num -= div;
    dgt++;
  }
  if (zs || dgt > 0)
    outDgt(dgt);
}

void tfp_printf(char *fmt, ...)
{
  va_list va;
  char ch;
  char *p;

  va_start(va, fmt);

  while ((ch = *(fmt++)))
  {
    if (ch != '%')
    {
      *TxRxUART0 = (unsigned int)(ch); /* Transmit char */
    }
    else
    {
      char lz = 0;
      char w = 0;
      ch = *(fmt++);
      if (ch == '0')
      {
        ch = *(fmt++);
        lz = 1;
      }
      if (ch >= '0' && ch <= '9')
      {
        w = 0;
        while (ch >= '0' && ch <= '9')
        {
          w = (((w << 2) + w) << 1) + ch - '0';
          ch = *(fmt++);
        }
      }
      bf = buf;
      p = bf; // 缓冲区的开头
      zs = 0;
      switch (ch)
      {
      case 0:
        goto abort;
      case 'u':
      case 'd':
        num = va_arg(va, unsigned int);
        if (ch == 'd' && (int)num < 0)
        {
          num = -(int)num;
          out('-');
        }
        divOut(10000);
        divOut(1000);
        divOut(100);
        divOut(10);
        outDgt(num);
        break;
      /*case 'x':
      case 'X':
        uc = ch == 'X';
        num = va_arg(va, unsigned int);
        divOut(0x1000);
        divOut(0x100);
        divOut(0x10);
        outDgt(num);
        break; 
       case 'c':
         out((char)(va_arg(va, int)));
         break;*/
      case 's':
        p = va_arg(va, char *);
        break;
      case '%':
        out('%');
      default:
        break;
      }
      *bf = 0;
      bf = p;
      // *bf++;
      // if (*bf++ && w > 0) print_uart0("wow");
      while (*bf++ && w > 0)
      {
        w--;
      }
      while (w-- > 0)
      {
        *TxRxUART0 = (unsigned int)(lz ? '0' : ' ');
      }
      while ((ch = *p++))
      {
        *TxRxUART0 = (unsigned int)(ch); /* Transmit char */
      }
    }
  }
abort:;
  va_end(va);
}