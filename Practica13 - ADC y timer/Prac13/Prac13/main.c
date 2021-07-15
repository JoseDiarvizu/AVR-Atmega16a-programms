/*
 * Titulo
 *
 * Created: Fecha
 * Author : Nombre
 */ 

#define  F_CPU 1000000UL
#include <avr/io.h>				//Librería general del Microcontrolador
#include <stdint.h>				//Para poder declarar varialbes especiales
#include <util/delay.h>			//Para poder hacer delays
#include <avr/interrupt.h>	 //Para poder manejar interrupciones
#include "lcd.h"
#include <stdio.h>
#include <stdlib.h>

//PROTOTIPADO DE FUNCIONES PARA PODER UTILIZARLAS DESDE CUALQUIER "LUGAR"
//*************************************************************************
uint8_t cero_en_bit(volatile uint8_t *LUGAR, uint8_t BIT);
uint8_t uno_en_bit(volatile uint8_t *LUGAR, uint8_t BIT);
//*************************************************************************

volatile uint8_t conTimer = 0,cincoSeg = 0;
volatile uint8_t segundos = 0, minutos = 0, horas = 0;
char numToString[10];
int main(void)
{
	sei();
	DDRD = 0;
	PORTD = 1;
	//CONFIGURACION ADC
	ADMUX = 0B01000000;
	ADCSRA = 0B10111011;
	SFIOR = 0B01000000;
	
	//CONFIGURACION INTERRUPCIONES
	MCUCR = 0B00000011;
	GIFR = 0B11100000;
	GICR = 0B01000000;
	
	//CONFIGURACIÓN TIMER
	TCCR0 = 0B00001101;
	OCR0 = 23;
	TIMSK = 0B00000010;
	TIFR = 0B00000011;
	
	lcd_init(LCD_DISP_ON);
    while (1) 
    {
		
    }
}


ISR(INT0_vect)
{
}

ISR(ADC_vect)
{
	cincoSeg = 0;
	uint16_t res  = ADC;
	float resDisplay = ((float)res*70)/1023;
	lcd_gotoxy(0,1);
	lcd_puts("                ");
	lcd_gotoxy(4,1);
	dtostrf(resDisplay-20,5,2,numToString);
	lcd_puts(numToString);
	lcd_puts(" ");
	lcd_putc(223);
	lcd_puts("C"); 
}

ISR(TIMER0_COMP_vect)
{
	conTimer++;
	if (conTimer >= 40)
	{
		cincoSeg++;
		if (cincoSeg == 5)
		{
			lcd_gotoxy(0,1);
			lcd_puts("                ");
			cincoSeg  = 0;
		}
		conTimer = 0;
		//segundos++;
		segundos++;
		if (segundos>=60)
		{
			segundos = 0;
			minutos++;
			if (minutos>=60)
			{
				minutos = 0;
				horas++;
				if (horas>23)
				{
					horas = 0;
					minutos = 0;
					segundos = 0;
				}
			}
		}
		
		lcd_gotoxy(4,0);
		if (horas>9)
		{
			sprintf(numToString,"%d",horas);
			lcd_puts(numToString);
			lcd_puts(":");
		}
		else
		{
			lcd_puts("0");
			sprintf(numToString,"%d",horas);
			lcd_puts(numToString);
			lcd_puts(":");
		}
		
		if (minutos>9)
		{
			sprintf(numToString,"%d",minutos);
			lcd_puts(numToString);
			lcd_puts(":");
		}
		else
		{
			lcd_puts("0");
			sprintf(numToString,"%d",minutos);
			lcd_puts(numToString);
			lcd_puts(":");
		}
		
		if (segundos>9)
		{
			sprintf(numToString,"%d",segundos);
			lcd_puts(numToString);
		}
		else
		{
			lcd_puts("0");
			sprintf(numToString,"%d",segundos);
			lcd_puts(numToString);
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