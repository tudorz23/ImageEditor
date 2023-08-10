/* Copyright Marius-Tudor Zaharia 313CAa 2022-2023 */
#include "utils.h"
#include "rotate.h"
#include "commands.h"

// roteste o submatrice patratica cu key grade
void rotate_square(image *photo, int key)
{
	int size = photo->x2 - photo->x1; // latura patratului
	int new_y, new_x; // variabile ajutatoare

	if (photo->magic_number == 2 || photo->magic_number == 5) {
		// imagine grayscale
		int **aux_gray = alloc_matrix(size, size);
		if (!aux_gray) {
			fprintf(stderr, "Alocarea auxiliarei a esuat\n");
			exit(-1);
		}
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
				aux_gray[i][j] = photo->gray[i + photo->y1][j + photo->x1];

		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++) {
				if (key == 90) {
					new_y = size - 1 - j;
					new_x = i;
				}
				if (key == 180) {
					new_y = size - 1 - i;
					new_x = size - 1 - j;
				}
				if (key == 270) {
					new_y = j;
					new_x = size - 1 - i;
				}
				photo->gray[i + photo->y1][j + photo->x1] =
							aux_gray[new_y][new_x];
			}
		free_any_matrix(aux_gray, size);

	} else {
		// imagine color
		int **aux_red = alloc_matrix(size, size);
		int **aux_green = alloc_matrix(size, size);
		int **aux_blue = alloc_matrix(size, size);
		if (!aux_red || !aux_green || !aux_blue) {
			fprintf(stderr, "Alocarea unei auxiliare a esuat\n");
			exit(-1);
		}
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++) {
				aux_red[i][j] = photo->red[i + photo->y1][j + photo->x1];
				aux_green[i][j] = photo->green[i + photo->y1][j + photo->x1];
				aux_blue[i][j] = photo->blue[i + photo->y1][j + photo->x1];
			}

		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++) {
				if (key == 90) {
					new_y = size - 1 - j;
					new_x = i;
				}
				if (key == 180) {
					new_y = size - 1 - i;
					new_x = size - 1 - j;
				}
				if (key == 270) {
					new_y = j;
					new_x = size - 1 - i;
				}
				photo->red[i + photo->y1][j + photo->x1] =
							aux_red[new_y][new_x];
				photo->green[i + photo->y1][j + photo->x1] =
							aux_green[new_y][new_x];
				photo->blue[i + photo->y1][j + photo->x1] =
							aux_blue[new_y][new_x];
			}
		free_any_matrix(aux_red, size);
		free_any_matrix(aux_green, size);
		free_any_matrix(aux_blue, size);
	}
}

// roteste toata imaginea 180 de grade
void upside_down(image *photo)
{
	int length = photo->rows;
	int width = photo->columns;
	if (photo->magic_number == 2 || photo->magic_number == 5) {
		int **aux_gray = alloc_matrix(length, width);
		if (!aux_gray) {
			fprintf(stderr, "Alocarea auxiliarei a esuat\n");
			exit(-1);
		}
		for (int i = 0; i < length; i++)
			for (int j = 0; j < width; j++)
				aux_gray[i][j] = photo->gray[i][j];

		for (int i = 0; i < length; i++)
			for (int j = 0; j < width; j++)
				photo->gray[i][j] = aux_gray[length - 1 - i][width - 1 - j];

		free_any_matrix(aux_gray, length);

	} else {
		int **aux_red = alloc_matrix(length, width);
		int **aux_green = alloc_matrix(length, width);
		int **aux_blue = alloc_matrix(length, width);
		if (!aux_red || !aux_green || !aux_blue) {
			fprintf(stderr, "Alocarea unei auxiliare a esuat\n");
			exit(-1);
		}
		for (int i = 0; i < length; i++)
			for (int j = 0; j < width; j++) {
				aux_red[i][j] = photo->red[i][j];
				aux_green[i][j] = photo->green[i][j];
				aux_blue[i][j] = photo->blue[i][j];
			}

		for (int i = 0; i < length; i++)
			for (int j = 0; j < width; j++) {
				photo->red[i][j] = aux_red[length - 1 - i][width - 1 - j];
				photo->green[i][j] = aux_green[length - 1 - i][width - 1 - j];
				photo->blue[i][j] = aux_blue[length - 1 - i][width - 1 - j];
			}
		free_any_matrix(aux_red, length);
		free_any_matrix(aux_green, length);
		free_any_matrix(aux_blue, length);
	}
}

