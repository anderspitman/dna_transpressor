
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

void print_vector(unsigned char *vec, int len);
int symbol_to_number(char sym);
char number_to_symbol(int num);
void pack_sequence(char *symbols, int len_sym, unsigned char **packed_seq, 
                   int *len_packed);
void unpack_sequence(unsigned char *packed_seq, int len_packed,
                       char **symbols, int *len_sym);
void symbols_from_file(const char *filename, char **out_symbols,
                       int *size);
void symbols_to_file(const char *filename, char *symbols, int len_symbols);
void packed_from_file(const char *filename, unsigned char **out_packed,
                      int *size);
void packed_to_file(const char *filename, unsigned char *packed_seq,
                  int size, int len);
void file_symbols_to_packed(const char *in_file, const char *out_file);
void file_packed_to_symbols(const char *in_file, const char *out_file);
void test_pack_sequence();

#endif // _DNA_TRANSPRESSOR_H_
