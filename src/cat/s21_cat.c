#include "s21_cat.h"

int main(int argc, char* argv[]) {
  Flag flags = parser(argc, argv);
  int count_str = 1;
  if (!test_files(flags.files, flags.count_files)) {
    for (int i = 0; i < flags.count_files; i++) {
      if (flags.fl_n)
        count_str = flag_n(flags.files[i], count_str);
      else if (flags.fl_b)
        count_str = flag_b(flags.files[i], count_str);
      else if (flags.fl_e)
        flag_e(flags.files[i], flags);
      else if (flags.fl_s)
        flag_s(flags.files[i]);
      else if (flags.fl_t)
        flag_t(flags.files[i], flags);
      else if (flags.fl_v && flags.count_fl == 1)
        flag_v(flags.files[i]);
      else
        read_f(flags.files[i]);
    }
  }
  for (int i = 0; i < flags.count_files; i++) free(flags.files[i]);
  free(flags.files);
  return 0;
}

struct Flags parser(int argc, char** argv) {
  int value;
  const struct option long_options[] = {{"number-nonblank", 0, NULL, 'b'},
                                        {"number", 0, NULL, 'e'},
                                        {"squeeze-blank", 0, NULL, 's'},
                                        {NULL, 0, NULL, 0}};
  Flag flags = {0, 0, 0, 0, 0, 0, 0, 0, NULL};
  while ((value = getopt_long(argc, argv, "+beEnstTv", long_options, NULL)) !=
         -1) {
    if (value == 'e') {
      flags.fl_e = 1;
      flags.fl_v = 1;
      flags.count_fl += 2;
    } else if (value == 'b') {
      flags.fl_b = 1;
      flags.count_fl++;
    } else if (value == 'v') {
      flags.fl_v = 1;
      flags.count_fl++;
    } else if (value == 'n') {
      flags.fl_n = 1;
      flags.count_fl++;
    } else if (value == 's') {
      flags.fl_s = 1;
      flags.count_fl++;
    } else if (value == 't') {
      flags.fl_t = 1;
      flags.count_fl += 2;
    } else if (value == 'T') {
      flags.fl_t = 1;
      flags.count_fl++;
    } else if (value == 'E') {
      flags.fl_e = 1;
      flags.count_fl++;
    }
  }
  flags.files = (char**)malloc((argc - flags.count_fl) * sizeof(char*));
  for (int i = flags.count_fl + 1; i < argc; i++) {
    flags.files[flags.count_files] = (char*)malloc(sizeof(argv[i]));
    strcpy(flags.files[flags.count_files], argv[i]);
    flags.count_files++;
  }
  return flags;
}

int test_files(char** files, int count_files) {
  int flag;
  for (int i = 0; i < count_files; i++) {
    FILE* file = fopen(files[i], "r");
    if (!file) flag = 1;
    fclose(file);
  }
  return flag;
}

void read_f(char* file_n) {
  FILE* file = fopen(file_n, "r");
  char c;
  while ((c = fgetc(file)) != EOF) {
    putc(c, stdout);
  }
  fclose(file);
}

int flag_n(char* file_n, int count_str) {
  FILE* file = fopen(file_n, "r");
  char c0 = fgetc(file);
  char c = fgetc(file);
  int count = count_str;
  int flag = 0;
  while (c != EOF) {
    if (count == count_str && count_str == 1 && flag == 0) {
      printf("%6i\t", count_str);
      flag++;
    }
    putc(c0, stdout);
    if (c0 == '\n' && c != EOF) {
      count_str++;
      printf("%6i\t", count_str);
    }
    c0 = c;
    c = fgetc(file);
  }
  printf("%c", c0);
  fclose(file);
  return count_str;
}

int flag_b(char* file_n, int count_str) {
  FILE* file = fopen(file_n, "r");
  char c;
  char p_c = ' ';
  int count = count_str;
  int flag = 0;
  while ((c = fgetc(file)) != EOF) {
    if (count == count_str && flag == 0 && count_str == 1 && c != '\n') {
      printf("%6i\t", count_str);
      flag++;
    } else if (count == 1 && flag == 0 && c == '\n') {
      flag++;
      count_str++;
    }
    if (p_c == '\n' && c != '\n') {
      count_str++;
      printf("%6i\t", count_str);
      printf("%c", c);
    } else
      printf("%c", c);
    p_c = c;
  }
  return count_str;
}

void flag_e(char* file_n, struct Flags flag) {
  FILE* file = fopen(file_n, "r");
  char c = fgetc(file);
  while (c != EOF) {
    if (c == '\n') {
      printf("$\n");
    } else if (((c >= 0 && c <= 31) || c == 127) && c != 9 && flag.fl_v) {
      printf("^%c", c + 64);
    } else {
      printf("%c", c);
    }
    c = fgetc(file);
  }
}

void flag_t(char* file_n, struct Flags flag) {
  FILE* file = fopen(file_n, "r");
  char c;
  while ((c = fgetc(file)) != EOF) {
    if (c == '\n')
      printf("\n");
    else if (c == '\t') {
      printf("^I");
    } else if (((c >= 0 && c <= 31) || c == 127) && c != 14 && flag.fl_v) {
      printf("^%c", c + 64);
    } else
      printf("%c", c);
  }
}

void flag_s(char* file_n) {
  FILE* file = fopen(file_n, "r");
  char c;
  int count = 0;
  char p_c = ' ';
  while ((c = fgetc(file)) != EOF) {
    if (count != 0 && p_c == '\n' && c == '\n')
      count++;
    else if (count == 0 && p_c == '\n' && c == '\n') {
      count++;
      printf("%c", c);
    } else {
      printf("%c", c);
      count = 0;
    }
    p_c = c;
  }
}

void flag_v(char* file_n) {
  FILE* file = fopen(file_n, "r");
  char c;
  while ((c = fgetc(file)) != EOF) {
    if (c != '\n' && c != '\t') {
      if (c >= 0 && c <= 31) {
        printf("^");
        c += 64;
      } else if (c == 127) {
        printf("^");
        c -= 64;
      }
    }
  }
}