#include <stdlib.h>
#include <getopt.h>
#include <errno.h>

#include "rensa.h"


int main(int argc, char **argv)
{
	if (argc == 1) {
		fprintf(stderr, "No options provided\n");
		return 1;
	}
	int opt;
    FILE *input = stdin, *output = stdout, *key = NULL;
	size_t key_shift = 0;
    char zeroth_char = 0;
    int encrypt_mode = 1;
    char *endptr = NULL;
	while ((opt = getopt(argc, argv, "hedi:o:k:s:z:")) != -1) {
		switch (opt) {
			case 'h':
                printf(
                    "(c) Masahiko AMANO aka H1K0, 2024-present\n"
                    "(https://github.com/H1K0/rensa)\n\n"
                    "Usage:\n"
                    "  rensa [-e/-d] [-i <input_path>] [-o <output_path>] [-k <key_path>] [-s <key_shift>] [-z <zeroth_char>]\n\n"
                    "Options:\n"
                    "  -h                  Print help and exit\n"
                    "  -e                  Encryption mode (default)\n"
                    "  -d                  Decryption mode\n"
                    "  -i <input_path>     Input file path (stdio by default)\n"
                    "  -o <output_path>    Output file path (stdout by default)\n"
                    "  -k <key_path>       Key file path\n"
                    "  -s <key_shift>      Key shift in bytes (0 by default)\n"
                    "  -z <zeroth_char>    Zeroth char (0 by default)\n"
                );
                return 0;
            case 'd':
                encrypt_mode = 0;
                break;
            case 'i':
                input = fopen(optarg, "rb");
                if (input == NULL) {
                    perror("Failed to open input file");
                    return errno;
                }
                break;
            case 'o':
                output = fopen(optarg, "wb");
                if (output == NULL) {
                    perror("Failed to open output file");
                    return errno;
                }
                break;
            case 'k':
                key = fopen(optarg, "rb");
                if (key == NULL) {
                    perror("Failed to open key file");
                    return errno;
                }
                break;
            case 's':
                if (key == NULL) {
                    fprintf(stderr, "Key shift is only available if key file specified\n");
                    return 1;
                }
                key_shift = strtoull(optarg, &endptr, 10);
                if (*endptr != 0) {
                    fprintf(stderr, "Invalid key shift\n");
                    return 1;
                }
                break;
            case 'z':
                zeroth_char = (char) strtol(optarg, &endptr, 10);
                if (*endptr != 0) {
                    fprintf(stderr, "Invalid zeroth char\n");
                    return 1;
                }
                break;
            case '?':
                return 1;
            default:
                break;
		}
	}
    if (encrypt_mode) {
        return rensa_fencrypt(input, output, key, key_shift, zeroth_char);
    } else {
        return rensa_fdecrypt(input, output, key, key_shift, zeroth_char);
    }
}
