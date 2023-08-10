/* Copyright Marius-Tudor Zaharia 313CAa 2022-2023 */
#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define TOTAL 256

typedef struct {
	int magic_number;
	int columns, rows;
	int max_intensity;
	// coordonatele unei selectii
	int x1, y1, x2, y2;
	// pentru imaginile grayscale
	int **gray;
	// pentru imaginile color
	int **red;
	int **green;
	int **blue;
	// 1 daca exista o imagine in memorie, 0 altfel
	int is_loaded;
} image;

void swap_ints(int *a, int *b);
int check_power_of_2(int x);
void free_any_matrix(int **matrix, int rows);
void clamp(int *value, int minn, int maxx);
int **alloc_matrix(int rows, int columns);
void free_matrix_by_type(image *photo);
void ignore_comments_and_empty_lines(FILE *input_file);


#endif /* _UTILS_H */
