
#ifndef _DNA_TRANSPRESSOR_H_
#define _DNA_TRANSPRESSOR_H_

typedef struct {
  unsigned char *buf;
  char *sym_buf;
  int len;
  int size;
  int byte_idx;
  int bit_idx;
} PackedSeq;

int symbol_to_number(char sym);
char number_to_symbol(int num);
void pack_sequence(char *symbols, int len_sym, unsigned char **packed_seq, 
                   int *len_packed);
void unpack_sequence(unsigned char *packed_seq, int len_packed,
                       char **symbols, int *len_sym);
void pack_from_file(const char *filename, unsigned char **packed_seq,
                    int *len_packed);

#endif // _DNA_TRANSPRESSOR_H_
