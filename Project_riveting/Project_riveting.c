/*
 * Project_riveting.c
 *
 * Created: 7/28/2015 11:52:56 AM
 *  Author: Raj
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU/USART_BAUDRATE*16UL))-1)
volatile int flag32ms=0,i=0;
volatile char array[]={};

char switch_read(int Switch_Pin_number)
{
	unsigned char cs=0,ls=1;
	unsigned char Read_Conform=0;
	cs = PINC & (1<<Switch_Pin_number);
	if (cs==0 && ls!=0)
	{
		_delay_ms(20);
		if (!(PINC & (1<<Switch_Pin_number)))
		{
			Read_Conform=1;
			return Read_Conform;
		}
	}
	ls=cs;
}

void uart_init(void)
{
	UCSRB|=(1<<RXEN)|(1<<TXEN);
	UCSRC|=(1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);
	UBRRH=(BAUD_PRESCALE >>8);
	UBRRL=BAUD_PRESCALE;
	UCSRB|=((1<<RXCIE)|(1<<TXCIE));
	sei();
}

int main(void)
{
	DDRD=0xFF;
	DDRC=0x00;
	TIMSK|=(1<<TOIE0);
	sei();
	TCCR0|=(1<<CS02) | (1<<CS00);
	char switch_counter=0;
	unsigned int timer_start=0,timer_stop=0,difference=0;
    uart_init();
	while(1)
    {
		if(switch_read(0))
		{
			TCNT0=0;
			timer_start=flag32ms;
			switch_counter++;
			if (switch_counter%2==0)
			{
				switch_counter=0;
				timer_stop=flag32ms;
			}
		}
		 difference= timer_stop-timer_start;
		 UDR='d';
		 itoa(difference,array,10);
		// UartSend=difference
		
        //TODO:: Please write your application code 
    }
}
ISR (TIMER0_OVF_vect)
{
	flag32ms++;
}
ISR (USART_RXC_vect)
{
	cli();
	sei();
}
ISR (USART_TXC_vect)
{
		UDR=array[i++];
		if (i>3)
		{
			i=0;
		}
}