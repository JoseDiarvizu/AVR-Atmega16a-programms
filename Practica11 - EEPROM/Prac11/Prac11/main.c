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
#include <string.h>
#define maxLength 4
//PROTOTIPADO DE FUNCIONES PARA PODER UTILIZARLAS DESDE CUALQUIER "LUGAR"
//*************************************************************************
uint8_t cero_en_bit(volatile uint8_t *LUGAR, uint8_t BIT);
uint8_t uno_en_bit(volatile uint8_t *LUGAR, uint8_t BIT);
uint8_t tecladoFuncion(volatile uint8_t *DDRT, volatile uint8_t *PORTT, volatile uint8_t *PINT);
void EEPROM_write(uint16_t uiAddress, uint8_t ucData);
uint8_t EEPROM_read(uint16_t uiAddress);

//*************************************************************************

uint8_t filas = 0, filasend = 3, columnas = 4, columnasend = 7;

//Una vez estén los pines asignados correctamente, entonces el código ya está listo para tomar el tamaño de la matriz correctamente
//NO ES NECESARIO MODIFICAR EL TAMAÑO DE LA MATRIZ, el tamaño se tomará conforme a los pines que asignaste anteriormente.
//Asigna los valores que la matriz debe tomar, si la matriz es más pequeña que 4x4, sólo llena los valores correspondientes al tamaño comenzando de la posición 0,0
volatile uint8_t teclado[4][4] = {
	{13,14,0,15},
	{12,3,2,1},
	{11,6,5,4},
	{10,9,8,7}};




int main(void)
{
	
	
    lcd_init(LCD_DISP_ON);
	lcd_puts("0000");
	uint8_t n1=0;
    while (1) 
    {
		uint8_t lectura = tecladoFuncion(&DDRD, &PORTD, &PIND);
		
		if (lectura <=9)
		{
			
			for (int i = maxLength-1; i>0; i--)
			{
				n1 = EEPROM_read(i-1);
				EEPROM_write(i,n1);
			}
			EEPROM_write(0,lectura);
			lcd_gotoxy(0,0);
			for (int i = 0; i<maxLength; i++)
			{
				n1 = EEPROM_read(i);
				lcd_putc(n1+48);
			}
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

uint8_t tecladoFuncion(volatile uint8_t *DDRT, volatile uint8_t *PORTT, volatile uint8_t *PINT) {
	
	for(uint8_t i=filas; i<=filasend;i++){
		*DDRT |= (1<<i);		// definimos puerto para teclado
	}
	
	while(1) {
		
		*PORTT = 0b11111111;
		
		for (uint8_t fila = filas; fila <= filasend; fila++) {
			
			*PORTT = ~(1<<fila);
			_delay_us(2);
			
			for (uint8_t col = columnas; col <= columnasend; col++) {
				
				if (cero_en_bit(PINT,col)) {
					_delay_ms(100);
					while(cero_en_bit(PINT,col)) {}
					_delay_ms(100);
					return teclado[filasend-fila][columnasend-col];
				}
			}
		}
	}
}

void EEPROM_write(uint16_t uiAddress, uint8_t ucData)
{
	while (EECR & (1<<EEWE)){}
	EEAR = uiAddress;
	EEDR = ucData;
	cli();
	EECR |= (1<<EEMWE);
	EECR |= (1<<EEWE);
	sei();
}

uint8_t EEPROM_read(uint16_t uiAddress)
{
	while (EECR & (1<<EEWE)){}
	EEAR = uiAddress;
	EECR |= (1<<EERE);
	
	return EEDR;
}