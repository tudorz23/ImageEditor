/* Copyright Marius-Tudor Zaharia 313CAa 2022-2023 */
#include "utils.h"
#include "commands.h"
#include "load.h"
#include "save.h"

void write_to_text_file(FILE *input_file, char *file_name, image photo)
{
	input_file = fopen(file_name, "w");
	if (!input_file) {
		fprintf(stderr, "Deschiderea fisierului a esuat\n");
		exit(-1);
	}
	// pentru a folosi "cuvinte" mai scurte
	int width = photo.columns;
	int height = photo.rows;

	if (photo.magic_number == 2 || photo.magic_number == 5) {
		// imagine grayscale
		fprintf(input_file, "P2\n");
		fprintf(input_file, "%d %d\n", width, height);
		fprintf(input_file, "%d\n", photo.max_intensity);

		for (int i = 0; i < height - 1; i++) {
			for (int j = 0; j < width - 1; j++)
				fprintf(input_file, "%d ", photo.gray[i][j]);
			// printez separat ultimul element,
			// pentru a nu avea trailing whitespace
			fprintf(input_file, "%d\n", photo.gray[i][width - 1]);
		}

		// printez ultima linie fara \n la final
		for (int j = 0; j < width - 1; j++)
			fprintf(input_file, "%d ", photo.gray[height - 1][j]);

		// printez separat ultimul element, pentru a evita trailing whitespace
		fprintf(input_file, "%d", photo.gray[height - 1][width - 1]);

	} else if (photo.magic_number == 3 || photo.magic_number == 6) {
		// imagine color
		fprintf(input_file, "P3\n"); // magic word
		fprintf(input_file, "%d %d\n", width, height);
		fprintf(input_file, "%d\n", photo.max_intensity);

		for (int i = 0; i < height - 1; i++) {
			for (int j = 0; j < width - 1; j++)
				fprintf(input_file, "%d %d %d ", photo.red[i][j],
						photo.green[i][j], photo.blue[i][j]);
			// printez separat ultimele elemente,
			// pentru a nu avea trailing whitespace
			fprintf(input_file, "%d ", photo.red[i][width - 1]);
			fprintf(input_file, "%d ", photo.green[i][width - 1]);
			fprintf(input_file, "%d\n", photo.blue[i][width - 1]);
		}

		// printez ultima linie fara \n la final
		for (int j = 0; j < width - 1; j++) {
			fprintf(input_file, "%d ", photo.red[height - 1][j]);
			fprintf(input_file, "%d ", photo.green[height - 1][j]);
			fprintf(input_file, "%d ", photo.blue[height - 1][j]);
		}

		// printez separat ultimul element, pentru a evita trailing whitespace
		fprintf(input_file, "%d ", photo.red[height - 1][width - 1]);
		fprintf(input_file, "%d ", photo.green[height - 1][width - 1]);
		fprintf(input_file, "%d", photo.blue[height - 1][width - 1]);
	}
	fclose(input_file);
}

void write_to_binary_file(FILE *input_file, char *file_name, image photo)
{
	input_file = fopen(file_name, "wb");
	if (!input_file) {
		fprintf(stderr, "Deschiderea fisierului a esuat\n");
		exit(-1);
	}
	// pentru a folosi "cuvinte" mai scurte
	int width = photo.columns;
	int height = photo.rows;

	if (photo.magic_number == 2 || photo.magic_number == 5) {
		// imagine grayscale
		fprintf(input_file, "P5\n");
		fprintf(input_file, "%d %d\n", width, height);
		fprintf(input_file, "%d\n", photo.max_intensity);

		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++)
				fwrite(&photo.gray[i][j], sizeof(char), 1, input_file);

	} else if (photo.magic_number == 3 || photo.magic_number == 6) {
		// imagine color
		fprintf(input_file, "P6\n"); // magic word
		fprintf(input_file, "%d %d\n", width, height);
		fprintf(input_file, "%d\n", photo.max_intensity);

		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				fwrite(&photo.red[i][j], sizeof(char), 1, input_file);
				fwrite(&photo.green[i][j], sizeof(char), 1, input_file);
				fwrite(&photo.blue[i][j], sizeof(char), 1, input_file);
			}
		}
	}
	fclose(input_file);
}

void save_image(char *user_input, FILE *input_file, image photo)
{
	if (photo.is_loaded == 0) {
		printf("No image loaded\n");
		return;
	}

	strtok(user_input, "\n "); // sar peste "SAVE"
	char *file_name = strtok(NULL, "\n ");
	if (!file_name) {
		printf("Invalid command\n");
		return;
	}

	char *token = strtok(NULL, "\n ");
	if (!token) {
		// fisier binar
		write_to_binary_file(input_file, file_name, photo);
	} else if (strcmp(token, "ascii") == 0) {
		// fisier text
		write_to_text_file(input_file, file_name, photo);
	} else {
		// s-au adaugat cuvinte in plus
		printf("Invalid command\n");
		return;
	}
	printf("Saved %s\n", file_name);
}
