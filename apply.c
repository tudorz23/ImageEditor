/* Copyright Marius-Tudor Zaharia 313CAa 2022-2023 */
#include "utils.h"
#include "apply.h"

void apply_kernel(image *photo, int **kernel)
{
	// coordonatele pixelilor asupra carora aplicam nucleul
	// verificam daca sunt pixeli marginali
	int coord1_x = photo->x1;
	if (coord1_x == 0)
		coord1_x++;

	int coord1_y = photo->y1;
	if (coord1_y == 0)
		coord1_y++;

	int coord2_x = photo->x2;
	if (coord2_x == photo->columns)
		coord2_x--;

	int coord2_y = photo->y2;
	if (coord2_y == photo->rows)
		coord2_y--;

	// matrice aux, caci valorile din imagine se modifica continuu
	int **aux_red = alloc_matrix(photo->rows, photo->columns);
	int **aux_green = alloc_matrix(photo->rows, photo->columns);
	int **aux_blue = alloc_matrix(photo->rows, photo->columns);
	if (!aux_red || !aux_green || !aux_blue) {
		fprintf(stderr, "Alocarea unei auxiliare a esuat\n");
		exit(-1);
	}
	for (int i = 0; i < photo->rows; i++)
		for (int j = 0; j < photo->columns; j++) {
			aux_red[i][j] = photo->red[i][j];
			aux_green[i][j] = photo->green[i][j];
			aux_blue[i][j] = photo->blue[i][j];
		}
	for (int i = coord1_y; i < coord2_y; i++) {
		for (int j = coord1_x; j < coord2_x; j++) {
			int sum_red = 0;
			int sum_green = 0;
			int sum_blue = 0;
			for (int k = 0; k <= 2; k++) {
				for (int p = 0 ; p <= 2; p++) {
					sum_red += aux_red[i + k - 1][j + p - 1] * kernel[k][p];
					sum_green += aux_green[i + k - 1][j + p - 1] * kernel[k][p];
					sum_blue += aux_blue[i + k - 1][j + p - 1] * kernel[k][p];
				}
			}
			photo->red[i][j] = sum_red;
			photo->green[i][j] = sum_green;
			photo->blue[i][j] = sum_blue;
		}
	}
	free_any_matrix(aux_red, photo->rows);
	free_any_matrix(aux_green, photo->rows);
	free_any_matrix(aux_blue, photo->rows);
}

void edge(image *photo)
{
	int **kernel = alloc_matrix(3, 3);
	if (!kernel) {
		fprintf(stderr, "Alocarea nucleului a esuat\n");
		exit(-1);
	}
	// punem valorile in kernel
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			kernel[i][j] = -1;
	kernel[1][1] = 8;
	apply_kernel(photo, kernel);

	// clamping, pentru ca nu l-am facut in apply_kernel
	for (int i = photo->y1; i < photo->y2; i++)
		for (int j = photo->x1; j < photo->x2; j++) {
			clamp(&photo->red[i][j], 0, photo->max_intensity);
			clamp(&photo->green[i][j], 0, photo->max_intensity);
			clamp(&photo->blue[i][j], 0, photo->max_intensity);
		}

	free_any_matrix(kernel, 3);
}

void sharpen(image *photo)
{
	int **kernel = alloc_matrix(3, 3);
	if (!kernel) {
		fprintf(stderr, "Alocarea nucleului a esuat\n");
		exit(-1);
	}
	for (int i = 0; i <= 2; i += 2)
		for (int j = 0; j <= 2; j += 2)
			kernel[i][j] = 0;
	kernel[0][1] = -1;
	kernel[1][0] = -1;
	kernel[1][2] = -1;
	kernel[2][1] = -1;
	kernel[1][1] = 5;
	apply_kernel(photo, kernel);

	// clamping, pentru ca nu l-am facut in apply_kernel
	for (int i = photo->y1; i < photo->y2; i++)
		for (int j = photo->x1; j < photo->x2; j++) {
			clamp(&photo->red[i][j], 0, photo->max_intensity);
			clamp(&photo->green[i][j], 0, photo->max_intensity);
			clamp(&photo->blue[i][j], 0, photo->max_intensity);
		}

	free_any_matrix(kernel, 3);
}

