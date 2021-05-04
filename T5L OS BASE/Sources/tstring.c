#include "tstring.h"

void t_strcpy(uint8_t *dest, uint8_t *src, uint16_t len){
	int i;
	for (i = 0; i< len; i++){
		dest[i] = src[i];
	}
	return;
}