/*
 * Titulo
 *
 * Created: Fecha
 * Author : Nombre
 */ 
#define FOSC 1000000
#define BAUD 4800
#define  F_CPU 1000000UL
#define MYUBRR F_CPU/16/BAUD-1
#include <avr/io.h>				//Librería general del Microcontrolador
#include <stdint.h>				//Para poder declarar varialbes especiales
#include <util/delay.h>			//Para poder hacer delays
#include <avr/interrupt.h>	 //Para poder manejar interrupciones
#include "lcd.h"
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

char numToString[10];

int main(void)
{
	lcd_init(LCD_DISP_ON);
	lcd_puts("Voltaje:");
	sei();
	ADMUX = 0b01000010;
	ADCSRA = 0B10011011;
	ADCSRA |= (1<<ADSC);
	PORTD = 0b00000010;
	UCSRB = 0B00001000;
	USART_init(MYUBRR);
	while (1)
	{
	}
}

ISR(ADC_vect)
{
	uint16_t res  = ADC;
	float resDisplay = ((float)res*255)/1023;
	float resDisplay2 = ((float)res*5)/1023;
	int floatCeil =(int) resDisplay;
	uint8_t conversion = (uint8_t) floatCeil;
	USART_Transmit(conversion);
	dtostrf(resDisplay2,3,1,numToString);
	lcd_gotoxy(0,1);
	lcd_puts(numToString);
	_delay_ms(250);
	
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
	UCSRB = (1<<TXEN);
	UCSRC = (1<<URSEL) | (3<<UCSZ0);
}

ISR (USART_RXC_vect)
{
	//dato = UDR; puedo sacar por el puerto un valor
}

void USART_Transmit(uint8_t dato){
	while (!(UCSRA & (1<<UDRE))){}
	UDR = dato;
}