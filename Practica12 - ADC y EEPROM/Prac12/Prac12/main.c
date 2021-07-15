
#define  F_CPU 1000000UL
#include <avr/io.h>				//Librería general del Microcontrolador
#include <stdint.h>				//Para poder declarar varialbes especiales
#include <util/delay.h>			//Para poder hacer delays
#include <avr/interrupt.h>	 //Para poder manejar interrupciones
#include <stdio.h>
#include "lcd.h"
//PROTOTIPADO DE FUNCIONES PARA PODER UTILIZARLAS DESDE CUALQUIER "LUGAR"
//*************************************************************************
uint8_t cero_en_bit(volatile uint8_t *LUGAR, uint8_t BIT);
uint8_t uno_en_bit(volatile uint8_t *LUGAR, uint8_t BIT);
uint8_t tecladoFuncion(volatile uint8_t *DDRT, volatile uint8_t *PORTT, volatile uint8_t *PINT);
void EEPROM_write(uint16_t uiAddress, uint8_t ucData);
uint8_t EEPROM_read(uint16_t uiAddress);

uint8_t filas = 0, filasend = 3, columnas = 4, columnasend = 7;

volatile uint8_t teclado[4][4] = {
	{13,14,0,15},
	{12,3,2,1},
	{11,6,5,4},
	{10,9,8,7}};

volatile int contEP = 0;
char numToString[15];

int main(void)
{
	for (int i = 0; i<512; i++)
	EEPROM_write(i,0);
	sei();
	//CONFIGUTACION ADC
	ADMUX = 0b01000000;
	ADCSRA = 0B10111011;
	SFIOR = 0B01100000;
	
	
	//CONFIGURACION TIMER
	TCCR0 = 0b00001101;
	OCR0 = 243;
	TIMSK = 0B00000010;
	TIFR = 0B00000011;
	
	lcd_init(LCD_DISP_ON);
	lcd_puts("Sensando...");
	int sensingFlag = 0;
	int contValores = 0;
	
	while (1)
	{
		uint8_t lectura = tecladoFuncion(&DDRD, &PORTD, &PIND);
		if (lectura == 13)
		{
			if (sensingFlag)
			{
				for (int i = 0; i<512; i++)
					EEPROM_write(i,0);
				contEP = 0;
				sensingFlag = 0;
				lcd_clrscr();
				lcd_puts("Sensando...");
				TCCR0 = 0b00001101;
				
			}
			else
			{
				contEP--;
				TCCR0 = 0b00001000;
				lcd_clrscr();
				lcd_puts("Ultimos val.");
				lcd_gotoxy(0,1);
				sprintf(numToString,"%d",contEP);
				lcd_puts(numToString);
				lcd_puts("   ");
				sprintf(numToString,"%d",EEPROM_read(contEP));
				lcd_puts(numToString);
				sensingFlag = 1;
				contValores = contEP;
			}
		}
		else if (lectura == 15 && sensingFlag == 1)
		{
			
			contValores--;
			if (contValores>=0)
			{
				lcd_gotoxy(0,1);
				lcd_puts("                ");
				lcd_gotoxy(0,1);
				sprintf(numToString,"%d",contValores);
				lcd_puts(numToString);
				lcd_puts("   ");
				sprintf(numToString,"%d",EEPROM_read(contValores));
				lcd_puts(numToString);
			}
			else
			{
				contValores = 0;
			}
		}
		
		else if (lectura == 14 && sensingFlag == 1)
		{
			
			contValores++;
			if (contValores<=contEP)
			{
				lcd_gotoxy(0,1);
				lcd_puts("                ");
				lcd_gotoxy(0,1);
				sprintf(numToString,"%d",contValores);
				lcd_puts(numToString);
				lcd_puts("   ");
				sprintf(numToString,"%d",EEPROM_read(contValores));
				lcd_puts(numToString);
			}
			else
			{
				contValores = contEP;
			}
		}
		
	}
}

ISR(TIMER0_COMP_vect)
{
}

ISR(ADC_vect)
{
	uint16_t res  = ADC;
	float resDisplay = ((float)res*255)/1023;
	int floatCeil =(int) resDisplay;
	uint8_t EPdat = (uint8_t) floatCeil;
	
	EEPROM_write(contEP,EPdat);
	contEP++;
	if (contEP>511)
	{
		TCCR0 = 0b00001000;
		lcd_clrscr();
		lcd_puts("EEPROM llena");
	}
	TIFR = 0B00000011;
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