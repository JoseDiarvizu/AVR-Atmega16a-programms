/*
 * Titulo
 *
 * Created: Fecha
 * Author : Nombre
 */ 
 
#define FOSC 2000000
#define BAUD 9600
#define  F_CPU 2000000UL
#define MYUBRR FOSC/16/BAUD-1
#include <avr/io.h>				//Librería general del Microcontrolador
#include <stdint.h>				//Para poder declarar varialbes especiales
#include <util/delay.h>			//Para poder hacer delays
#include <avr/interrupt.h>	 //Para poder manejar interrupciones
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
//PROTOTIPADO DE FUNCIONES PARA PODER UTILIZARLAS DESDE CUALQUIER "LUGAR"
//*************************************************************************
uint8_t cero_en_bit(volatile uint8_t *LUGAR, uint8_t BIT);
uint8_t uno_en_bit(volatile uint8_t *LUGAR, uint8_t BIT);
void USART_init(uint16_t ubrr);
void USART_Transmit(uint8_t dato);
//*************************************************************************
volatile uint8_t contSensor = 0;
volatile uint16_t datoTemp = 0, datoPh = 0, datoHum = 0;
volatile uint8_t currentValue = 0;

int main(void)
{
	sei();
	USART_init(MYUBRR);
	ADMUX = 0B01000000;
	ADCSRA = 0B10011100;
	DDRB = 0b11111111;
	DDRC = 0b11111111;
	DDRD = 0b00000010;
	ADCSRA |= (1<<ADSC);
	while (1)
	{
		
	}
}

ISR(ADC_vect)
{
	uint16_t res  = ADC;
	float resDisplay = ((float)res*255)/1023;
	float resLeds = ((float)res*5)/1023;
	int floatFloor =(int) resDisplay;
	uint8_t conversion = (uint8_t) floatFloor;
	if (contSensor == 1)
	{
		datoHum = conversion;
		currentValue = PINB & 0B00001111;
		if (resLeds>= 0.0 && resLeds<1.0)
		{
			PORTB = currentValue | 0;
		}
		else if (resLeds>= 1.0 && resLeds<2.0)
		{
			PORTB = currentValue | 0b00010000;
		}
		else if (resLeds>= 2.0 && resLeds<3.0)
		{
			PORTB = currentValue | 0b00110000;
		}
		else if (resLeds>= 3.0 && resLeds<4.0)
		{
			PORTB = currentValue | 0b01110000;
		}
		else if (resLeds>= 4.0 && resLeds<=5.0)
		{
			PORTB = currentValue | 0b11110000;
		}
	}
	else if (contSensor == 0)
	{
		datoTemp = conversion;
		currentValue = PINB & 0B11110000;
		if (resLeds>= 0.0 && resLeds<1.0)
		{
			PORTB = currentValue | 0;
		}
		else if (resLeds>= 1.0 && resLeds<2.0)
		{
			PORTB = currentValue | 0b00000001;
		}
		else if (resLeds>= 2.0 && resLeds<3.0)
		{
			PORTB = currentValue | 0b00000011;
		}
		else if (resLeds>= 3.0 && resLeds<4.0)
		{
			PORTB = currentValue | 0b00000111;
		}
		else if (resLeds>= 4.0 && resLeds<=5.0)
		{
			PORTB = currentValue | 0b00001111;
		}
	}
	else if (contSensor == 2)
	{
		datoPh = conversion;
		if (resLeds>= 0.0 && resLeds<1.0)
		{
			PORTC = 0;
		}
		else if (resLeds>= 1.0 && resLeds<2.0)
		{
			PORTC = 0B00000001;
		}
		else if (resLeds>= 2.0 && resLeds<3.0)
		{
			PORTC = 0B00000011;
		}
		else if (resLeds>= 3.0 && resLeds<4.0)
		{
			PORTC = 0B00000111;
		}
		else if (resLeds>= 4.0 && resLeds<=5.0)
		{
			PORTC = 0B00001111;
		}
	}
	contSensor++;
	if (contSensor>2) contSensor = 0;
	uint8_t adMuxTemp = ADMUX;
	adMuxTemp &= 0b01000000;
	adMuxTemp|= contSensor;
	ADMUX = adMuxTemp;
	ADCSRA |= (1<<ADSC);	
}


uint8_t cero_en_bit(volatile uint8_t *LUGAR, uint8_t BIT)
{
	return (!(*LUGAR&(1<<BIT)));
}

uint8_t uno_en_bit(volatile uint8_t *LUGAR, uint8_t BIT)
{
	return (*LUGAR&(1<<BIT));
}


void USART_init(uint16_t ubrr){
	UBRRH = (ubrr>>8);
	UBRRL = ubrr;
	UCSRB = (1<<TXEN) | (1<<RXEN) | (1<<RXCIE);
	UCSRC = (1<<URSEL) | (3<<UCSZ0);
}

ISR (USART_RXC_vect)
{
	uint8_t dato = UDR;
	if (dato == 'T')
	{
		USART_Transmit(datoTemp);
	}
	else if (dato == 'H')
	{
		USART_Transmit(datoHum);
	}
	else if (dato =='P')
	{
		USART_Transmit(datoPh);
	}
}

void USART_Transmit(uint8_t dato){
	while (!(UCSRA & (1<<UDRE))){}
	UDR = dato;
}