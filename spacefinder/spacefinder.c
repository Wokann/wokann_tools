#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

long parse_min_length(const char *input) {
    char *endptr;
    long value = 0;

    // Check for 0x or 0X prefix for hexadecimal
    if (strncmp(input, "0x", 2) == 0 || strncmp(input, "0X", 2) == 0) {
        value = strtol(input, &endptr, 16);
    } 
    // Check for 'h' suffix for hexadecimal
    else if (input[strlen(input) - 1] == 'h' || input[strlen(input) - 1] == 'H') {
        char hexInput[256];
        strncpy(hexInput, input, strlen(input) - 1); // Remove 'h' suffix
        hexInput[strlen(input) - 1] = '\0';
        value = strtol(hexInput, &endptr, 16);
    } 
    // Otherwise, assume it's decimal
    else {
        value = strtol(input, &endptr, 10);
    }

    if (*endptr != '\0') {
        fprintf(stderr, "Invalid number format: %s\n", input);
        exit(EXIT_FAILURE);
    }

    return value;
}

void find_continuous_patterns(FILE *inputFile, const char *outputFile, long minLength) {
    FILE *outFile = fopen(outputFile, "w");
    if (outFile == NULL) {
        perror("Failed to open output file");
        exit(EXIT_FAILURE);
    }

    unsigned char buffer;
    long startAddress = -1, endAddress = -1;
    unsigned char currentByte = 0, previousByte = 0;
    long currentAddress = 0;

    while (fread(&buffer, 1, 1, inputFile) == 1) {
        currentByte = buffer;

        // Check if current byte is 0x00 or 0xFF and match with the previous byte
        if ((currentByte == 0x00 || currentByte == 0xFF) && 
            (previousByte == currentByte || startAddress == -1)) {
            if (startAddress == -1) {
                startAddress = currentAddress;
            }
            endAddress = currentAddress;
        } else {
            // If we end a sequence, check its length and write to file if it meets minLength
            if (startAddress != -1 && endAddress != -1) {
                long length = endAddress - startAddress + 1;
                if (length >= minLength) {
                    fprintf(outFile, "Pattern: 0x%02X, Start: 0x%08lX, Length: 0x%lX bytes\n", 
                            previousByte, startAddress, length);
                }
                startAddress = -1;
                endAddress = -1;
            }
        }

        previousByte = currentByte;
        currentAddress++;
    }

    // If file ends with a valid sequence, check its length and write to file
    if (startAddress != -1 && endAddress != -1) {
        long length = endAddress - startAddress + 1;
        if (length >= minLength) {
            fprintf(outFile, "Pattern: 0x%02X, Start: 0x%08lX, Length: 0x%lX bytes\n", 
                    previousByte, startAddress, length);
        }
    }

    fclose(outFile);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <min_length>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *inputFile = argv[1];
    long minLength = parse_min_length(argv[2]);

    if (minLength <= 0) {
        fprintf(stderr, "Invalid minimum length: %ld\n", minLength);
        return EXIT_FAILURE;
    }

    FILE *file = fopen(inputFile, "rb");
    if (file == NULL) {
        perror("Failed to open input file");
        return EXIT_FAILURE;
    }

    find_continuous_patterns(file, "result.txt", minLength);
    fclose(file);

    printf("Pattern search complete. Results saved to result.txt.\n");
    return EXIT_SUCCESS;
}
