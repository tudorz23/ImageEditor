/* Copyright Marius-Tudor Zaharia 313CAa 2022-2023 */
#ifndef _SAVE_H_
#define _SAVE_H_

void write_to_text_file(FILE *input_file, char *file_name, image photo);
void write_to_binary_file(FILE *input_file, char *file_name, image photo);
void save_image(char *user_input, FILE *input_file, image photo);

#endif /* _SAVE_H_ */
