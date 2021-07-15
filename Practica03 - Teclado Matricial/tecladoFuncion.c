
volatile uint8_t teclado[4][4] = {{1,2,3,10},
                                  {4,5,6,11},
								  {7,8,9,12},
								  {14,0,15,13}};					  
uint8_t tecladoFuncion(volatile uint8_t *DDRT, volatile uint8_t *PORTT, volatile uint8_t *PINT);


uint8_t tecladoFuncion(volatile uint8_t *DDRT, volatile uint8_t *PORTT, volatile uint8_t *PINT) {
//La funcion tecladoFuncion se quedará trabada hasta el momento en que se presione una tecla
//Esta función regresa un valor del tipo uint8_t que tendrá que ser asignada a una variable
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
				
				if (cero_en_pin(PINT,col)) {
					_delay_ms(100);
					while(cero_en_pin(PINT,col)) {}
					_delay_ms(100);
					return teclado[7-fila][col];
				}
			}
		}
	}
}