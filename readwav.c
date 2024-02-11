#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define HEADER_SIZE 44
#define SAMPLE_SIZE sizeof(short)
#define DEFAULT_DELAY 8000
#define DEFAULT_VOLUME_SCALE 4

/*
Function to check if a string is a positive integer.

Checks if the entire string passed to it represents a 
positive integer by iterating through each character of 
the string and verifying that each character is a digit.
 */
int is_positive_integer(const char *str) {
    while (*str != '\0') {
        if (*str < '0' || *str > '9') return 0; // Not a digit
        str++;
    }
    return 1;
}

// Check if a string is a valid float
int is_valid_float(const char *str) {
    int has_decimal_point = 0;
    if (*str == '-') { // Skip leading minus sign
        str++;
    }
    while (*str != '\0') {
        if (*str == '.') {
            if (has_decimal_point) return 0; // More than one decimal point
            has_decimal_point = 1;
        } else if (*str < '0' || *str > '9') {
            return 0; // Not a digit
        }
        str++;
    }
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input file> <output file> [delay] [volume_scale]\n", argv[0]);
        return 1;
    }

    char *input_file_path = argv[1];
    // Open the input WAV file
    FILE *input_file = fopen(input_file_path, "rb");
    if (!input_file) {
        fprintf(stderr, "Could not open input file: %s\n", input_file_path);
        return 1;
    }

    // Read the header from the input file
    short header[HEADER_SIZE / 2];
    if (fread(header, sizeof(short), HEADER_SIZE / 2, input_file) != HEADER_SIZE/2) {
        fprintf(stderr, "Error reading header from input file\n");
        fclose(input_file);
    	return 1;
    }

    // Read and process samples from the input file
    short sample;
    while (fread(&sample, SAMPLE_SIZE, 1, input_file) == 1) {
	printf("%d ", sample);
       }

    fclose(input_file);

    return 0; // Exit successfully
}
