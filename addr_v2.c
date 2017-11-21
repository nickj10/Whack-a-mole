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
	uint16_t checksum1;
	uint16_t checksum2;	

	int array[16];
//	for (nivell = 0b0; nivell < 0x1A; nivell++) {
	nivell = 0x00;		
//	for (nivell = 0b0; nivell < 0x02; nivell++) {
	quintaulell = 0b0;

	llenarArray (array, 3);
	
	while (quintaulell < 0x100) {
		checksum1 = 0x10;
		checksum1 += (quintaulell & 0x0F);
		checksum1 += (quintaulell >> 4);
		checksum1 += (nivell & 0x0F);
		checksum1 += (nivell >> 4);

		checksum2 = 0x10;
		checksum2 += (quintaulell & 0x0F);
		checksum2 += (quintaulell >> 4);
		checksum2 += (nivell & 0x0F);
		checksum2 += (nivell >> 4);
		
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
			checksum1 += num1;
			checksum2 += num2;

			fprintf (g, "%.2X", num2);
			fprintf (f, "%.2X", num1);
			i++;
			quintaulell++;
		}
		checksum1 = checksum1 & 0xFF;
		printf ("checksum parcial 1: %.4X\n", checksum1);
		checksum2 = checksum2 & 0xFF;
		printf ("checksum parcial 2: %.4X\n", checksum2);
		
		// Convertir en Ca2
		checksum1 = ~checksum1;
		checksum2 = ~checksum2;
		checksum1 += 1;
		checksum2 += 1;
		
		// Quitar FF delante
		checksum1 &= 0xFF;
		checksum2 &= 0xFF;

		fprintf (f, "%.2X\n", checksum1);
		fprintf (g, "%.2X\n", checksum2);
	}
		
	//}
	/*
	// Indiquem el EOF per acabar
	fprintf (f, ":00000001FF\n");
	fprintf (g, ":00000001FF\n");
	*/
	fclose(f);
	fclose(g);
	return 0;
}
