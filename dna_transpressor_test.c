#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "dna_transpressor.h"

void fail(char *msg) {
  fprintf(stderr, "FAILED: %s\n", msg);
  exit(0);
}

void test_symbol_to_number() {
  char *NAME = "test_symbol_to_number";
  int num;
  num = symbol_to_number('A');
  if (num != 0) fail(NAME);
  num = symbol_to_number('C');
  if (num != 1) fail(NAME);
  num = symbol_to_number('G');
  if (num != 2) fail(NAME);
  num = symbol_to_number('T');
  if (num != 3) fail(NAME);
}

void test_number_to_symbol() {
  char *NAME = "test_number_to_symbol";
  char sym;
  sym = number_to_symbol(0);
  if (sym != 'A') fail(NAME);
  sym = number_to_symbol(1);
  if (sym != 'C') fail(NAME);
  sym = number_to_symbol(2);
  if (sym != 'G') fail(NAME);
  sym = number_to_symbol(3);
  if (sym != 'T') fail(NAME);
}

void test_packed_seq() {
  char *name= "test_packed_seq";
  char *in_sym = "GATTACAGATTACA";
  char *symbols;;
  unsigned char *packed_seq;
  int len_seq;
  int len_sym;
  pack_sequence(in_sym, 14, &packed_seq, &len_seq);
  unpack_sequence(packed_seq, len_seq, &symbols, &len_sym);
  printf("%s\n", symbols);
}

int main(int argc, char **argv) {
  test_symbol_to_number();
  test_number_to_symbol();
  test_packed_seq();
  printf("All tests passed\n");
  return 0;
}
