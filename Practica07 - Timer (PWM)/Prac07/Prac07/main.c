/*
 * Titulo
 *
 * Created: Fecha
 * Author : Nombre
 */ 

#define  F_CPU 4000000UL
#include <avr/io.h>				//Librería general del Microcontrolador
#include <stdint.h>				//Para poder declarar varialbes especiales
#include <util/delay.h>			//Para poder hacer delays
#include "lcd.h"
//#include <avr/interrupt.h>	 //Para poder manejar interrupciones
#define MAX_STRING_SIZE 7
//PROTOTIPADO DE FUNCIONES PARA PODER UTILIZARLAS DESDE CUALQUIER "LUGAR"
//*************************************************************************
uint8_t cero_en_bit(volatile uint8_t *LUGAR, uint8_t BIT);
uint8_t uno_en_bit(volatile uint8_t *LUGAR, uint8_t BIT);
uint8_t tecladoFuncion(volatile uint8_t *DDRT, volatile uint8_t *PORTT, volatile uint8_t *PINT);

//*************************************************************************

volatile uint8_t teclado[4][4] =
{{10,9,8,7},
{11,6,5,4},
{12,3,3,1},
{13,14,0,15}};

uint8_t filas = 0, filasend = 3, columnas = 4, columnasend = 7;

int main(void)
{
	//CONFIGURACION TIMER
	TCCR0 = 0B01101100;
	
	//
	DDRB = 0B11111111;
	
	lcd_init(LCD_DISP_ON);
	lcd_puts("Select position:");
	lcd_gotoxy(0,1);
	
	uint8_t arr[18];
	for (int i = 14; i<32;i++)
		arr[i-14] = i;
	
	 char cadena[][MAX_STRING_SIZE]={"0.00","4.32","15.80","27.40","38.90","50.40","61.90","73.40","85.00","96.50","108.00","120.00","131.00","143.00","154.00","166.00","177.00","180.00"};
	

	volatile uint8_t iterador = 0;	
	lcd_puts(cadena[iterador]);
	lcd_puts(" degrees");
    while (1) 
    {
			volatile uint8_t lectura = tecladoFuncion(&DDRA, &PORTA, &PINA);
			if (lectura == 9)
			{
				
				iterador++;
				if (iterador<=17 && iterador>=0)
				{
					lcd_gotoxy(0,1);
					lcd_puts("             ");
					lcd_gotoxy(0,1);
					lcd_puts(cadena[iterador]);
					lcd_puts(" degrees");
				}
				else{
					iterador = 17;
				}
			}
			else if (lectura ==1)
			{
				
				iterador--;
				if (iterador>=0 && iterador <=17)
				{
					lcd_gotoxy(0,1);
					lcd_puts("                ");
					lcd_gotoxy(0,1);
					lcd_puts(cadena[iterador]);	
					lcd_puts(" degrees");		
				}
				else{
					iterador = 0;
				}
			}
			else if (lectura ==13)
			{
				OCR0 = arr[iterador];
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