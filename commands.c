/* Copyright Marius-Tudor Zaharia 313CAa 2022-2023 */
#include "commands.h"
#include "load.h"

// selecteaza intreaga imagine
void select_all(image *photo)
{
	photo->x1 = 0;
	photo->y1 = 0;
	photo->x2 = photo->columns;
	photo->y2 = photo->rows;
}

// selecteaza o anumita zona
void select_area(char *user_input, image *photo)
{
	if (photo->is_loaded == 0) {
		printf("No image loaded\n");
		return;
	}
	strtok(user_input, " "); // sar peste "SELECT"
	char *token = strtok(NULL, "\n ");
	if (!token) {
		printf("Invalid command\n");
		return;
	}
	if (strcmp(token, "ALL") == 0) {
		select_all(photo);
		printf("Selected ALL\n");
		return;
	}
	// daca e doar select, trebuie sa urmeze coordonatele
	int x1 = atoi(token);
	if (x1 == 0 && strcmp(token, "0") != 0) {
		printf("Invalid command\n");
		return;
	}
	token = strtok(NULL, "\n ");
	if (!token) {
		printf("Invalid command\n");
		return;
	}
	int y1 = atoi(token);
	if (y1 == 0 && strcmp(token, "0") != 0) {
		printf("Invalid command\n");
		return;
	}
	token = strtok(NULL, "\n ");
	if (!token) {
		printf("Invalid command\n");
		return;
	}
	int x2 = atoi(token);
	if (x2 == 0 && strcmp(token, "0") != 0) {
		printf("Invalid command\n");
		return;
	}
	token = strtok(NULL, "\n ");
	if (!token) {
		printf("Invalid command\n");
		return;
	}
	int y2 = atoi(token);
	if (y2 == 0 && strcmp(token, "0") != 0) {
		printf("Invalid command\n");
		return;
	}
	if (x1 == x2 || y1 == y2) {
		printf("Invalid set of coordinates\n");
		return;
	}

	if (x1 < 0 || x2 < 0 || x1 > photo->columns ||
		x2 > photo->columns) {
		printf("Invalid set of coordinates\n");
		return;
	}
	if (y1 < 0 || y2 < 0 || y1 > photo->rows || y2 > photo->rows) {
		printf("Invalid set of coordinates\n");
		return;
	}
	if (x1 > x2)
		swap_ints(&x1, &x2);
	if (y1 > y2)
		swap_ints(&y1, &y2);
	printf("Selected %d %d %d %d\n", x1, y1, x2, y2);
	photo->x1 = x1;
	photo->y1 = y1;
	photo->x2 = x2;
	photo->y2 = y2;
}

void crop_grayscale(image *photo)
{
	int new_columns = photo->x2 - photo->x1;
	int new_rows = photo->y2 - photo->y1;
	int **aux_gray = alloc_matrix(new_rows, new_columns);
	if (!aux_gray) {
		fprintf(stderr, "Alocarea auxiliarei a esuat\n");
		exit(-1);
	}
	for (int i = photo->y1; i < photo->y2; i++)
		for (int j = photo->x1; j < photo->x2; j++)
			aux_gray[i - photo->y1][j - photo->x1] = photo->gray[i][j];

	// eliberez vechea imagine
	free_matrix_by_type(photo);

	// aloc memorie pentru imaginea cropped
	photo->columns = new_columns;
	photo->rows = new_rows;
	photo->gray = alloc_matrix(photo->rows, photo->columns);
	if (!photo->gray) {
		fprintf(stderr, "Alocarea imaginii cropped a esuat\n");
		exit(-1);
	}
	for (int i = 0; i < new_rows; i++)
		for (int j = 0; j < new_columns; j++)
			photo->gray[i][j] = aux_gray[i][j];

	// eliberez auxiliara
	free_any_matrix(aux_gray, new_rows);
}

void crop_color(image *photo)
{
	int new_columns = photo->x2 - photo->x1;
	int new_rows = photo->y2 - photo->y1;
	int **aux_red = alloc_matrix(new_rows, new_columns);
	int **aux_green = alloc_matrix(new_rows, new_columns);
	int **aux_blue = alloc_matrix(new_rows, new_columns);
	if (!aux_red || !aux_green || !aux_blue) {
		fprintf(stderr, "Alocarea unei auxiliare a esuat\n");
		exit(-1);
	}
	for (int i = photo->y1; i < photo->y2; i++)
		for (int j = photo->x1; j < photo->x2; j++) {
			aux_red[i - photo->y1][j - photo->x1] = photo->red[i][j];
			aux_green[i - photo->y1][j - photo->x1] = photo->green[i][j];
			aux_blue[i - photo->y1][j - photo->x1] = photo->blue[i][j];
		}

	// eliberez vechea imagine
	free_matrix_by_type(photo);

	// aloc memorie pentru imaginea cropped
	photo->columns = new_columns;
	photo->rows = new_rows;

	photo->red = alloc_matrix(photo->rows, photo->columns);
	photo->green = alloc_matrix(photo->rows, photo->columns);
	photo->blue = alloc_matrix(photo->rows, photo->columns);
	if (!photo->red || !photo->green || !photo->blue) {
		fprintf(stderr, "Alocarea imaginii cropped a esuat\n");
		exit(-1);
	}

	for (int i = 0; i < new_rows; i++)
		for (int j = 0; j < new_columns; j++) {
			photo->red[i][j] = aux_red[i][j];
			photo->green[i][j] = aux_green[i][j];
			photo->blue[i][j] = aux_blue[i][j];
		}
	// eliberez auxiliarele
	free_any_matrix(aux_red, new_rows);
	free_any_matrix(aux_green, new_rows);
	free_any_matrix(aux_blue, new_rows);
}

