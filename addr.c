#include <stdio.h>
#include <stdint.h>

int main(void) {
	uint16_t dir;
	FILE* f = fopen("out.hex", "w");
	//0000 -> 07FF
	for (dir = 0b0; dir < 0x7FFF; dir += 16) {
		fprintf(f, ":10%.4X00YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYZZ\n", dir);
	}
	fprintf(f, ":00000001FF\n");
	fclose(f);
	return 0;
}