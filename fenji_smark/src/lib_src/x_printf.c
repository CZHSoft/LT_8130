#include "includes.h"

#include "stdlib.h"

#include "x_udp.h"
#include "m_init.h"

/*
****************************************
20131119
暂无修改
****************************************
*/


//20110601 xu printf
void term_putc(char text);
char itohexstr(unsigned int value, char *string, int radix);
int xtm_printf(const char *fmt, ...);
int xtm_sprintf(char *s_fmt, const char *fmt, ...);

typedef char *va_list;
#define   _INTSIZEOF(n)   (   (sizeof(n)   +   sizeof(int)   -   1)   &   ~(sizeof(int)   -   1)   )
#define   va_start(ap,v)  (   ap   =   (va_list)&v   +   _INTSIZEOF(v)   )
#define   va_arg(ap,t)   (   *(t   *)((ap   +=   _INTSIZEOF(t))   -   _INTSIZEOF(t))   )
#define   va_end(ap)   (   ap   =   (va_list)0   )

//---------------------------------------------------------------------------
void term_putc(char text)
{
  while((inportb(UART0_TX_STATUS)&0x1)!=0x1);	// 等待发送保持器为空
    outportb(UART0_TX_DATA, text);				// 发送字符
}
//---------------------------------------------------------------------------
char itohexstr(unsigned int value, char *string, int radix)
{
  unsigned char shift;
  unsigned char data;
  unsigned char pos, pos1;
  unsigned long mask;
  unsigned char i;
  char string1[10];
  pos = 0;
  shift = radix*2 - 1;
  mask = 0xf0000000;
  //outportb(UART0_TX_DATA, value);
  while(1)
   {
    data = ((value & mask) >> (shift*4)) & 0x0f;
    //outportb(UART0_TX_DATA, data);
    switch(data)
     {
      case 0:
      case 1:
      case 2:
      case 3:
      case 4:
      case 5:
      case 6:
      case 7:
      case 8:
      case 9:
             string[pos] = data + '0';
             break;
      case 10:
      case 11:
      case 12:
      case 13:
      case 14:
      case 15:
             string[pos] = data - 10 + 'a';
             break;
     }
    pos ++;
    if(shift == 0)
      break;
    shift --;
    mask = mask >> 4;
   }
  string[pos] = '\0';
  pos1 = 0;
  for(i=0; i<8; i++)
   if(string[i] != '0')
    {
     pos1 = i;
     break;
    }
  if(pos1 > 6)
    pos1 = 6;
  xtm_memcopy(string1, string + pos1, 8 - pos1);
  string1[8 - pos1] = '\0';
  xtm_memcopy(string, string1, 8 - pos1 + 1);

  return 1;
}
/********************************************
*输出到串口
*     只支持以下格式输出:
*     s:字符串
*     d:十进制
*     x:十六进制
*     b:二进制
********************************************/
//---------------------------------------------------------------------------
#define DEBUG_MAX 200
int xtm_printf(const char *fmt, ...)
{
#if 1
  const char *s;
  int value;
  unsigned int value_hex;
  char buf[16];

  INT16U debug_pos;
  char debug_info[DEBUG_MAX];

  va_list ap;
  va_start(ap, fmt);
  debug_pos = 0;
  while (*fmt)
   {
    if (*fmt != '%')
     {
      debug_info[debug_pos] = *fmt++;
      term_putc(debug_info[debug_pos]);
      if((debug_pos + 1) < DEBUG_MAX)
        debug_pos ++;
      continue;
     }
    switch (*++fmt)
     {
      case 's':
               s = va_arg(ap, const char *);
               for ( ; *s != '\0'; s++)
                {
                 debug_info[debug_pos] = *s;
                 term_putc(debug_info[debug_pos]);
                 if((debug_pos + 1) < DEBUG_MAX)
                   debug_pos ++;
                }
               break;
      case 'd':                     //Signed decimal integer
               value = va_arg(ap, int);  //不同编译器/CPU位数,类型可能不同,uint8_t,uint16_t,uint32_t等
               itoa(value, buf, 10);
               for (s = buf; *s != '\0'; s++)
                {
                 debug_info[debug_pos] = *s;
                 term_putc(debug_info[debug_pos]);
                 if((debug_pos + 1) < DEBUG_MAX)
                   debug_pos ++;
                }
               break;
      case 'x':                     //Unsigned hexadecimal integer
      case 'X':
               value_hex = va_arg(ap, unsigned int);
               if(value_hex == 0x00)
                 strcpy(buf, "0000");
               else
                 itohexstr(value_hex, buf, 4);
               for (s = buf; *s != '\0'; s++)
                {
                 debug_info[debug_pos] = *s;
                 term_putc(debug_info[debug_pos]);
                 if((debug_pos + 1) < DEBUG_MAX)
                   debug_pos ++;
                }
               break;
      case 'b':                     //二进制打印
               value = va_arg(ap, int);
               //term_putbin_byte(value);
               debug_info[debug_pos] = 'b';
               term_putc(debug_info[debug_pos]);
               if((debug_pos + 1) < DEBUG_MAX)
                 debug_pos ++;
               break;
      /* 在下面增加其他格式 */
      default:
             break;
     }
    fmt++;
   }
  va_end(ap);

  debug_info[debug_pos] = '\0';
  send_debug_info(debug_info);
    
  return 1;   /* Dummy return value */
#endif  
}
//---------------------------------------------------------------------------
int xtm_sprintf(char *s_fmt, const char *fmt, ...)
{
  const char *s;
  int value;
  INT32U value_hex;
  INT8U precision, i, tmp;
  char buf[16];
  va_list ap;
  va_start(ap, fmt);
  precision = 0;
  while (*fmt)
   {
    if (*fmt != '%')
     {
      *s_fmt++ = *fmt++;
      continue;
     }
    if(*(fmt + 1) == '0')
     {
      precision = *(fmt+2) - '0';
      fmt += 2;
      //xtm_printf("precision = %d\n", precision);
     }
    switch (*++fmt)
     {
      case 's':
               s = va_arg(ap, const char *);
               for ( ; *s != '\0'; s++)
                {
                 *s_fmt++ = *s;
                }
               break;
      case 'd':                     //Signed decimal integer
               value = va_arg(ap, int);  //不同编译器/CPU位数,类型可能不同,uint8_t,uint16_t,uint32_t等
               itoa(value, buf, 10);

               if(precision)
                {
                 tmp = strlen(buf);
                 //xtm_printf("buf = %s, tmp = %d, precision = %d\n", buf, tmp, precision);
                 for(i=tmp; i<precision; i++)
                   *s_fmt++ = '0';
                }

               for (s = buf; *s != '\0'; s++)
                {
                 *s_fmt++ = *s;
                }
               break;
      case 'x':                     //Unsigned hexadecimal integer
      case 'X':
               value_hex = va_arg(ap, unsigned int);
               if(value_hex == 0x00)
                 strcpy(buf, "00");
               else
                 itohexstr(value_hex, buf, 4);
               if(precision)
                {
                 tmp = strlen(buf);
                 //xtm_printf("buf = %s, tmp = %d, precision = %d\n", buf, tmp, precision);
                 for(i=tmp; i<precision; i++)
                   *s_fmt++ = '0';
                }                 
               for (s = buf; *s != '\0'; s++)
                {
                 *s_fmt++ = *s;
                }
               break;
      case 'b':                     //二进制打印
               value = va_arg(ap, int);
               //term_putbin_byte(value);
               *s_fmt++ = 'b';
               break;
      /* 在下面增加其他格式 */
      default:
             break;
     }
    fmt++;
   }
  va_end(ap);
  return 1;   /* Dummy return value */
}
//---------------------------------------------------------------------------
#if 0
//20110601 xu printf
void term_putc(char text);
char itohexstr(INT32U value, char *string, int radix);
int xtm_printf(const char *fmt, ...);

