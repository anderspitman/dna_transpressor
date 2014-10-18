#include <stdlib.h>
#include <stdio.h>

#include "dna_transpressor.h"

const int NUM_SYMS = 4;

void print_vector(unsigned char *vec, int len) {
  for (int i=0; i<len; i++) {
    fprintf(stderr, "%X  ", vec[i]);
    //fprintf(stderr, "%c  ", vec[i]);
  }
  fprintf(stderr, "%s", "\n");
}

void error(char *msg) {
  fprintf(stderr, "error: %s\n", msg);
  exit(0);
}

int sym_len_to_packed_size(int len_sym) {
  int len = len_sym / NUM_SYMS;
  if ((len_sym % NUM_SYMS) != 0) {
    len += 1;
  }
  return len;
}

int packed_size_to_sym_len(int packed_size) {
  return packed_size * NUM_SYMS;
}

void packed_seq_init_common(PackedSeq *seq) {
  seq->byte_idx = 0;
  seq->bit_idx = 0;
  seq->size = sym_len_to_packed_size(seq->len);
}

PackedSeq *packed_seq_init(int len) {
  PackedSeq *new_seq = malloc(sizeof(PackedSeq));
  new_seq->len = len;
  packed_seq_init_common(new_seq);
  new_seq->buf = malloc(new_seq->size * sizeof(unsigned char));
  //for (int i=0; i<seq->len; i++) {
  //  seq->buf[i] = 0;
  //}
  return new_seq;
}

PackedSeq *packed_seq_init_buf(unsigned char *buf, int len) {
  PackedSeq *new_seq = malloc(sizeof(PackedSeq));
  //new_seq->len = packed_size_to_sym_len(size);
  new_seq->len = len;
  packed_seq_init_common(new_seq);
  new_seq->buf = buf;
  return new_seq;
}

void packed_seq_destroy(PackedSeq *seq) {
  free(seq->buf);
  free(seq);
}

void packed_seq_pack(int bits, unsigned char *byte, int position) {
  *byte = *byte | (bits << (6 - (position*2)));
}

void packed_seq_unpack(unsigned char byte, int position, int *bits) {
  *bits = (byte >> (6 - (position*2))) & 0x03;
}

void packed_seq_push(PackedSeq *seq, char sym) {
  int bits = symbol_to_number(sym);  
  
  packed_seq_pack(bits, &seq->buf[seq->byte_idx], seq->bit_idx);

  seq->bit_idx += 1;
  if (seq->bit_idx >= NUM_SYMS) {
    seq->bit_idx = 0;
    seq->byte_idx += 1;
    if (seq->byte_idx >= seq->len) error("push");
  }
}

void packed_seq_pop(PackedSeq *seq, char *sym) {
  int bits;
  packed_seq_unpack(seq->buf[seq->byte_idx], seq->bit_idx, &bits);
  seq->bit_idx += 1;
  if (seq->bit_idx >= NUM_SYMS) {
    seq->bit_idx = 0;
    seq->byte_idx += 1;
    if (seq->byte_idx >= seq->len) error("pop");
  }
  *sym = number_to_symbol(bits);
}

PackedSeq *packed_seq_create(char *symbols, int len_symbols) {
  PackedSeq *seq = packed_seq_init(len_symbols);
  for (int i=0; i<len_symbols; i++) {
    if (symbol_to_number(symbols[i]) != -1) {
      packed_seq_push(seq, symbols[i]);
    }
  }
  return seq;
}

void packed_seq_extract(PackedSeq *seq, char *symbols) {
  for (int i=0; i<seq->len; i++) {
    packed_seq_pop(seq, &symbols[i]);
  }
}

void pack_sequence(char *symbols, int len_sym, unsigned char **packed_seq, 
                   int *len_packed) {
  PackedSeq *seq = packed_seq_create(symbols, len_sym);
  *packed_seq = seq->buf;
  *len_packed = seq->size;
}

