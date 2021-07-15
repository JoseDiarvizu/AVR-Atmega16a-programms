/*
 * Titulo
 *
 * Created: Fecha
 * Author : Nombre
 */ 
 
#define FOSC 1000000
#define BAUD 4800
#define  F_CPU 1000000UL
#define MYUBRR FOSC/16/BAUD-1
#include <avr/io.h>				//Librería general del Microcontrolador
#include <stdint.h>				//Para poder declarar varialbes especiales
#include <util/delay.h>			//Para poder hacer delays
#include <avr/interrupt.h>	 //Para poder manejar interrupciones
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lcd.h"
//PROTOTIPADO DE FUNCIONES PARA PODER UTILIZARLAS DESDE CUALQUIER "LUGAR"
//*************************************************************************
uint8_t cero_en_bit(volatile uint8_t *LUGAR, uint8_t BIT);
uint8_t uno_en_bit(volatile uint8_t *LUGAR, uint8_t BIT);
void USART_init(uint16_t ubrr);
void USART_Transmit(uint8_t dato);
uint8_t tecladoFuncion(volatile uint8_t *DDRT, volatile uint8_t *PORTT, volatile uint8_t *PINT);

//*************************************************************************
volatile uint8_t teclado[4][4] = {
	{15,9,8,7},
	{14,6,5,4},
	{13,3,2,1},
	{12,11,0,10}};
		
volatile uint8_t instruccion =0;

int main(void)
{
	lcd_init(LCD_DISP_ON);
	USART_init(MYUBRR);
	sei();
	DDRD = 0b00000010;
	lcd_puts("Simon dice...");
	while (1)
	{
		uint8_t lectura = tecladoFuncion(&DDRA, &PORTA, &PINA);	
		if (lectura == 1)
		{
			lcd_gotoxy(0,1);
			lcd_puts("                ");
			lcd_gotoxy(0,1);
			lcd_puts("De pie");
			USART_Transmit(lectura);
			instruccion = lectura;
		}
		else if (lectura == 2)
		{
			lcd_gotoxy(0,1);
			lcd_puts("                ");
			lcd_gotoxy(0,1);
			lcd_puts("Sentado");
			USART_Transmit(lectura);
			instruccion = lectura;
		}
		else if (lectura == 3)
		{
			lcd_gotoxy(0,1);
			lcd_puts("                ");
			lcd_gotoxy(0,1);
			lcd_puts("Agachado");
			USART_Transmit(lectura);
			instruccion = lectura;
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


void USART_init(uint16_t ubrr){
	UBRRH = (ubrr>>8);
	UBRRL = ubrr;
	UCSRB = (1<<TXEN) | (1<<RXEN) | (1<<RXCIE);
	UCSRC = (1<<URSEL) | (3<<UCSZ0);
}

ISR (USART_RXC_vect)
{
	uint8_t dato = UDR;
	dato = dato - 48;
	if (dato == instruccion)
	{
		lcd_clrscr();
		lcd_puts("CORRECTO!");
		
		_delay_ms(2500);
		lcd_clrscr();
		lcd_puts("Simon dice...");
	}
	else{
		lcd_clrscr();
		lcd_puts("INCORRECTO!  ");
		
		_delay_ms(2500);
		lcd_clrscr();
		lcd_puts("Simon dice...");
	}
	
}

void USART_Transmit(uint8_t dato){
	while (!(UCSRA & (1<<UDRE))){}
	UDR = dato;
	
}

uint8_t tecladoFuncion(volatile uint8_t *DDRT, volatile uint8_t *PORTT, volatile uint8_t *PINT) {
	//La funcion tecladoFuncion se quedar? trabada hasta el momento en que se presione una tecla
	//Esta funci?n regresa un valor del tipo uint8_t que tendr? que ser asignada a una variable
	//Ejemplo:

	//uint8_t tecla;
	//tecla = tecladoFuncion(&DDRA,&PORTA,&PINA);

	*DDRT = (1<<7)|(1<<6)|(1<<5)|(1<<4);		// definimos puerto para teclado
	
	while(1) {
		
		*PORTT = 0b11111111;
		
		for (uint8_t fila = 7; fila > 3; fila--) {
			
			*PORTT = ~(1<<fila);
			_delay_us(2);
			
			for (uint8_t col = 0; col < 4; col++) {
				
				if (cero_en_bit(&*PINT,col)) {
					_delay_ms(100);
					while(cero_en_bit(&*PINT,col)) {}
					_delay_ms(100);
					return teclado[7-fila][col];
				}
			}
		}
	}
}