/*
 * Titulo
 *
 * Created: Fecha
 * Author : Nombre
 */ 

#define  F_CPU 1000000UL
#include <avr/io.h>				//Librería general del Microcontrolador
#include <stdint.h>				//Para poder declarar varialbes especiales
#include <util/delay.h>
		//Para poder hacer delays
//#include <avr/interrupt.h>	 //Para poder manejar interrupciones

//PROTOTIPADO DE FUNCIONES PARA PODER UTILIZARLAS DESDE CUALQUIER "LUGAR"
//*************************************************************************
uint8_t cero_en_bit(volatile uint8_t *LUGAR, uint8_t BIT);
uint8_t uno_en_bit(volatile uint8_t *LUGAR, uint8_t BIT);
//*************************************************************************


int main(void)
{
    /* Codigo principal */
	DDRA = 0B00000000;
	PORTA = 0B11110000;
	DDRC = 0B01111111;
	
	
    while (1) 
    {
		if (PINA == 0)
		{
			PORTC = 0b00111111;
		}
		else if (PINA==0x10){
			PORTC =0B00000110;
		}
		else if (PINA == 0X20){
			PORTC =0B01011011;
		}
		else if (PINA == 0X30){
			PORTC =0B01001111;
		}
		else if (PINA == 0X40){
			PORTC =0B01100110;
		}
		else if (PINA == 0X50){
			PORTC =0B01101101;
		}
		else if (PINA == 0X60){
			PORTC =0B01111101;
		}
		else if (PINA == 0X70){
			PORTC =0B00000111;
		}
		else if (PINA == 0b10000000){
			PORTC =0b01111111;
		}
		else if (PINA == 0X90){
			PORTC =0B01100111;
		}
		else{
			PORTC =0;
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