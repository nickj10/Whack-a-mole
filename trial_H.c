#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define MAXBITS 8
#define MAXTAULELLS 256

uint8_t convertToHex(char c) {
	if (c >= '0' && c <= '9') {
		return c - '0';
	} else {
		switch (c) {
			case 'A':
				return 0x0A;
			case 'B':
				return 0x0B;
			case 'C':
				return 0x0C;
			case 'D':
				return 0x0D;
			case 'E':
				return 0x0E;
			case 'F':
				return 0x0F;
		}
	}
	return 0x00;
}

uint8_t calculateCheckSum(char num[MAXBITS]) {
	uint8_t ca2 = 0x00;
	int i;

	for (i = 0; i < MAXBITS - 1; i++) {
		ca2 = convertToHex(num[i]);
		ca2 = ca2 << 4;
		ca2 = ca2 | convertToHex(num[i+1]);
	}
	ca2 = ~ca2;
	ca2 = ca2 + 1;

	return ca2;
}

int main () {
	FILE *f_comb = fopen("comb_taulell_H.hex", "w");
	
	srand(time(NULL));
	
	uint8_t nivell;
	uint16_t quintaulell;
	uint16_t dir;
	uint8_t taulell_random[MAXTAULELLS];
	int i, j;

	for (nivell = 0b0; nivell < 0x1A; nivell++) {
		for (i = 0; i < 256; i++) {
			if (nivell >= 0x0F && i == 0) {
				taulell_random[i] = 0xFF;
			}
			else {
				taulell_random[i] = rand() & nivell;
			}
		}
	
		// Inicialitzem
		quintaulell = 0b0;
		dir = nivell << 8;
		dir |= quintaulell;
		j = 0;
		while (j < 32) {
			fprintf (f_comb, ":10%.4X", dir);
			for (i = 0; i < 7; i++) {
				fprintf (f_comb, "%.4X", taulell_random[i]);
			}
			printf ("\n");
			dir += 8;
		}
	}
	
	// Indiquem el EOF per acabar
	fprintf(f_comb, ":00000001FF\n");
	fclose(f_comb);
	return 0;
}

/*********************************************************************************
** 
**				 Cosas importantes /!\
**
**	1.	Taulell es de 16 bits, por tanto, hay que dividirlo en grupos de 8 bits
**		porque el EEPROM guarda words de 8 bits.
**
**	2. 	El campo de datos consiste en los datos de CUARTO direcciones, es decir,
**		cada linea que imprimimos al fichero .hex (output) consiste en 2 tableros
**		o 2 combinaciones
**
**	3.	Temas de la direccion que entrará en la ROM:
**		Dependerá de los siguientes:
**			- Nivell
**			- QuinTaulell (Que combinacion/tablero queremos acceder)
** 		Nos permite tener hasta 4 hexadecimales de direcciones asi que podemos
**		usar de 0000 a FFFF.
**
**	4.	Convertiremos la direccion en binario a hexadecimal usando la funcion
**		convertToHex
**
**	5.	Idea: Podriamos recorrer todos los niveles y dentro de ese bucle, generar
**		los 265 tableros con rand(). Convertir el tablero en hexadecimal, dividir
**		el tablero en PA y PB. Finalmente, hacer un fprintf cuando ya tenemos 2
**		tableros generados
**
**********************************************************************************/
