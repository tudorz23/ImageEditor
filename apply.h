/* Copyright Marius-Tudor Zaharia 313CAa 2022-2023 */
#ifndef _APPLY_H_
#define _APPLY_H_

void apply_kernel(image *photo, int **kernel);
void edge(image *photo);
void sharpen(image *photo);
void blur(image *photo);
void gaussian(image *photo);
void apply(char *user_input, image *photo);

#endif /* _APPLY_H_ */
