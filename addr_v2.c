#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

void llenarArray (int array[16], int nivell) {
	int i = 0;
	while (i < 16) {
		if (i < nivell) {
			array[i] = 1;
		}
		else {
			array[i] = 0;
		}
		i++;
	}
	printf ("Array generada: ");
	for (i = 0; i < 16; i++)
		printf ("%d ", array[i]);
	printf ("\n");
}

void shuffle (int array[16], int max) {
	int i, j;
	int temp;

	if (max > 1) {
		for (i = 0; i < max - 1; i++) {
			j = (i + rand()) % 16;
			
			// Intercambio
			temp = array[j];
			array[j] = array[i];
			array[i] = temp;
		}
	}
}

int main (void) {

	srand(time(NULL));

	uint16_t dir;
	uint16_t quintaulell;
	uint8_t nivell;
	uint8_t num1 = 0b0;
	uint8_t num2 = 0b0;
	FILE * f = fopen ("out_high.hex", "w");
	FILE * g = fopen ("out_low.hex", "w");
	int i, k;	

	int array[16];
	//printf ("Hexadecimal generado: %.2X\n", num1);
	//printf ("Hexadecimal generado 2: %.2X\n", num2);
//	for (nivell = 0b0; nivell < 0x1A; nivell++) {
	nivell = 0x00;		
	quintaulell = 0b0;

	llenarArray (array,1);
	/*printf ("Shuffled array: ");	
	for (i = 0; i < 16; i++)
		printf ("%d ", array[i]);
	printf ("\n");
	for (i = 0; i < 8; i++) {
		num1 = num1 << 1;
		num1 |= array[i];
	}
	for (i = 8; i < 16; i++) {
		num2 = num2 << 1;
		num2 |= array[i];
	}
*/
	while (quintaulell < 0x100) {
		dir = nivell << 8;
		dir |= quintaulell;
			
		fprintf (f, ":10%.4X00", dir);
		fprintf (g, ":10%.4X00", dir);
		i = 0;
		while (i < 8) {
			
			shuffle (array, 16);
			for (k = 0; k < 8; k++) {
				num1 = num1 << 1;
				num1 |= array[k];
			}
			for (k = 8; k < 16; k++) {
				num2 = num2 << 1;
				num2 |= array[k];
			}
			fprintf (g, "%.2X", num2);
			fprintf (f, "%.2X", num1);
			i++;
			quintaulell++;
		}
		fprintf (f, "\n");
		fprintf (g, "\n");
	}
		
//	}

}
