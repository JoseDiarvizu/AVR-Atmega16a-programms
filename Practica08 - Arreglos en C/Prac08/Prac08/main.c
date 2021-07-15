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
#include "lcd.h"
#define maxCol 50
#include <string.h>
#include <stdio.h>
//PROTOTIPADO DE FUNCIONES PARA PODER UTILIZARLAS DESDE CUALQUIER "LUGAR"
//*************************************************************************
uint8_t cero_en_bit(volatile uint8_t *LUGAR, uint8_t BIT);
uint8_t uno_en_bit(volatile uint8_t *LUGAR, uint8_t BIT);
uint8_t tecladoFuncion(volatile uint8_t *DDRT, volatile uint8_t *PORTT, volatile uint8_t *PINT);
//*************************************************************************

uint8_t filas = 0, filasend = 3, columnas = 4, columnasend = 7;

volatile uint8_t teclado[4][4] = {	
{7,4,1,15},
{8,5,2,0},
{9,6,3,14},
{10,11,12,13}};
int main(void)
{
	
	char numeroToString[10];
	 DDRB = 0B11111111;
	
	uint8_t intentos = 0;
	uint8_t auxLectura = 0;
    lcd_init(LCD_DISP_ON);
	lcd_puts("Cuantos cuadros?");
    while (1) 
    {
		PORTB = 0;
		lcd_clrscr();
		lcd_puts("Cuantos cuadros?");
		int valorX [maxCol];
		int valorY [maxCol];
		memset(valorX,-1,sizeof(valorX));
		memset(valorY,-1,sizeof(valorY));
		uint8_t lectura = tecladoFuncion(&DDRA, &PORTA, &PINA);
		if (lectura <=9)
		{
			auxLectura = lectura;
			for (int i = 0; i<lectura;i++)
			{
				valorX[i] = rand() % 10 + 0;
				valorY[i] = rand() % 2 + 0;
			}
			intentos = lectura * 2;
			lcd_clrscr();
			lcd_puts("Escondere ");
			lcd_putc(lectura + 48);
			lcd_gotoxy(0,1);
			lcd_puts("cuadritos.");
			_delay_ms(2000);
			lcd_clrscr();
			lcd_gotoxy(0,0);
			lcd_puts("Tu debes buscar");
			lcd_gotoxy(0,1);
			lcd_putc(lectura+48);
			lcd_puts(" cuadritos.");
			_delay_ms(2000);
			lcd_clrscr();
			lcd_gotoxy(0,0);
			lcd_puts("2 filas (0,1)");
			lcd_gotoxy(0,1);
			lcd_puts("10 cols (0-9)");
			_delay_ms(2000);
			lcd_clrscr();
			lcd_gotoxy(0,0);
			lcd_puts("Presiona +");
			lcd_gotoxy(0,1);
			lcd_puts("para continuar..");
			
			lectura = tecladoFuncion(&DDRA, &PORTA, &PINA);
			while(lectura!=13) lectura = tecladoFuncion(&DDRA, &PORTA, &PINA);
			lcd_clrscr();
			lcd_gotoxy(0,0);
			lcd_puts("Trata de");
			lcd_gotoxy(0,1);
			lcd_puts("memorizar...");
			_delay_ms(2000);
			lcd_clrscr();
			
			for (int i = 0;i<auxLectura;i++)
			{
				lcd_clrscr();
				lcd_gotoxy(0,0);
				lcd_puts("Cuadrito ");
				lcd_putc((i+1)+48);
				lcd_gotoxy(0,1);
				lcd_putc(valorY[i]+48);
				lcd_puts(",");
				lcd_putc(valorX[i]+48);
				_delay_ms(1000);
			}
			lcd_clrscr();
			lcd_gotoxy(0,0);
			lcd_puts("Tienes ");
			sprintf(numeroToString,"%d",intentos);
			lcd_puts(numeroToString);
			lcd_gotoxy(0,1);
			lcd_puts("intentos");
			_delay_ms(2000);
			uint8_t aciertos = 0;
			
			for (int i = 0;i<intentos;i++)
			{
				if(aciertos < auxLectura)
				{
					
					lcd_clrscr();
					lcd_puts("Intento ");
					sprintf(numeroToString,"%d",i+1);
					lcd_puts(numeroToString);
				
					uint8_t buscarY= tecladoFuncion(&DDRA, &PORTA, &PINA);
					lcd_puts(" (");
					lcd_putc(buscarY+48);
					lcd_puts(",");
				
					uint8_t buscarX = tecladoFuncion(&DDRA, &PORTA, &PINA);
					lcd_putc(buscarX+48);
					lcd_puts(")");
					
					if (buscarY>1)
					{
						lcd_gotoxy(0,1);
						lcd_puts("No existe lugar");
						_delay_ms(2000);
					}
					else
					{
						int encontrado = 0;
						for (int j = 0;j<auxLectura;j++)
						{
			
							if (buscarX == valorX[j] && buscarY == valorY[j])
							{
								if (valorX[j+20] == -1)
								{
									lcd_gotoxy(0,1);
									lcd_puts("Acierto");
									valorX[j+20] = 0;
									aciertos++;
									PORTB = aciertos;
									encontrado = 1;
									_delay_ms(1000);
									break;
									
								}
								else
								{
									lcd_gotoxy(0,1);
									lcd_puts("Ya estaba!");
									encontrado = 1;
									_delay_ms(1000);
									
								}
							}
							
						}
						if (encontrado ==0)
						{
							lcd_gotoxy(0,1);
							lcd_puts("Error");
							_delay_ms(1000);
						}
					}
				
				}
				else
				{
					lcd_clrscr();
					lcd_puts("Tienes excelente");
					lcd_gotoxy(0,1);
					lcd_puts("memoria !!!");
					_delay_ms(2000);
					lcd_clrscr();
					lcd_puts("Ganaste!!!");
					lcd_gotoxy(0,1);
					lcd_puts("Felicidades!");
					_delay_ms(1000);
					lcd_clrscr();
					break;
					
				}
			}
			
		if (auxLectura == 0)
		{
			lcd_clrscr();
			lcd_puts("Tienes excelente");
			lcd_gotoxy(0,1);
			lcd_puts("memoria !!!");
			_delay_ms(1000);
			lcd_clrscr();
			lcd_puts("Ganaste!!!");
			lcd_gotoxy(0,1);
			lcd_puts("Felicidades!");
			_delay_ms(1000);
			lcd_clrscr();
		}
		else if (aciertos<auxLectura)
		{
			lcd_clrscr();
			lcd_puts("Tu memoria no es");
			lcd_gotoxy(0,1);
			lcd_puts("tan buena =(");
			_delay_ms(2000);
			lcd_clrscr();
			lcd_puts("Perdiste!!!");
			lcd_gotoxy(0,1);
			lcd_puts("Intenta de nuevo");
			_delay_ms(2000);
			lcd_clrscr();
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
