#ifndef __MNIST_H__
#define __MNIST_H__

#define PRG_BANK_MNIST 1

extern const char mnist_test_images[][784];

void mnist_print_img(const char* buf, int threshold);
void mnist_booleanize_img(char* img, int size, int threshold);

#endif /* __MNIST_H__ */
