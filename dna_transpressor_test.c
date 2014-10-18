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

void test_pack_sequence() {
  char *name= "test_pack_sequence";
  //char *in_sym = "GATTACAA";
  char *in_sym = "ACAATGAG";
  char *symbols;
  unsigned char *packed_seq;
  int len_seq;
  //print_vector(in_sym, 8);
  pack_sequence(in_sym, 8, &packed_seq, &len_seq);
  //fprintf(stderr, "other test%s", "\n");
  //print_vector(packed_seq, len_seq);
  if (packed_seq[0] != 0x10 ||
      packed_seq[1] != 0xE2) {
    fail(name);
  }
}

void test_unpack_sequence() {
  char *name = "test_unpack_sequence";
  unsigned char packed[] = { 0x8F, 0x10 };
  char *sym;
  int len_sym;
  unpack_sequence(packed, 2, &sym, &len_sym);
  if (sym[0] != 'G' ||
      sym[1] != 'A') {
    fail(name);
  }
}

void test_symbols_from_file() {
  char *name= "test_symbols_from_file";
  char *sym;
  int len;
  symbols_from_file("symbols.txt", &sym, &len);
  if (sym[0] != 'A' ||
      sym[1] != 'C' ||
      sym[2] != 'A' ||
      sym[3] != 'A' ||
      sym[4] != 'T' ||
      sym[5] != 'G' ||
      sym[6] != 'A' ||
      sym[7] != 'G') {
    fail(name);
  }
}

void test_file_symbols_to_packed() {
  char *name = "test_file_symbols_to_packed";
  file_symbols_to_packed("symbols.txt", "packed.bin");
}

void test_file_packed_to_symbols() {
  char *name= "test_file_packed_to_symbols";
  file_packed_to_symbols("packed.bin", "symbols2.txt");
}

void test_packed_from_file() {
  char *name= "test_packed_from_file";
  unsigned char *packed_seq;
  int len_packed;
  packed_from_file("packed.bin", &packed_seq, &len_packed);
}

void test_symbols_to_file() {
  char *name= "test_symbol_to_number";
  char *buf = malloc(1108251);
  symbols_to_file("testfile1.txt", buf, 1108251);
  symbols_to_file("testfile.txt", "GATTACA", 7);
}

void test_file_end_to_end() {
  char *name = "test_file_end_to_end";
  file_symbols_to_packed("symbols.txt", "packed.bin");
  file_packed_to_symbols("packed.bin", "symbols2.txt");
}

int main(int argc, char **argv) {
  test_symbol_to_number();
  test_number_to_symbol();
  test_pack_sequence();
  test_unpack_sequence();
  test_symbols_from_file();
  test_file_symbols_to_packed();
  test_file_packed_to_symbols();
  test_packed_from_file();
  test_symbols_to_file();
  test_file_end_to_end();
  printf("All tests passed\n");
  return 0;
}