void blur(image *photo)
{
	int **kernel = alloc_matrix(3, 3);
	if (!kernel) {
		fprintf(stderr, "Alocarea nucleului a esuat\n");
		exit(-1);
	}
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			kernel[i][j] = 1;
	apply_kernel(photo, kernel);

	// coordonatele pixelilor asupra carora aplicam nucleul
	// verificam daca sunt pixeli marginali
	int coord1_x = photo->x1;
	if (coord1_x == 0)
		coord1_x++;

	int coord1_y = photo->y1;
	if (coord1_y == 0)
		coord1_y++;

	int coord2_x = photo->x2;
	if (coord2_x == photo->columns)
		coord2_x--;

	int coord2_y = photo->y2;
	if (coord2_y == photo->rows)
		coord2_y--;

	double new_value; // stocam raportul dintre valoare si 9
	for (int i = coord1_y; i < coord2_y; i++)
		for (int j = coord1_x; j < coord2_x; j++) {
			new_value = (double)(photo->red[i][j]) / 9;
			photo->red[i][j] = round(new_value);

			new_value = (double)(photo->green[i][j]) / 9;
			photo->green[i][j] = round(new_value);

			new_value = (double)(photo->blue[i][j]) / 9;
			photo->blue[i][j] = round(new_value);
		}

	free_any_matrix(kernel, 3);
}

void gaussian(image *photo)
{
	int **kernel = alloc_matrix(3, 3);
	if (!kernel) {
		fprintf(stderr, "Alocarea nucleului a esuat\n");
		exit(-1);
	}
	for (int i = 0; i <= 2; i += 2)
		for (int j = 0; j <= 2; j += 2)
			kernel[i][j] = 1;
	kernel[0][1] = 2;
	kernel[1][0] = 2;
	kernel[1][2] = 2;
	kernel[2][1] = 2;
	kernel[1][1] = 4;
	apply_kernel(photo, kernel);

	// coordonatele pixelilor asupra carora aplicam nucleul
	// verificam daca sunt pixeli marginali
	int coord1_x = photo->x1;
	if (coord1_x == 0)
		coord1_x++;

	int coord1_y = photo->y1;
	if (coord1_y == 0)
		coord1_y++;

	int coord2_x = photo->x2;
	if (coord2_x == photo->columns)
		coord2_x--;

	int coord2_y = photo->y2;
	if (coord2_y == photo->rows)
		coord2_y--;

	double new_value; // stocam raportul dintre valoare si 16
	for (int i = coord1_y; i < coord2_y; i++)
		for (int j = coord1_x; j < coord2_x; j++) {
			new_value = (double)(photo->red[i][j]) / 16;
			photo->red[i][j] = round(new_value);

			new_value = (double)(photo->green[i][j]) / 16;
			photo->green[i][j] = round(new_value);

			new_value = (double)(photo->blue[i][j]) / 16;
			photo->blue[i][j] = round(new_value);
		}

	free_any_matrix(kernel, 3);
}

void apply(char *user_input, image *photo)
{
	if (photo->is_loaded == 0) {
		printf("No image loaded\n");
		return;
	}
	strtok(user_input, "\n "); // sar peste "APPLY"
	char *token = strtok(NULL, "\n ");
	if (!token) {
		printf("Invalid command\n");
		return;
	}
	if (photo->magic_number == 2 || photo->magic_number == 5) {
		printf("Easy, Charlie Chaplin\n");
		return;
	}

	if (strcmp(token, "EDGE") == 0) {
		edge(photo);
		printf("APPLY EDGE done\n");
		return;
	}

	if (strcmp(token, "SHARPEN") == 0) {
		sharpen(photo);
		printf("APPLY SHARPEN done\n");
		return;
	}

	if (strcmp(token, "BLUR") == 0) {
		blur(photo);
		printf("APPLY BLUR done\n");
		return;
	}

	if (strcmp(token, "GAUSSIAN_BLUR") == 0) {
		gaussian(photo);
		printf("APPLY GAUSSIAN_BLUR done\n");
		return;
	}
	printf("APPLY parameter invalid\n");
}
