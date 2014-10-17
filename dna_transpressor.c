#include <stdlib.h>
#include <stdio.h>

#include "dna_transpressor.h"

const int NUM_SYMS = 4;

void print_vector(unsigned char *vec, int len) {
  for (int i=0; i<len; i++) {
    fprintf(stderr, "%c  ", vec[i]);
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

int packed_len_to_sym_len(int packed_len) {
  return packed_len * NUM_SYMS;
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
    packed_seq_push(seq, symbols[i]);
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
  *len_packed = seq->len;
}

void unpack_sequence(unsigned char *packed_seq, int len_packed,
                     char **symbols, int *len_sym) {
  PackedSeq *seq = packed_seq_init_buf(packed_seq, len_packed);
  *len_sym = seq->len;
  *symbols = malloc(seq->len*sizeof(char));
  packed_seq_extract(seq, *symbols);
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


