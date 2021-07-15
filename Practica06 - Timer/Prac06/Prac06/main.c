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
#include "lcd.h"
#include <avr/interrupt.h>	 //Para poder manejar interrupciones

//PROTOTIPADO DE FUNCIONES PARA PODER UTILIZARLAS DESDE CUALQUIER "LUGAR"
//*************************************************************************
uint8_t cero_en_bit(volatile uint8_t *LUGAR, uint8_t BIT);
uint8_t uno_en_bit(volatile uint8_t *LUGAR, uint8_t BIT);


//*************************************************************************

volatile uint8_t posX = 0;
volatile uint8_t contTimer = 0;
volatile char letra =65 ;

int main(void)
{
	lcd_init(LCD_DISP_ON);
	///CONFIGURACIÓN TIMER
	TCCR0 = 0B00001000;
	TIMSK = 0B00000010;
	TIFR = 0B00000011;
	OCR0 = 243;
	//
	
	//CONFIUGRACIÓN PUERTOS
	DDRA = 0B11111110;
	PORTA = 0B00000001;
	
	//
	sei();
	
	
	
    while (1) 
    {
		if (cero_en_bit(&PINA,0))
		{
			_delay_ms(60);
			while(cero_en_bit(&PINA,0)){}
			_delay_ms(60);
			if (posX >15)
			{
				posX = 0;
				lcd_gotoxy(posX,0);
				contTimer = 0;
				lcd_clrscr();
				letra = 65;
				TCCR0 = 0B00001000;
				
			}else{
				lcd_gotoxy(posX,0);
				lcd_putc(letra);
				TCCR0 = 0B00001101;
				contTimer = 0;
				letra++;
				if(letra>90) letra = 65;
			}
		}
		
    }
}

ISR (TIMER0_COMP_vect)
{
	contTimer++;
	if (contTimer >=4)
	{
		posX++;
		letra = 65;
		contTimer = 0;
		TCCR0 = 0B00001000;
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
