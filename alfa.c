#include <stdio.h>
#include <stdint.h>

uint8_t to_hex(char c) {
	if (c >= '0' && c <= '9') {
		return c - '0'; //1, 2, 3...
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
}

int main(void) {
	printf("Introduce un numero: ");
	char buff[5]; //3A
	scanf("%s", buff); //0011 1010

	uint8_t ca2 = 0x00;
	//[0] -> 3, [1] -> [A]
	ca2 = to_hex(buff[0]); //0x03
	ca2 = ca2 << 4; //0x30
	ca2 = ca2 | to_hex(buff[1]);

	ca2 = ~ca2;
	ca2 = ca2 + 1;

	printf("El CA2 es %X\n", ca2);
	return 0;
}









