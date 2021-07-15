/*
 * Titulo
 *
 * Created: Fecha
 * Author : Nombre
 */ 

#define  F_CPU 8000000UL
#include <avr/io.h>				//Librería general del Microcontrolador
#include <stdint.h>				//Para poder declarar varialbes especiales
#include <util/delay.h>			//Para poder hacer delays
#include <avr/interrupt.h>	 //Para poder manejar interrupciones

//PROTOTIPADO DE FUNCIONES PARA PODER UTILIZARLAS DESDE CUALQUIER "LUGAR"
//*************************************************************************
uint8_t cero_en_bit(volatile uint8_t *LUGAR, uint8_t BIT);
uint8_t uno_en_bit(volatile uint8_t *LUGAR, uint8_t BIT);
void SPI_Slaveinit(void);
uint8_t SPI_Slavereceive(void);

//*************************************************************************

volatile uint8_t dato = 0;
int main(void)
{
	
	SPI_Slaveinit();
    DDRC = 0b11111111;
	sei();
    while (1) 
    {
    }
}

void SPI_Slaveinit(void)
{
	DDRB |= (1<<6);
	SPCR = (1<<SPIE) | (1<<SPE) | (1<<SPR0);
	SPSR |= (1<<SPIF);
}

ISR(SPI_STC_vect)
{
	uint8_t dato = SPDR;
	PORTC = dato;
	SPSR |= (1<<SPIF);
	_delay_ms(2000);
	PORTC = 0;
}

uint8_t SPI_Slavereceive(void)
{
	while (cero_en_bit(&SPSR,SPIF)){}
	dato = SPDR;
	return dato;
}
uint8_t cero_en_bit(volatile uint8_t *LUGAR, uint8_t BIT)
{
	return (!(*LUGAR&(1<<BIT)));
}

uint8_t uno_en_bit(volatile uint8_t *LUGAR, uint8_t BIT)
{
	return (*LUGAR&(1<<BIT));
}