/* Copyright Marius-Tudor Zaharia 313CAa 2022-2023 */
#include "commands.h"
#include "utils.h"
#include "load.h"

// citeste din fisier magic word-ul si retruneaza cifra care
// da tipul imaginii
int determine_type_of_image(FILE *input_file, char *file_name)
{
	input_file = fopen(file_name, "r");
	if (!input_file)
		return 0;

	// in caz ca sunt spatii inainte de "P"
	ignore_comments_and_empty_lines(input_file);

	char magic_word; // Va salva caracterul "P"
	fscanf(input_file, "%c", &magic_word);

	int magic_number; // cifra care da tipul imaginii
	fscanf(input_file, "%d", &magic_number);

	fclose(input_file);
	return magic_number;
}

int read_from_text_file(FILE *input_file, char *file_name, image *photo)
{
	input_file = fopen(file_name, "r");
	if (!input_file)
		return 0;

	// in caz ca sunt spatii inainte de "P"
	ignore_comments_and_empty_lines(input_file);

	// sar peste magic word
	char magic_word; // Va salva caracterul "P"
	fscanf(input_file, "%c", &magic_word);

	int magic_number; // cifra care da tipul imaginii
	fscanf(input_file, "%d", &magic_number);
	ignore_comments_and_empty_lines(input_file);

	fscanf(input_file, "%d%d", &photo->columns, &photo->rows);
	ignore_comments_and_empty_lines(input_file);

	fscanf(input_file, "%d", &photo->max_intensity);
	ignore_comments_and_empty_lines(input_file);

	if (photo->magic_number == 2) {
		// imagine grayscale
		photo->gray = alloc_matrix(photo->rows, photo->columns);
		if (!(photo->gray))
			return 0;
		for (int i = 0; i < photo->rows; i++) {
			for (int j = 0; j < photo->columns; j++)
				fscanf(input_file, "%d", &photo->gray[i][j]);
		}

	} else {
		// imagine color
		photo->red = alloc_matrix(photo->rows, photo->columns);
		photo->green = alloc_matrix(photo->rows, photo->columns);
		photo->blue = alloc_matrix(photo->rows, photo->columns);

		if (!(photo->red) || !(photo->green) || !(photo->blue))
			return 0;

		for (int i = 0; i < photo->rows; i++) {
			for (int j = 0; j < photo->columns; j++)
				fscanf(input_file, "%d%d%d", &photo->red[i][j],
					   &photo->green[i][j], &photo->blue[i][j]);
		}
	}

	fclose(input_file);
	return 1;
}

int read_from_binary_file(FILE *input_file, char *file_name, image *photo)
{
	input_file = fopen(file_name, "rb");
	if (!input_file)
		return 0;

	// in caz ca sunt spatii inainte de "P"
	ignore_comments_and_empty_lines(input_file);

	// sar peste magic word
	char magic_word; // Va salva caracterul "P"
	fscanf(input_file, "%c", &magic_word);

	int magic_number; // cifra care da tipul imaginii
	fscanf(input_file, "%d", &magic_number);
	ignore_comments_and_empty_lines(input_file);

	fscanf(input_file, "%d%d", &photo->columns, &photo->rows);
	ignore_comments_and_empty_lines(input_file);

	fscanf(input_file, "%d", &photo->max_intensity);
	ignore_comments_and_empty_lines(input_file);

	if (photo->magic_number == 5) {
		// imagine grayscale
		photo->gray = alloc_matrix(photo->rows, photo->columns);
		if (!(photo->gray))
			return 0;

		// citesc matricea din fisier, octet cu octet
		for (int i = 0; i < photo->rows; i++) {
			for (int j = 0; j < photo->columns; j++)
				fread(&photo->gray[i][j], sizeof(char), 1, input_file);
		}

	} else {
		// imagine color
		photo->red = alloc_matrix(photo->rows, photo->columns);
		photo->green = alloc_matrix(photo->rows, photo->columns);
		photo->blue = alloc_matrix(photo->rows, photo->columns);

		if (!(photo->red) || !(photo->green) || !(photo->blue))
			return 0;

		for (int i = 0; i < photo->rows; i++) {
			for (int j = 0; j < photo->columns; j++) {
				fread(&photo->red[i][j], sizeof(char), 1, input_file);
				fread(&photo->green[i][j], sizeof(char), 1, input_file);
				fread(&photo->blue[i][j], sizeof(char), 1, input_file);
			}
		}
	}

	fclose(input_file);
	return 1;
}

void load_image(char *user_input, FILE *input_file, image *photo)
{
	strtok(user_input, "\n "); // sar peste "LOAD"
	char *file_name = strtok(NULL, "\n ");

	// daca este deja o imagine incarcata in memorie
	if (photo->is_loaded == 1) {
		free_matrix_by_type(photo);
		photo->is_loaded = 0;
	}

	photo->magic_number = determine_type_of_image(input_file, file_name);
	if (photo->magic_number == 0) {
		printf("Failed to load %s\n", file_name);
		return;
	}

	if (photo->magic_number == 2 || photo->magic_number == 3) {
		// fisier text
		if (!read_from_text_file(input_file, file_name, photo)) {
			printf("Failed to load %s\n", file_name);
			return;
		}
	} else if ((photo->magic_number == 5) || (photo->magic_number == 6)) {
		// fisier binar
		if (!read_from_binary_file(input_file, file_name, photo)) {
			printf("Failed to load %s\n", file_name);
			return;
		}
	}
	select_all(photo);

	// imaginea este incarcata in memorie
	photo->is_loaded = 1;
	printf("Loaded %s\n", file_name);
}