void unpack_sequence(unsigned char *packed_seq, int len_packed,
                     char **symbols, int *len_sym) {
  PackedSeq *seq = packed_seq_init_buf(packed_seq, len_packed);
  *len_sym = seq->len;
  *symbols = malloc(seq->len*sizeof(char));
  packed_seq_extract(seq, *symbols);
}

void symbols_from_file(const char *filename, char **out_symbols,
                       int *size) {
  FILE *fp = fopen(filename, "r");
  fseek(fp, 0L, SEEK_END);
  *size = ftell(fp);
  fseek(fp, 0L, SEEK_SET);
  *out_symbols = malloc((*size)*sizeof(char));
  //int read_size = fscanf(fp, "%s", *out_symbols);
  int read_size = fread(*out_symbols, 1, *size, fp);

  //for (int i=0; i<40; i++) {
  //  fprintf(stderr, "%c", (*out_symbols)[i]);
  //}
  //fprintf(stderr, "%s", "\n");

  fclose(fp);
}

void packed_from_file(const char *filename, unsigned char **out_packed,
                      int *size) {
  FILE *fp = fopen(filename, "r");
  //fseek(fp, 0L, SEEK_END);
  //*size = ftell(fp);
  //fseek(fp, 0L, SEEK_SET);
  // read len form first 4 bytes of file
  fread(size, sizeof(int), 1, fp);
  *out_packed = malloc(*size*sizeof(char));
  int read_size = fread(*out_packed, 1, *size, fp);

  //for (int i=0; i<40; i++) {
  //  fprintf(stderr, "%X ", (*out_packed)[i]);
  //}
  //fprintf(stderr, "%s", "\n");

  fclose(fp);
}

void packed_to_file(const char *filename, unsigned char *packed_seq,
                    int size, int len) {
  FILE *fp = fopen(filename, "w");
  // write length to first 4 bytes
  fwrite(&len, sizeof(int), 1, fp);
  fwrite(packed_seq, 1, size, fp);
  fclose(fp);
}

void symbols_to_file(const char *filename, char *symbols, int len_symbols) {
  FILE *fp = fopen(filename, "w");
  fwrite(symbols, 1, len_symbols-1, fp);
  fwrite("\n", 1, 1, fp);
  fclose(fp);
}

void file_symbols_to_packed(const char *in_file, const char *out_file) {
  unsigned char *packed_seq;
  char *symbols;
  //char *in_sym = "ACAATGAG";
  int len_symbols;
  int len_seq;

  FILE *fp = fopen(in_file, "r");
  fseek(fp, 0L, SEEK_END);
  len_symbols = ftell(fp);
  fseek(fp, 0L, SEEK_SET);
  symbols = malloc(len_symbols*sizeof(char));
  //int read_size = fscanf(fp, "%s", *out_symbols);
  int read_size = fread(symbols, 1, len_symbols, fp);

  //symbols_from_file(in_file, &symbols, &len_symbols);
  pack_sequence(symbols, len_symbols, &packed_seq, &len_seq);
  //fprintf(stderr, "this test%s", "\n");
  //print_vector(packed_seq, len_seq);
  packed_to_file(out_file, packed_seq, len_seq, len_symbols);
}

void file_packed_to_symbols(const char *in_file, const char *out_file) {
  unsigned char *packed_seq;
  int len_packed;
  char *symbols;
  int len_symbols;
  packed_from_file(in_file, &packed_seq, &len_packed);
  unpack_sequence(packed_seq, len_packed, &symbols, &len_symbols);
  fprintf(stderr, "lensym: %d\n", len_symbols);
  symbols_to_file(out_file, symbols, len_symbols);
}

int symbol_to_number(char sym) {
  switch (sym) {
    case 'A':
      return 0;
    break;
    case 'C':
      return 1;
    break;
    case 'G':
      return 2;
    break;
    case 'T':
      return 3;
    break;
    default:
      return -1;
    break;
  }
}

char number_to_symbol(int num) {
  switch (num) {
    case 0:
      return 'A';
    break;
    case 1:
      return 'C';
    break;
    case 2:
      return 'G';
    break;
    case 3:
      return 'T';
    break;
    default:
      return 0;
    break;
  }
}


