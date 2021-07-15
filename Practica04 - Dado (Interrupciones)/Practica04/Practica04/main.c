/*
 * Titulo
 *
 * Created: Fecha
 * Author : Nombre
 */ 

#define  F_CPU 1000000UL
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>		/* time */
#include <avr/io.h>				//Librería general del Microcontrolador
#include <stdint.h>				//Para poder declarar varialbes especiales
#include <util/delay.h>			//Para poder hacer delays
#include <avr/interrupt.h>

//#include <avr/interrupt.h>	 //Para poder manejar interrupciones

//PROTOTIPADO DE FUNCIONES PARA PODER UTILIZARLAS DESDE CUALQUIER "LUGAR"
//*************************************************************************
uint8_t cero_en_bit(volatile uint8_t *LUGAR, uint8_t BIT);
uint8_t uno_en_bit(volatile uint8_t *LUGAR, uint8_t BIT);
//*************************************************************************

volatile uint8_t randomNumber = 0;

 
int main(void)
{
    sei();
	MCUCR = 0B00000011;
	GIFR = 0B11100000;
	GICR = 0B01000000;
	DDRA = 0B11111111;
	DDRD = 0B00000000;
	srand (time(NULL));
	
    while (1) 
    {
		
    }
}


ISR(INT0_vect)
{
	_delay_ms(60);
	while (cero_en_bit(&PIND,2)){}
	_delay_ms(60);	
	
	randomNumber = rand()%6 + 1;
	
	if (randomNumber == 1)
	{
		PORTA = 0B00001000;
	}
	else if (randomNumber ==2)
	{
		PORTA = 0B01000001;
	}
	else if (randomNumber ==3)
	{
		PORTA = 0B01001001;
	}
	else if (randomNumber ==4)
	{
		PORTA =0B01010101;
	}
	else if (randomNumber ==5)
	{
		PORTA = 0B01011101;
	}
	else if (randomNumber ==6)
	{
		PORTA = 0B01110111;
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