#ifndef S21_GREP_H
#define S21_GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 512

typedef struct Flags {
  int fl_e, fl_i, fl_v, fl_c, fl_l, fl_n, fl_h, fl_s, fl_f, fl_o;
  int count_fl, count_files;
  int regex_flag;
  char* pattern;
  int count_str;
} Flag;

typedef struct STR {
  char str[MAX];
  int str_num;
} STR;

void parser(Flag* flags, int argc, char** argv);
void add_pattern(Flag* flags, char* pattern);
void flag_f(Flag* flags, char* file_n);
void find_coincidence(Flag flags, char* file_n);
void flag_o(char* string, Flag flags, STR stuc[]);
void print(Flag flags, STR str[], char* file_n, int flag);

#endif