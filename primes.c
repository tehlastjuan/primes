#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define ARGS_N 2

void print_usage() {
  printf("primes - prime number generator\n");
  printf("\n usage: [amount] [cols]\n");
  printf("-----------------------------------------------------------\n");
  printf(" [amount] - number of prime numbers to generate\n");
  printf(" [cols] - columns to print (default 1)\n");
}

void print_error(char *str) { 
  printf("primes - ERROR: %s\n", str);
}

/*
 * Ripped shamelessly from:
 * https://stackoverflow.com/questions/7021725/how-to-convert-a-string-to-integer-in-c
 *
 */

typedef enum {
  STR2INT_SUCCESS,
  STR2INT_OVERFLOW,
  STR2INT_UNDERFLOW,
  STR2INT_INCONVERTIBLE
} str2int_errno;

str2int_errno strtoint(int *out, char *s, int base) {
  char *end;
  if (s[0] == '\0' || isspace(s[0]))
    return STR2INT_INCONVERTIBLE;
  errno = 0;
  long l = strtol(s, &end, base);
  /* Both checks are needed because INT_MAX == LONG_MAX is possible. */
  if (l > INT_MAX || (errno == ERANGE && l == LONG_MAX))
    return STR2INT_OVERFLOW;
  if (l < INT_MIN || (errno == ERANGE && l == LONG_MIN))
    return STR2INT_UNDERFLOW;
  if (*end != '\0')
    return STR2INT_INCONVERTIBLE;
  *out = l;
  return STR2INT_SUCCESS;
}

int gen_primes(int **a, int s) {
  if (*a == NULL) {
    *a = calloc(1, sizeof(int *));
    if (*a == NULL) {
      print_error("Memory allocation error");
      return EXIT_FAILURE;
    }
  }
  int *t = calloc(s, sizeof(int));
  if (t == NULL) {
    print_error("Memory allocation error");
    return EXIT_FAILURE;
  }

  int p[4] = {2, 3, 5, 7};
  int smp_s = 4;
  for (int i = 0; (i < s) && (i < smp_s); i++)
    t[i] = p[i];

  int k = t[smp_s - 1];
  for (int i = smp_s; i < s; i++) {

    int prim_f = 1;
    for (k++; prim_f == 1;) {
      for (int j = 0; j < smp_s; j++) {
        prim_f = 0;
        if (k % p[j] == 0) {
          prim_f = 1;
          k++;
          break;
        }
      }
      if (prim_f == 0) {
        t[i] = k;
      }
    }
  }

  *a = t;
  return 0;
}

void print_array(int *a, int *args) {
  for (int i = 0; i < args[0]; i++) {
    printf("[%u]\t%u\t", i, a[i]);

    if (i > 1 && ((i + 1) % args[1] == 0)) {
      putchar('\n');
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc == 1) {
    print_usage();
    return EXIT_FAILURE;
  }
  if (argc > 3) {
    print_error("Too many arguments");
    return EXIT_FAILURE;
  }

  int *args = calloc(ARGS_N, sizeof(int));

  for (int i = 0; i < argc - 1; i++) {
    int *temp = calloc(1, sizeof(int));
    if (temp == NULL) {
      print_error("Memory allocation error");
      return EXIT_FAILURE;
    }
    if (strtoint(temp, argv[i + 1], 10) == 0) {
      args[i] = *temp;
    }
    if (args[1] == 0)
      args[1] = 1;

    free(temp);
  }

  for (int i = 0; i < ARGS_N; i++) {
    if (args[i] < 0) {
      print_error("Only positive numbers allowed");
      free(args);
      return EXIT_FAILURE;
    }
  }

  int *array_p = 0;
  gen_primes(&array_p, args[0]);
  print_array(array_p, args);

  return EXIT_SUCCESS;
}