// reduce imaginea la selectia curenta
void crop(image *photo)
{
	if (photo->is_loaded == 0) {
		printf("No image loaded\n");
		return;
	}
	if (photo->magic_number == 2 || photo->magic_number == 5)
		crop_grayscale(photo);
	else
		crop_color(photo);

	select_all(photo);
	printf("Image cropped\n");
}

void histogram(char *user_input, image photo)
{
	if (photo.is_loaded == 0) {
		printf("No image loaded\n");
		return;
	}
	strtok(user_input, "\n "); // sar peste "HISTOGRAM"
	char *token = strtok(NULL, "\n ");
	if (!token) {
		printf("Invalid command\n");
		return;
	}
	int stars = atoi(token);

	token = strtok(NULL, "\n ");
	if (!token) {
		printf("Invalid command\n");
		return;
	}
	int bins = atoi(token);
	if (check_power_of_2(bins) == 0 || bins < 2) {
		printf("Invalid set of parameters\n");
		return;
	}

	token = strtok(NULL, "\n ");
	if (token) {
		printf("Invalid command\n");
		return;
	}
	if (photo.magic_number == 3 || photo.magic_number == 6) {
		printf("Black and white image needed\n");
		return;
	}
	int *frequency = (int *)calloc(bins, sizeof(int));
	if (!frequency) {
		fprintf(stderr, "Alocarea vectorului de frecventa a esuat\n");
		exit(-1);
	}
	int numbers_per_bin = TOTAL / bins; // cate nr sunt intr-un interval
	// In frequency salvam suma frecventelor numerelor din fiecare
	// interval determinat de bin-uri
	for (int i = 0; i < photo.rows; i++) {
		for (int j = 0; j < photo.columns; j++) {
			// Determinam intervalul in care se afla valoarea
			int interval = photo.gray[i][j] / numbers_per_bin;
			frequency[interval]++;
		}
	}
	// Gasim maximul frecventelor
	int maxx = 0;
	for (int i = 0; i < bins; i++)
		if (frequency[i] > maxx)
			maxx = frequency[i];

	// Afisam histograma
	for (int i = 0; i < bins; i++) {
		// rezultat impartire
		double division = (double)(frequency[i]) / (double)(maxx);

		// cate stele (numar real)
		double no_of_stars = division * (double)(stars);

		// cate stele (nr intreg)
		int integer_stars = (int)(no_of_stars);
		printf("%d\t|", integer_stars);

		if (integer_stars > 0) {
			printf("\t");
			for (int j = 0; j < integer_stars; j++)
				printf("*");
		}
		printf("\n");
	}
	free(frequency);
}

void equalize(image *photo)
{
	if (photo->is_loaded == 0) {
		printf("No image loaded\n");
		return;
	}
	if (photo->magic_number == 3 || photo->magic_number == 6) {
		printf("Black and white image needed\n");
		return;
	}
	// vector de frecventa pentru valorile de la 0 la 255
	int *frequency = (int *)calloc(TOTAL, sizeof(int));
	if (!frequency) {
		fprintf(stderr, "Alocarea vectorului de frecventa a esuat\n");
		exit(-1);
	}
	int area = photo->rows * photo->columns;

	for (int i = 0; i < photo->rows; i++)
		for (int j = 0; j < photo->columns; j++)
			frequency[photo->gray[i][j]]++;

	// sumele aparitiilor valorilor (din formula)
	int *sum = (int *)calloc(TOTAL, sizeof(int));
	if (!sum) {
		fprintf(stderr, "Alocarea vectorului de sume a esuat\n");
		exit(-1);
	}
	// folosim sume partiale
	sum[0] = frequency[0];
	for (int i = 1; i < TOTAL; i++)
		sum[i] = sum[i - 1] + frequency[i];

	free(frequency);
	// efectuam calculele
	for (int i = 0; i < TOTAL; i++) {
		double new_value = ((double)(sum[i])) / ((double)(area)) * 255;
		int rounded_value = round(new_value);

		// clamping
		clamp(&rounded_value, 0, 255);
		sum[i] = rounded_value;
	}

	for (int i = 0; i < photo->rows; i++)
		for (int j = 0; j < photo->columns; j++)
			photo->gray[i][j] = sum[photo->gray[i][j]];

	free(sum);
	printf("Equalize done\n");
}

void exit_prog(image *photo)
{
	if (photo->is_loaded == 0)
		printf("No image loaded\n");
	else
		free_matrix_by_type(photo);
}
