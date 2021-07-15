/*
 * Titulo
 *
 * Created: Fecha
 * Author : Nombre
 */ 

#define  F_CPU 4000000UL
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
//*************************************************************************


int main(void)
{
	DDRB = 0B11111111;
	DDRC = 0B11111111;
	
	ADMUX = 0b01000000;
	ADCSRA = 0B10010101;
	
		while (1)
		{
			ADMUX = 0b01000000;
			ADCSRA |= (1<<ADSC);
			while (uno_en_bit(&ADCSRA, ADSC)){}
			uint16_t res  = ADC;
			volatile uint8_t currentVal = 0;
			float resDisplay = ((float)res*5)/1023;
			
			currentVal = PINB & 0B11110000;
			if (resDisplay>= 0.0 && resDisplay<1.0)
			{
				PORTB = currentVal | 0B00000000;
			}
			else if (resDisplay>= 1.0 && resDisplay< 2.0)
			{
			
				PORTB = currentVal | 0B00000001;
			}
			else if (resDisplay>= 2.0 && resDisplay< 3.0)
			{
				PORTB = currentVal | 0B00000011;
			}
			else if (resDisplay>= 3.0 && resDisplay< 4.0)
			{
				PORTB = currentVal | 0B00000111;
			}
			else if (resDisplay>= 4.0 && resDisplay<= 5.0)
			{
				PORTB = currentVal | 0B00001111;
			}
			
			ADMUX = 0b01000001;
			ADCSRA |= (1<<ADSC);
			while (uno_en_bit(&ADCSRA, ADSC)){}
			res  = ADC;
			currentVal = 0;
			resDisplay = ((float)res*5)/1023;
			
			currentVal = PINB & 0B00001111;
			if (resDisplay>= 0.0 && resDisplay<1.0)
			{
				PORTB = currentVal | 0B00000000;
			}
			else if (resDisplay>= 1.0 && resDisplay< 2.0)
			{
				
				PORTB = currentVal | 0B00010000;
			}
			else if (resDisplay>= 2.0 && resDisplay< 3.0)
			{
				PORTB = currentVal | 0B00110000;
			}
			else if (resDisplay>= 3.0 && resDisplay< 4.0)
			{
				PORTB = currentVal | 0B01110000;
			}
			else if (resDisplay>= 4.0 && resDisplay<= 5.0)
			{
				PORTB = currentVal | 0B11110000;
			}
			
			
			ADMUX = 0b01000010;
			ADCSRA |= (1<<ADSC);
			while (uno_en_bit(&ADCSRA, ADSC)){}
			res  = ADC;
			currentVal = 0;
			resDisplay = ((float)res*5)/1023;
			
			currentVal = PINC & 0B11110000;
			if (resDisplay>= 0.0 && resDisplay<1.0)
			{
				PORTC = currentVal | 0B00000000;
			}
			else if (resDisplay>= 1.0 && resDisplay< 2.0)
			{
				
				PORTC = currentVal | 0B00000001;
			}
			else if (resDisplay>= 2.0 && resDisplay< 3.0)
			{
				PORTC = currentVal | 0B00000011;
			}
			else if (resDisplay>= 3.0 && resDisplay< 4.0)
			{
				PORTC = currentVal | 0B00000111;
			}
			else if (resDisplay>= 4.0 && resDisplay<= 5.0)
			{
				PORTC = currentVal | 0B00001111;
			}
			
			
		}
}


uint8_t cero_en_bit(volatile uint8_t *LUGAR, uint8_t BIT)
{
	return (!(*LUGAR&(1<<BIT)));
}

uint8_t uno_en_bit(volatile uint8_t *LUGAR, uint8_t BIT)
{
	return (*LUGAR&(1<<BIT));
}


