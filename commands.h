/* Copyright Marius-Tudor Zaharia 313CAa 2022-2023 */
#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#include "utils.h"

void select_all(image *photo);
void select_area(char *user_input, image *photo);

void crop_grayscale(image *photo);
void crop_color(image *photo);
void crop(image *photo);

void histogram(char *user_input, image photo);

void equalize(image *photo);

void exit_prog(image *photo);

#endif /* _COMMANDS_H_ */
