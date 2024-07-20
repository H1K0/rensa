#include <string.h>
#include <errno.h>

#include "rensa.h"


#define BUFSIZ 8192


int rensa_encrypt(const char *plain, size_t len, const char *key, char zeroth_char, char *cipher) {
	if (key == NULL) {
		*(cipher++) = *(plain++) + zeroth_char;
		for (int i = 1; i < len; i++, plain++, cipher++) {
			*cipher = *plain + *(plain - 1);
		}
	} else {
		*(cipher++) = *(plain++) + key[(unsigned char) zeroth_char];
		for (int i = 1; i < len; i++, plain++, cipher++) {
			*cipher = *plain + key[(unsigned char) *(plain - 1)];
		}
	}
	return 0;
}

int rensa_decrypt(const char *cipher, size_t len, const char *key, char zeroth_char, char *plain) {
	if (key == NULL) {
		*(plain++) = *(cipher++) - zeroth_char;
		for (int i = 1; i < len; i++, plain++, cipher++) {
			*plain = *cipher - *(plain - 1);
		}
	} else {
		*(plain++) = *(cipher++) - key[(unsigned char) zeroth_char];
		for (int i = 1; i < len; i++, plain++, cipher++) {
			*plain = *cipher - key[(unsigned char) *(plain - 1)];
		}
	}
	return 0;
}

int rensa_fencrypt(FILE *input, FILE *output, FILE *key, size_t key_shift, char zeroth_char) {
	if (input == NULL) {
		input = stdin;
	}
	if (output == NULL) {
		output = stdout;
	}
	int status;
	size_t readcount;
	char key_buffer[256];
	if (key != NULL) {
		if (fseek(key, key_shift, SEEK_SET) != 0) {
			return errno;
		}
		readcount = fread(key_buffer, 1, 256, key);
		for (int i = readcount; i < 256; i += readcount) {
			rewind(key);
			readcount = fread(key_buffer + i, 1, 256 - i, key);
		}
	}
	char rbuffer[BUFSIZ], wbuffer[BUFSIZ];
	while ((readcount = fread(rbuffer, 1, BUFSIZ, input)) != 0) {
		status = rensa_encrypt(rbuffer, BUFSIZ, (key != NULL) ? key_buffer : NULL, zeroth_char, wbuffer);
		if (status != 0) {
			return status;
		}
		if (fwrite(wbuffer, 1, readcount, output) < readcount) {
			return -1;
		}
		zeroth_char = rbuffer[BUFSIZ - 1];
	}
	return 0;
}

int rensa_fdecrypt(FILE *input, FILE *output, FILE *key, size_t key_shift, char zeroth_char) {
	if (input == NULL) {
		input = stdin;
	}
	if (output == NULL) {
		output = stdout;
	}
	int status;
	size_t readcount;
	char key_buffer[256];
	if (key != NULL) {
		if (fseek(key, key_shift, SEEK_SET) != 0) {
			return errno;
		}
		readcount = fread(key_buffer, 1, 256, key);
		for (int i = readcount; i < 256; i += readcount) {
			rewind(key);
			readcount = fread(key_buffer + i, 1, 256 - i, key);
		}
	}
	char rbuffer[BUFSIZ], wbuffer[BUFSIZ];
	while ((readcount = fread(rbuffer, 1, BUFSIZ, input)) != 0) {
		status = rensa_decrypt(rbuffer, BUFSIZ, (key != NULL) ? key_buffer : NULL, zeroth_char, wbuffer);
		if (status != 0) {
			return status;
		}
		if (fwrite(wbuffer, 1, readcount, output) < readcount) {
			return -1;
		}
		zeroth_char = wbuffer[BUFSIZ - 1];
	}
	return 0;
}