// roteste toata imaginea cu key grade, pe o parte
void to_the_side(image *photo, int key)
{
	int new_x, new_y; // variabile ajutatoare
	int length = photo->rows;
	int width = photo->columns;
	if (photo->magic_number == 2 || photo->magic_number == 5) {
		int **aux_gray = alloc_matrix(length, width);
		if (!aux_gray) {
			fprintf(stderr, "Alocarea auxiliarei a esuat\n");
			exit(-1);
		}
		for (int i = 0; i < length; i++)
			for (int j = 0; j < width; j++)
				aux_gray[i][j] = photo->gray[i][j];

		free_matrix_by_type(photo); // eliberez matricea veche
		photo->gray = alloc_matrix(width, length);// aloc cu noile dimensiuni
		if (!photo->gray) {
			fprintf(stderr, "Alocarea matricei greyscale a esuat\n");
			exit(-1);
		}
		for (int i = 0; i < width; i++)
			for (int j = 0; j < length; j++) {
				if (key == 90) {
					new_y = length - 1 - j;
					new_x = i;
				}
				if (key == 270) {
					new_y = j;
					new_x = width - 1 - i;
				}
				photo->gray[i][j] = aux_gray[new_y][new_x];
			}
		free_any_matrix(aux_gray, length);

	} else {
		int **aux_red = alloc_matrix(length, width);
		int **aux_green = alloc_matrix(length, width);
		int **aux_blue = alloc_matrix(length, width);
		if (!aux_red || !aux_green || !aux_blue) {
			fprintf(stderr, "Alocarea unei auxiliare a esuat\n");
			exit(-1);
		}
		for (int i = 0; i < length; i++)
			for (int j = 0; j < width; j++) {
				aux_red[i][j] = photo->red[i][j];
				aux_green[i][j] = photo->green[i][j];
				aux_blue[i][j] = photo->blue[i][j];
			}

		free_matrix_by_type(photo); // eliberez matricea veche
		photo->red = alloc_matrix(width, length);
		photo->green = alloc_matrix(width, length);
		photo->blue = alloc_matrix(width, length);
		if (!photo->red || !photo->green || !photo->blue) {
			fprintf(stderr, "Alocarea matricei RGB a esuat\n");
			exit(-1);
		}
		for (int i = 0; i < width; i++)
			for (int j = 0; j < length; j++) {
				if (key == 90) {
					new_y = length - 1 - j;
					new_x = i;
				}
				if (key == 270) {
					new_y = j;
					new_x = width - 1 - i;
				}
				photo->red[i][j] = aux_red[new_y][new_x];
				photo->green[i][j] = aux_green[new_y][new_x];
				photo->blue[i][j] = aux_blue[new_y][new_x];
			}
		free_any_matrix(aux_red, length);
		free_any_matrix(aux_green, length);
		free_any_matrix(aux_blue, length);
	}
	// actualizez dimensiunile imaginii
	photo->rows = width;
	photo->columns = length;
	select_all(photo);
}

void rotate(char *user_input, image *photo)
{
	if (photo->is_loaded == 0) {
		printf("No image loaded\n");
		return;
	}

	strtok(user_input, "\n "); // sar peste "ROTATE"
	char *token = strtok(NULL, "\n ");
	if (!token) {
		printf("Invalid command\n");
		return;
	}
	int angle = atoi(token);
	if (angle == 0 && strcmp(token, "0") != 0) {
		printf("Invalid command\n");
		return;
	}

	int width = photo->x2 - photo->x1; // latimea selectiei
	int length = photo->y2 - photo->y1; // lungimea selectiei

	// vad daca e selectata toata imaginea
	int selected_all = 0;
	if (length == photo->rows && width == photo->columns)
		selected_all = 1;

	if (length != width && selected_all == 0) {
		printf("The selection must be square\n");
		return;
	}
	if (((angle % 90) != 0) || (angle < -360 || angle > 360)) {
		printf("Unsupported rotation angle\n");
		return;
	}
	if (angle == 90 || angle == -270) {
		if (selected_all == 0)
			rotate_square(photo, 90);
		else
			to_the_side(photo, 90);
	}
	if (angle == 180 || angle == -180) {
		if (selected_all == 0)
			rotate_square(photo, 180);
		else
			upside_down(photo);
	}
	if (angle == 270 || angle == -90) {
		if (selected_all == 0)
			rotate_square(photo, 270);
		else
			to_the_side(photo, 270);
	}
	printf("Rotated %s\n", token);
}
