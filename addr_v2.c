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
	FILE * f = fopen ("out_high3.hex", "w");
	FILE * g = fopen ("out_low3.hex", "w");
	int i, k;
	uint16_t checksum1;
	uint16_t checksum2;	
	int level;
	int table;
	int rand_num;
	int rand_table;
	int max_num, min_num;	
	int sub;
	int array[16];

	nivell = 0x00;
	//nivell = 0x06;
	//nivell = 0x0F;		
	quintaulell = 0b0;
	level = 1;
	//level = 7;
	//level = 16;
	table = 0;
	rand_table = 1;
	while (level <= 26) {
	
	// reinicialitzar quintaulell
	quintaulell = 0b0;
	if (level >= 16) {
		table = 0;
		rand_table = rand() % 257; // un taulell random de los 256 que tiene todos los LEDs encendidos	
	}
	while (quintaulell < 0x100) {
		if (table == rand_table && level >= 16) {
			// Todos los LEDs encendidos
			llenarArray (array, 16);
		}
		else {
			if (level >= 7 && level < 16) {
				sub = 3;
			}
			else if (level >= 16) {
				sub = 15;
			}
			if (level < 7) { // PARA NIVELES 1 -6
				llenarArray (array, level);
			}
			else {
				// Genera un array con [level-3,level] LEDs encendidos
				max_num = level;
				min_num = level - sub;
				rand_num = rand() % (max_num + 1 - min_num) + min_num;

				// Llenamos el array con 1s 
				llenarArray (array, rand_num);
			}
		}
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
			// ordenar el array aleatoriamente
			shuffle (array, 16);
			for (k = 0; k < 8; k++) {
				num1 = num1 << 1;
				num1 |= array[k];
			}
			
			// cálculo del checksum:
			// sumarlo todo
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
			table++;
		}
		
		
		checksum1 = checksum1 & 0xFF;
		//printf ("checksum parcial 1: %.4X\n", checksum1);
		checksum2 = checksum2 & 0xFF;
		//printf ("checksum parcial 2: %.4X\n", checksum2);
		
		// Convertir en Ca2
		checksum1 = ~checksum1;
		checksum2 = ~checksum2;
		checksum1 += 1;
		checksum2 += 1;
		
		// Quitar FF delante
		checksum1 &= 0xFF;
		checksum2 &= 0xFF;
		
		// Imprimir el checksum
		fprintf (f, "%.2X\n", checksum1);
		fprintf (g, "%.2X\n", checksum2);

	} // end quintaulell while
	level++;
	nivell++;
	} // end level while
	
	// Indiquem el EOF per acabar
	fprintf (f, ":00000001FF\n");
	fprintf (g, ":00000001FF\n");
	
	fclose(f);
	fclose(g);
	return 0;
}
