#ifndef S21_CAT_H
#define S21_CAT_H

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Flags {
  int fl_e;
  int fl_b;
  int fl_v;
  int fl_n;
  int fl_s;
  int fl_t;
  int count_fl;
  int count_files;
  char** files;
} Flag;

struct Flags parser(int argc, char** argv);
int test_files(char** files, int count_files);
void read_f(char* file_n);
int flag_n(char* file_n, int count_str);
int flag_b(char* file_n, int count_str);
void flag_e(char* file_n, struct Flags flag);
void flag_t(char* file_n, struct Flags flag);
void flag_s(char* file_n);
void flag_v(char* file_n);

#endif