typedef char *va_list;
#define   _INTSIZEOF(n)   (   (sizeof(n)   +   sizeof(int)   -   1)   &   ~(sizeof(int)   -   1)   )
#define   va_start(ap,v)(   ap   =   (va_list)&v   +   _INTSIZEOF(v)   )
#define   va_arg(ap,t)   (   *(t   *)((ap   +=   _INTSIZEOF(t))   -   _INTSIZEOF(t))   )
#define   va_end(ap)   (   ap   =   (va_list)0   )

#define   _INTSIZEOF_HEX(n)   (   (sizeof(n)   +   sizeof(INT32U)   -   1)   &   ~(sizeof(INT32U)   -   1)   )
#define   va_arg_hex(ap,t)   (   *(t   *)((ap   +=   _INTSIZEOF_HEX(t))   -   _INTSIZEOF_HEX(t))   )
//---------------------------------------------------------------------------
void term_putc(char text)
{
  while((inportb(UART0_TX_STATUS)&0x1)!=0x1);	// 等待发送保持器为空
    outportb(UART0_TX_DATA, text);				// 发送字符
}
//---------------------------------------------------------------------------
char itohexstr(INT32U value, char *string, int radix)
{
  unsigned char shift;
  unsigned char data;
  unsigned char pos, pos1;
  unsigned long mask;
  unsigned char i;
  char string1[10];
  pos = 0;
  shift = radix*2 - 1;
  mask = 0xf0000000;
  //outportb(UART0_TX_DATA, value);
  while(1)
   {
    data = ((value & mask) >> (shift*4)) & 0x0f;
    //outportb(UART0_TX_DATA, data);
    switch(data)
     {
      case 0:
      case 1:
      case 2:
      case 3:
      case 4:
      case 5:
      case 6:
      case 7:
      case 8:
      case 9:
             string[pos] = data + '0';
             break;
      case 10:
      case 11:
      case 12:
      case 13:
      case 14:
      case 15:
             string[pos] = data - 10 + 'a';
             break;
     }
    pos ++;
    if(shift == 0)
      break;
    shift --;
    mask = mask >> 4;
   }
  string[pos] = '\0';
  pos1 = 0;
  for(i=0; i<8; i++)
   if(string[i] != '0')
    {
     pos1 = i;
     break;
    }
  if(pos1 > 6)
    pos1 = 6;
  xtm_memcopy(string1, string + pos1, 8 - pos1);
  string1[8 - pos1] = '\0';
  xtm_memcopy(string, string1, 8 - pos1 + 1);

  return 1;
}
/********************************************
*输出到串口
*     只支持以下格式输出:
*     s:字符串
*     d:十进制
*     x:十六进制
*     b:二进制
********************************************/
//---------------------------------------------------------------------------
#define DEBUG_MAX 200
int xtm_printf(const char *fmt, ...)
{
  const char *s;
  int value;
  INT32U value_hex;
  char buf[16];

  INT16U debug_pos;
  char debug_info[DEBUG_MAX];
  debug_pos = 0;

  va_list ap;
  va_start(ap, fmt);
  while (*fmt)
   {
    if (*fmt != '%')
     {
      debug_info[debug_pos] = *fmt++;
      term_putc(debug_info[debug_pos]);
      if((debug_pos + 1) < DEBUG_MAX)
        debug_pos ++;
      continue;
     }
    switch (*++fmt)
     {
      case 's':
               s = va_arg(ap, const char *);
               for ( ; *s != '\0'; s++)
                {
                 debug_info[debug_pos] = *s;
                 term_putc(debug_info[debug_pos]);
                 if((debug_pos + 1) < DEBUG_MAX)
                   debug_pos ++;
                }
               break;
      case 'd':                     //Signed decimal integer
               value = va_arg(ap, int);  //不同编译器/CPU位数,类型可能不同,uint8_t,uint16_t,uint32_t等
               itoa(value, buf, 10);
               for (s = buf; *s != '\0'; s++)
                {
                 debug_info[debug_pos] = *s;
                 term_putc(debug_info[debug_pos]);
                 if((debug_pos + 1) < DEBUG_MAX)
                   debug_pos ++;
                }
               break;
      case 'x':                     //Unsigned hexadecimal integer
      case 'X':
               value_hex = va_arg_hex(ap, INT32U);
               itohexstr(value_hex, buf, 4);
               for (s = buf; *s != '\0'; s++)
                {
                 debug_info[debug_pos] = *s;
                 term_putc(debug_info[debug_pos]);
                 if((debug_pos + 1) < DEBUG_MAX)
                   debug_pos ++;
                }
               break;
      case 'b':                     //二进制打印
               value = va_arg(ap, int);
               //term_putbin_byte(value);
               debug_info[debug_pos] = 'b';
               term_putc(debug_info[debug_pos]);
               if((debug_pos + 1) < DEBUG_MAX)
                 debug_pos ++;
               break;
      /* 在下面增加其他格式 */
      default:
             break;
     }
    fmt++;
   }
  va_end(ap);

  debug_info[debug_pos] = '\0';
  send_debug_info(debug_info);
  return 1;   /* Dummy return value */
}
#endif
//---------------------------------------------------------------------------

