#include "s21_grep.h"

int main(int argc, char** argv) {
  Flag flag = {0};
  if (argc > 2) {
    flag.pattern = (char*)malloc(sizeof(char));
    parser(&flag, argc, argv);
    flag.count_files = argc - optind;
    for (int i = optind; i < argc; i++) {
      find_coincidence(flag, argv[i]);
    }
    free(flag.pattern);
  } else
    printf(
        "usage: s21_grep [-cefhilnosv] [-e pattern] [-f file] [pattern] [file "
        "...]\n");
  return 0;
}

void parser(Flag* flags, int argc, char** argv) {
  int value = 0;
  const struct option long_options[] = {{NULL, 0, NULL, 0}};
  while ((value = getopt_long(argc, argv, "e:ivclnhsf:o", long_options,
                              NULL)) != -1) {
    if (value == 'e') {
      add_pattern(flags, optarg);
      flags->fl_e++, flags->count_fl++;
    } else if (value == 'i') {
      flags->regex_flag |= REG_ICASE;
      flags->fl_i = 1, flags->count_fl++;
    } else if (value == 'v') {
      flags->fl_v = 1, flags->count_fl++;
    } else if (value == 'c') {
      flags->fl_c = 1, flags->count_fl++;
    } else if (value == 'l') {
      flags->fl_l = 1, flags->count_fl++;
    } else if (value == 'n') {
      flags->fl_n = 1, flags->count_fl++;
    } else if (value == 'h') {
      flags->fl_h = 1, flags->count_fl++;
    } else if (value == 's') {
      flags->fl_s = 1, flags->count_fl++;
    }
  }
  if (strcmp(flags->pattern, "\0") == 0) {
    strcpy(flags->pattern, argv[optind++]);
  }
  for (int i = 1; i < argc; i++) {
    if ((strstr(argv[argc - i], "-") != NULL) && strlen(argv[argc - i]) > 2) {
      flags->count_fl -= strlen(argv[argc - i] - 2);
    }
  }
}

void add_pattern(Flag* flags, char* pattern) {
  if (strcmp(flags->pattern, "\0") != 0) strcat(flags->pattern, "|");
  strcat(flags->pattern, pattern);
}

void find_coincidence(Flag flags, char* file_n) {
  FILE* file = fopen(file_n, "r");
  STR str[MAX] = {0};
  if (file == NULL) {
    if (!flags.fl_s) perror(file_n);
    return;
  }
  regex_t re;
  if (!regcomp(&re, flags.pattern, REG_EXTENDED | flags.regex_flag)) {
    char* line = NULL;
    int read = 0;
    size_t memlen = 0;
    read = getline(&line, &memlen, file);
    int num = 1;
    while (read != -1) {
      if (!regexec(&re, line, 0, NULL, 0) && !flags.fl_v) {
        strcpy(str[flags.count_str].str, line);
        str[flags.count_str].str_num = num;
        flags.count_str++;
      } else if (regexec(&re, line, 0, NULL, 0) && flags.fl_v) {
        strcpy(str[flags.count_str].str, line);
        str[flags.count_str].str_num = num;
        flags.count_str++;
      }
      read = getline(&line, &memlen, file);
      num++;
    }
    free(line);
    regfree(&re);
  }
  int flag = 0;
  if (!flags.fl_h && flags.count_files > 1) flag++;
  print(flags, str, file_n, flag);
  fclose(file);
}

void print(Flag flags, STR str[], char* file_n, int flag) {
  if (flags.fl_c && flag == 1 && !flags.fl_l) {
    printf("%s:%d\n", file_n, flags.count_str);
  } else if (flags.fl_c && flag == 0 && !flags.fl_l) {
    printf("%d\n", flags.count_str);
  } else if (flags.fl_c && flag == 0 && flags.fl_l) {
    printf("%d\n", flags.count_str > 0);
  } else if (flags.fl_c && flag == 1 && flags.fl_l) {
    printf("%s:%d\n", file_n, flags.count_str > 0);
  }
  if (flags.fl_l && flags.count_str > 0) {
    printf("%s\n", file_n);
  }
  if (flags.fl_n && !flags.fl_c && !flags.fl_l && !flag) {
    for (int i = 0; i < flags.count_str; i++) {
      printf("%d:", str[i].str_num);
      printf("%s", str[i].str);
    }
  } else if (flags.fl_n && !flags.fl_c && !flags.fl_l && flag) {
    for (int i = 0; i < flags.count_str; i++) {
      printf("%s:", file_n);
      printf("%d:", str[i].str_num);
      printf("%s", str[i].str);
    }
  }
  if (!flags.fl_c && !flags.fl_l && !flags.fl_n && flag) {
    for (int i = 0; i < flags.count_str; i++) {
      printf("%s:%s", file_n, str[i].str);
    }
  } else if (!flags.fl_c && !flags.fl_l && !flags.fl_n && !flag) {
    for (int i = 0; i < flags.count_str; i++) {
      printf("%s", str[i].str);
    }
  }
}