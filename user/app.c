#include "ulibc.h"

// Entry point of a user application
void main() {
	cat("nb_secret_logo");
	const uint16_t MAX = 255;
	uint_t secret = random(MAX + 1);
	uint8_t nb_live = 10;
	bool victory = false;
	char buf[ULIBC_SM_BUFFER];
	while (nb_live) {
		printf("Trouvez le nombre secret [0-%d] ! Il vous reste %d vies\n\n", buf, MAX, nb_live);
		puts("Votre proposition : ");
		read_string(buf, ULIBC_SM_BUFFER);
		uint_t res = atoi(buf);
		if (res > secret) {
			puts("\nLe nombre secret est plus petit\n");
		} else if (res < secret) {
			puts("\nLe nombre secret est plus grand\n");
		} else {
			victory = true;
			break;
		}
		nb_live--;
	}
	
	if (victory) {
		printf("\nBravo ! Le nombre secret est %d\n", buf, secret);
	} else {
		printf("\nDommage ! Le nombre secret est %d\n", buf, secret);
	}
}