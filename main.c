/* Copyright Marius-Tudor Zaharia 313CAa 2022-2023 */
#include "commands.h"
#include "utils.h"
#include "load.h"
#include "save.h"
#include "apply.h"
#include "rotate.h"

int main(void)
{
	char user_input[100];
	char delim[] = "\n ";
	FILE *input_file = NULL;
	image photo;
	photo.is_loaded = 0;
	while (1) {
		fgets(user_input, 100, stdin);

		// facem o copie pentru a putea "strica" sirul cu strtok
		char copy_of_input[100];
		memcpy(copy_of_input, user_input, strlen(user_input) + 1);

		// determinam comanda introdusa
		char *command = strtok(copy_of_input, delim);

		if (strcmp(command, "LOAD") == 0) {
			load_image(user_input, input_file, &photo);
		} else if (strcmp(command, "SELECT") == 0) {
			select_area(user_input, &photo);
		} else if (strcmp(command, "HISTOGRAM") == 0) {
			histogram(user_input, photo);
		} else if (strcmp(command, "EQUALIZE") == 0) {
			equalize(&photo);
		} else if (strcmp(command, "ROTATE") == 0) {
			rotate(user_input, &photo);
		} else if (strcmp(command, "CROP") == 0) {
			crop(&photo);
		} else if (strcmp(command, "APPLY") == 0) {
			apply(user_input, &photo);
		} else if (strcmp(command, "EXIT") == 0) {
			exit_prog(&photo);
			break;
		} else if (strcmp(command, "SAVE") == 0) {
			save_image(user_input, input_file, photo);
		} else {
			printf("Invalid command\n");
		}
	}

	return 0;
}
