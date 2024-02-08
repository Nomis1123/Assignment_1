#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HEADER_SIZE 44

void add_echo(FILE *sourceFile, FILE *destFile, int delay, float volume_scale);

int main(int argc, char *argv[]) {
	int delay = 8000;
	float volume_scale = 4.0;
	char *sourceFile = NULL, *destFile = NULL;

	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-d") ==0) {
			if (i + 1 < argc) {
				delay = atoi(argv[++i]);
			} else {
				fprintf(stderr, "-d requires an argument. \n");
				exit(EXIT_FAILURE);
			}
		} else if (strcmp(argv[i], "-v") == 0) {
			if (i + 1 < argc) {
				volume_scale = atof(argv[++i]);
			} else {
				fprintf(stderr, "-v requires an argument. \n");
				exit(EXIT_FAILURE);
			}
		} else {
			if (sourceFile == NULL) {
				sourceFile = argv[i];
			} else if (destFile == NULL) {
				destFile = argv[i];
			} else {
				fprintf(stderr, "Usage: %s [-d delay] [-v volume_scale] sourcewav destwav\n", argv[0]);
				exit(EXIT_FAILURE);
			}
		}
	}

	if (sourceFile == NULL || destFile == NULL) {
		fprintf(stderr, "Source and destination files are required.\n");
		fprintf(stderr, "Usage: %s [-d delay] [-v volume_scale] sourcewav destwav\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	// Open source WAV file
	FILE *sourceFp = fopen(sourceFile, "rb");
	if (!sourceFp) {
		perror("Error opening source file");
		exit(EXIT_FAILURE);
	}

	// Open destination WAV file
	FILE *destFp = fopen(destFile, "wb");
	if (!destFp) {
		perror("Error opening destination file");
		fclose(sourceFp);
		exit(EXIT_FAILURE);
	}

	// Add echo to WAV file
	add_echo(sourceFp, destFp, delay, volume_scale);

	fclose(sourceFp);
	fclose(destFp);
		

	return 0;
}

void add_echo(FILE *sourceFile, FILE *destFile, int delay, float volume_scale) {
	unsigned int *sizeptr;
	short header[HEADER_SIZE / 2]; // Header data

		// Read header
	fread(header, sizeof(short), HEADER_SIZE / 2, sourceFile);

	// Adjust file size in header
	sizeptr = (unsigned int *)(header + 2);
	*sizeptr += delay * 2;
	sizeptr = (unsigned int *)(header + 20);
	*sizeptr += delay * 2;

	// Write updated header to destination file
	fwrite(header, sizeof(short), HEADER_SIZE / 2, destFile);

	// Allocate echo buffer
	short *echoBuffer = (short *)malloc(delay * sizeof(short));
	if (!echoBuffer) {
		perror("Memory allocation failed");
		exit(EXIT_FAILURE);
	} 
	
	memset(echoBuffer, 0, delay * sizeof(short)); // Initialize echo buffer with zeroes

	free(echoBuffer);


        return 0;
}
