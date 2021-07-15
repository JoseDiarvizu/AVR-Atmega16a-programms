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
//#include <avr/interrupt.h>	 //Para poder manejar interrupciones

//PROTOTIPADO DE FUNCIONES PARA PODER UTILIZARLAS DESDE CUALQUIER "LUGAR"
//*************************************************************************
uint8_t cero_en_bit(volatile uint8_t *LUGAR, uint8_t BIT);
uint8_t uno_en_bit(volatile uint8_t *LUGAR, uint8_t BIT);
void mostrar(volatile uint8_t cont);
//*************************************************************************


int main(void)
{
	DDRA = 0B01111110;
	PORTA = 0B10000001;
	DDRC = 0B11111111;
	volatile uint8_t cont = 0;
    while (1) 
    {
		if (cero_en_bit(&PINA,0)){
			_delay_ms(60);
			while (cero_en_bit(&PINA,0)){}
			_delay_ms(60);
	
				cont++;
				if(cont == 100) {
					cont--;
				}
				mostrar(cont);
		
		}
		else if (cero_en_bit(&PINA,7))
		{
			_delay_ms(60);
			while (cero_en_bit(&PINA,7)){}
			_delay_ms(60);
			if(cont<=0) cont = 1;
			cont--;
			mostrar(cont);
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

void mostrar(volatile uint8_t cont){
	uint8_t aux1, aux2;
	aux1 = aux2 = cont;
	aux1 = aux1%10;
	aux2 = aux2/10;
	aux2 = aux2%10;
	PORTC = aux2 | (aux1<<4);
	
}