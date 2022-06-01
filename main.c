#include "inc/tm4c1294ncpdt.h"
#include <stdint.h>
#include <stdlib.h>

void Delay(unsigned long counter); // used to add delay
void HC05_init(void); // Initialize UART5 module for HC-05
char Bluetooth_Read(void); //Read data from Rx5 pin of TM4C123
void Bluetooth_Write(unsigned char data); // Transmit a character to HC-05 over Tx5 pin
void Bluetooth_Write_String(char *str); // Transmit a string to HC-05 over Tx5 pin

int main(void)
 {

    HC05_init(); // call HC05_init() to initialze UART5 of TM4C123GH6PM

    /* Set PF1, PF2 and PF3 as digital output pins */

    SYSCTL_RCGCGPIO_R |= 0x1030;   /* enable clock to GPIOF  and GPION */
    GPIO_PORTF_AHB_DIR_R  |= 0x11;         //set PF0, PF4 a as digital output pin
    GPIO_PORTF_AHB_DEN_R |= 0x11;         // CON PF0, PF4  as digital GPIO pins

    GPIO_PORTN_DIR_R  |= 0x03;         //set PN0, PN1 a as digital output pin
    GPIO_PORTN_DEN_R |= 0x03;         // CON PN0, PN1  as digital GPIO pins
    GPIO_PORTE_AHB_DIR_R = 0x03;
    GPIO_PORTE_AHB_DEN_R = 0x03;

    Delay(10);

    while(1)
    {
        char c = Bluetooth_Read();          /* get a character from UART5 */

/* Check the received character and take action to control onboard LEDs of TM4C123 */
/* Send status string to Andriod app after turning on/off LEDs */

        if( c=='A'){
           GPIO_PORTF_AHB_DATA_R   |=(1<<0); //GPIO_PORTN_DATA_R  |=(1<<0); //GPIO_PORTF_AHB_DATA_R   |=(1<<0);
           Bluetooth_Write_String("ELEVADOR UP\n");
           GPIO_PORTE_AHB_DATA_R=0X01;
        }
        else if( c=='B'){
            GPIO_PORTF_AHB_DATA_R  &=~(1<<0); //GPIO_PORTN_DATA_R  &=~(1<<0);//GPIO_PORTF_AHB_DATA_R  &=~(1<<0);
            Bluetooth_Write_String("ELEVADOR DOWN\n");
            GPIO_PORTE_AHB_DATA_R=0X02;
        }
        else if( c=='C'){
            GPIO_PORTF_AHB_DATA_R  |=(1<<4);
            Bluetooth_Write_String("ELEVADOR STOP\n");
            GPIO_PORTE_AHB_DATA_R=0X00;
        }
        else if( c=='D'){
            GPIO_PORTF_AHB_DATA_R  &=~(1<<4);
            Bluetooth_Write_String("ELEVADOR\n");
        }
        if( c=='E'){
            GPIO_PORTN_DATA_R  |=(1<<0);// GPIO_PORTF_AHB_DATA_R   |=(1<<0);//GPIO_PORTN_DATA_R  |=(1<<0);
            Bluetooth_Write_String("Puerto N0 LED ON\n");
        }
        else if( c=='F'){
            GPIO_PORTN_DATA_R  &=~(1<<0);  //GPIO_PORTF_AHB_DATA_R  &=~(1<<0);// GPIO_PORTN_DATA_R  &=~(1<<0);
            Bluetooth_Write_String("Puerto N0 LED OFF\n");
        }

        if( c=='G'){
            GPIO_PORTN_DATA_R  |=(1<<1);// GPIO_PORTF_AHB_DATA_R   |=(1<<0);//GPIO_PORTN_DATA_R  |=(1<<0);
            Bluetooth_Write_String("Puerto N1 LED ON\n");
               }
         else if( c=='I'){
            GPIO_PORTN_DATA_R  &=~(1<<1);  //GPIO_PORTF_AHB_DATA_R  &=~(1<<0);// GPIO_PORTN_DATA_R  &=~(1<<0);
            Bluetooth_Write_String("I...NOLVIDABLE A SI ME DICEN\nMIS EX..AMORES\n");
               }
    }
}

void HC05_init(void)
{
    SYSCTL_RCGCUART_R |= 0x10;  /* enable clock to UART4 */
    SYSCTL_RCGCGPIO_R |= 0x200;  /* enable clock to PORTK for PK0/Rx and RK1/Tx */
    Delay(1);
    /* UART0 initialization */
            UART4_CTL_R = 0;         /* UART4 module disable */
           UART4_IBRD_R = 104;      /* for 9600 baud rate, integer = 104 */
           UART4_FBRD_R = 11;       /* for 9600 baud rate, fractional = 11*/
           UART4_CC_R = 0;          /*select system clock*/
           UART4_LCRH_R = 0x60;     /* data lenght 8-bit, not parity bit, no FIFO */
           UART4_CTL_R = 0x301;     /* Enable UART4 module, Rx and Tx */

           /* UART4 TX4 and RX4 use PK0 and PK1. Configure them digital and enable alternate function */
                   GPIO_PORTK_DEN_R    = 0x03;      /* set PK0 and PK1 as digital */
                   GPIO_PORTK_AFSEL_R = 0x03;    /* Use PK0,PK1 alternate function */
                   GPIO_PORTK_AMSEL_R  = 0;    /* Turn off analg function*/
                   GPIO_PORTK_PCTL_R = 0x00000011;     /* configure PK0 and PK1 for UART */
}

char Bluetooth_Read(void)
{
    char data;
      while((UART4_FR_R & (1<<4)) != 0); /* wait until Rx buffer is not full */
    data = UART4_DR_R ;      /* before giving it another byte */
    return (unsigned char) data;
}

void Bluetooth_Write(unsigned char data)
{
    while((UART4_FR_R & (1<<5))!= 0); /* wait until Tx buffer not full */
    UART4_DR_R = data;                  /* before giving it another byte */
}

void Bluetooth_Write_String(char *str)
{
  while(*str)
    {
        Bluetooth_Write(*(str++));
    }
}

void Delay(unsigned long counter)
{
    unsigned long i = 0;

    for(i=0; i< counter; i++);
}
