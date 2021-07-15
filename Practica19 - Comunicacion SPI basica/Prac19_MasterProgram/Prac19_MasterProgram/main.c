

#define  F_CPU 8000000UL
#include <avr/io.h>				
#include <stdint.h>				
#include <util/delay.h>			
	 

uint8_t cero_en_bit(volatile uint8_t *LUGAR, uint8_t BIT);
uint8_t uno_en_bit(volatile uint8_t *LUGAR, uint8_t BIT);
uint8_t SPI_Mastertransmit(uint8_t cData, uint8_t Device);
void SPI_Masterinit();
volatile uint8_t basura=0;

int main(void)
{
	SPI_Masterinit();
	PORTC = 255;
    while (1) 
    {
		if (cero_en_bit(&PINC,0))
		{
			_delay_ms(60);
			while (cero_en_bit(&PINC,0)){}
			_delay_ms(60);
			
			basura = SPI_Mastertransmit(0b11111110,0);
			_delay_ms(100);
		}
		if (cero_en_bit(&PINC,1))
		{
			_delay_ms(60);
			while (cero_en_bit(&PINC,1)){}
			_delay_ms(60);
			
			basura = SPI_Mastertransmit(0b11110000,1);
			_delay_ms(100);
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

uint8_t SPI_Mastertransmit(uint8_t cData, uint8_t Device)
{
	PORTA =~(1<<Device);
	SPDR = cData;
	while (cero_en_bit(&SPSR,SPIF)){}
	PORTA = 255;
	return SPDR;
}

void SPI_Masterinit()
{
	DDRB |= (1<<5) | (1<<7);
	DDRA = 0b11111111;
	PORTA = 255;
	SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0);
}