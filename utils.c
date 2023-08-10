/* Copyright Marius-Tudor Zaharia 313CAa 2022-2023 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "utils.h"

void swap_ints(int *a, int *b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

// verifica daca un numar x este putere a lui 2,
// din intervalul [2, 256], folosind operatii pe biti
int check_power_of_2(int x)
{
	if (x < 2 || x > 256)
		return 0;
	if ((x & (x - 1)) != 0)
		return 0;
	return 1;
}

// elibereaza o matrice alocata dinamic cu rows linii
void free_any_matrix(int **matrix, int rows)
{
	for (int i = 0; i < rows; i++)
		free(matrix[i]);
	free(matrix);
}

void clamp(int *value, int minn, int maxx)
{
	if (*value < minn)
		*value = minn;
	if (*value > maxx)
		*value = maxx;
}

// aloca memorie pentru o matrice
int **alloc_matrix(int rows, int columns)
{
	int **matrix = (int **)calloc(rows, sizeof(int *));
	// Programare defensiva
	if (!matrix) {
		fprintf(stderr, "Alocarea liniilor matricei a esuat.\n");
		return NULL;
	}

	for (int i = 0; i < rows; i++) {
		matrix[i] = (int *)calloc(columns, sizeof(int));
		// Programare defensiva
		if (!matrix[i]) {
			fprintf(stderr, "Alocarea coloanelor matricei a esuat.\n");
			// Dealoc matricea auxiliara (alocata pana in acel moment).
			for (int j = 0; j < i; j++)
				free(matrix[j]);
			free(matrix);
			return NULL;
		}
	}
	return matrix;
}

// elibereaza memoria unei matrice alocate dinamic,
// in functie de tipul imaginii
void free_matrix_by_type(image *photo)
{
	if (photo->magic_number == 2 || photo->magic_number == 5) {
		// eliberez grayscale
		for (int i = 0; i < photo->rows; i++)
			free(photo->gray[i]);
		free(photo->gray);
	} else if (photo->magic_number == 3 || photo->magic_number == 6) {
		// eliberez red
		for (int i = 0; i < photo->rows; i++)
			free(photo->red[i]);
		free(photo->red);

		// eliberez green
		for (int i = 0; i < photo->rows; i++)
			free(photo->green[i]);
		free(photo->green);

		// eliberez blue
		for (int i = 0; i < photo->rows; i++)
			free(photo->blue[i]);
		free(photo->blue);
	}
}

// sare peste liniile cu comentarii si ignora liniile goale
// si trailing whitespaces
void ignore_comments_and_empty_lines(FILE *input_file)
{
	char first_char;
	int rest_of_the_line;
	int position;
	while (1) {
		fscanf(input_file, "%c", &first_char);
		switch (first_char) {
		case '#':
			do {
				// citesc litera cu litera linia pana intalnesc \n
				rest_of_the_line = fgetc(input_file);
			} while (rest_of_the_line != '\n');
			break;

		case '\n':
			break;

		case ' ':
			do {
				rest_of_the_line = fgetc(input_file);
				if ((rest_of_the_line != ' ') && (rest_of_the_line != '\n')) {
					// mut cursorul la caracterul cifra
					position = ftell(input_file) - 1;
					fseek(input_file, position, SEEK_SET);
					return;
				}
			} while (rest_of_the_line != '\n');
			break;

		default:
			// mut cursorul la primul caracter de pe linie
			position = ftell(input_file) - 1;
			fseek(input_file, position, SEEK_SET);
			return;
		}
	}
}
