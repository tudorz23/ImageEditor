/* Copyright Marius-Tudor Zaharia 313CAa 2022-2023 */
#ifndef _LOAD_H_
#define _LOAD_H_

int determine_type_of_image(FILE *input_file, char *file_name);
int read_from_text_file(FILE *input_file, char *file_name, image *photo);
int read_from_binary_file(FILE *input_file, char *file_name, image *photo);
void load_image(char *user_input, FILE *input_file, image *photo);

#endif /* _LOAD_H_ */
