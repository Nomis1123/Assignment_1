#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define HEADER_SIZE 44
#define SAMPLE_SIZE sizeof(short)
#define DEFAULT_DELAY 8000
#define DEFAULT_VOLUME_SCALE 0.5

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
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <input file> <output file> [delay] [volume_scale]\n", argv[0]);
        return 1;
    }

    char *input_file_path = argv[1];
    char *output_file_path = argv[2];
    int delay = DEFAULT_DELAY;
    float volume_scale = DEFAULT_VOLUME_SCALE;

    // Validate and parse delay
    if (argc > 3) {
        if (is_positive_integer(argv[3])) {
            delay = atoi(argv[3]);
            if (delay <= 0) {
                fprintf(stderr, "Delay must be a positive integer.\n");
                return 1;
            }
        } else {
            fprintf(stderr, "Invalid delay value. It must be a positive integer.\n");
            return 1;
        }
    }

    // Validate and parse volume_scale
    if (argc > 4) {
        if (is_valid_float(argv[4])) {
            volume_scale = atof(argv[4]);
            if (volume_scale < 0) {
                fprintf(stderr, "Volume scale cannot be negative.\n");
                return 1;
            }
        } else {
            fprintf(stderr, "Invalid volume scale value. It must be a positive number.\n");
            return 1;
        }
    }

    // Open the input WAV file
    FILE *input_file = fopen(input_file_path, "rb");
    if (!input_file) {
        fprintf(stderr, "Could not open input file: %s\n", input_file_path);
        return 1;
    }

    // Open the output WAV file
    FILE *output_file = fopen(output_file_path, "wb");
    if (!output_file) {
        fprintf(stderr, "Could not open output file: %s\n", output_file_path);
        fclose(input_file);
        return 1;
    }

    // Read the header from the input file
    unsigned char header[HEADER_SIZE];
    if (fread(header, 1, HEADER_SIZE, input_file) != HEADER_SIZE) {
        fprintf(stderr, "Error reading header from input file\n");
        fclose(input_file);
        fclose(output_file);
        return 1;
    }

    // Write the header to the output file
    if (fwrite(header, 1, HEADER_SIZE, output_file) != HEADER_SIZE) {
        fprintf(stderr, "Error writing header to output file\n");
        fclose(input_file);
        fclose(output_file);
        return 1;
    }

    // Initialize the echo buffer with the size based on delay
    short *echo_buffer = malloc(delay * SAMPLE_SIZE);
    if (!echo_buffer) {
        fprintf(stderr, "Error allocating memory for echo buffer\n");
        fclose(input_file);
        fclose(output_file);
        return 1;
    }
    // Manually set allocated memory to zero
    memset(echo_buffer, 0, delay * SAMPLE_SIZE);

    // Read and process samples from the input file
    short sample;
    int buffer_index = 0;
    while (fread(&sample, SAMPLE_SIZE, 1, input_file) == 1) {
        // Get the echo sample from the echo buffer
        short echo_sample = echo_buffer[buffer_index];

        // Scale the echo sample
        echo_sample = (short)(echo_sample * volume_scale);

        // Mix the original sample with the echo sample
        short mixed_sample = sample + echo_sample;

        // Write the mixed sample to the output file
        if (fwrite(&mixed_sample, SAMPLE_SIZE, 1, output_file) != 1) {
            fprintf(stderr, "Error writing mixed sample to output file\n");
            free(echo_buffer);
            fclose(input_file);
            fclose(output_file);
            return 1;
        }

        // Update the echo buffer
        echo_buffer[buffer_index] = sample;
        buffer_index = (buffer_index + 1) % delay;
    }

    // Cleanup
    free(echo_buffer);
    fclose(input_file);
    fclose(output_file);

    return 0; // Exit successfully
}
