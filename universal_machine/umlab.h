#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <seq.h>
#include <bitpack.h>

typedef uint32_t Um_instruction;
typedef unsigned Um_opcode;

Um_instruction three_register(Um_opcode op, int ra, int rb, int rc);

Um_instruction loadval(unsigned ra, unsigned val);

extern void Um_write_sequence(FILE *output, Seq_T stream);

void emit_halt_test(Seq_T stream);

void emit_load_test(Seq_T stream);

void emit_out_test(Seq_T stream);

void emit_add_test(Seq_T stream);

void emit_add_neg_test(Seq_T stream);

void emit_in_test(Seq_T stream);

void emit_multiply_test(Seq_T stream);

void emit_divide_test(Seq_T stream);

void emit_nand_test(Seq_T stream);
