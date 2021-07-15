
#define  F_CPU 8000000UL
#include <avr/io.h>				//Librería general del Microcontrolador
#include <stdint.h>				//Para poder declarar varialbes especiales
#include <util/delay.h>			//Para poder hacer delays
#include "lcd.h"
#include <stdio.h>

//#include <avr/interrupt.h>	 //Para poder manejar interrupciones

//PROTOTIPADO DE FUNCIONES PARA PODER UTILIZARLAS DESDE CUALQUIER "LUGAR"
//*************************************************************************
uint8_t cero_en_bit(volatile uint8_t *LUGAR, uint8_t BIT);
uint8_t uno_en_bit(volatile uint8_t *LUGAR, uint8_t BIT);
void SPI_Mastertransmit(uint8_t cData, uint8_t Device);
void SPI_Masterinit();

//*************************************************************************
volatile uint8_t basura=0;
volatile float voltaje = 0;
char numToString[5];

int main(void)
{
	lcd_init(LCD_DISP_ON);
	SPI_Masterinit();
	PORTC = 255;
	lcd_puts("Prueba");
    while (1) 
    {
		if (cero_en_bit(&PINC,0))
		{
			_delay_ms(60);
			while (cero_en_bit(&PINC,0)){}
			_delay_ms(60);
			
			SPI_Mastertransmit(0,6);
		}
		else if (cero_en_bit(&PINC,2))
		{
			_delay_ms(60);
			while (cero_en_bit(&PINC,2)){}
			_delay_ms(60);
			SPI_Mastertransmit(2,6);
		}
		else if (cero_en_bit(&PINC,4))
		{
			_delay_ms(60);
			while (cero_en_bit(&PINC,4)){}
			_delay_ms(60);
			SPI_Mastertransmit(0,7);
		}
		else if (cero_en_bit(&PINC,6))
		{
			_delay_ms(60);
			while (cero_en_bit(&PINC,6)){}
			_delay_ms(60);
			SPI_Mastertransmit(2,7);
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

void SPI_Mastertransmit(uint8_t cData, uint8_t Device)
{
	PORTA =~(1<<Device);
	SPDR = cData;
	while (cero_en_bit(&SPSR,SPIF)){}
	basura = SPDR;
	_delay_ms(50);
	SPDR = cData;
	
	while (cero_en_bit(&SPSR,SPIF)){}
	basura = SPDR;
	
	voltaje = ((float)basura*5)/255;
	dtostrf(voltaje,3,1,numToString);
	
	lcd_gotoxy(0,1);
	lcd_puts("                ");
	lcd_gotoxy(0,1);
	lcd_puts(numToString);
	PORTA = 255;
}

void SPI_Masterinit()
{
	DDRB |= (1<<5) | (1<<7);
	DDRA = 0b11111111;
	PORTA = 255;
	SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0);
}