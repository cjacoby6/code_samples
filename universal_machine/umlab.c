#include "umlab.h"

#define OP_WIDTH 4
#define OP_LSB 28
#define REG_WIDTH 3
#define VAL_WIDTH 25

Um_instruction three_register(Um_opcode op, int ra, int rb, int rc)
{
        uint64_t word = 0;
        word = Bitpack_newu(word, OP_WIDTH, OP_LSB, (uint64_t)op);
        word = Bitpack_newu(word, REG_WIDTH, (REG_WIDTH * 2), (uint64_t)ra);
        word = Bitpack_newu(word, REG_WIDTH, (REG_WIDTH * 1), (uint64_t)rb);
        word = Bitpack_newu(word, REG_WIDTH, (REG_WIDTH * 0), (uint64_t)rc);
        return (Um_instruction)word;
}

Um_instruction loadval(unsigned ra, unsigned val)
{
        uint64_t word = 0;
        word = Bitpack_newu(word, OP_WIDTH, OP_LSB, (uint64_t)13);
        word = Bitpack_newu(word, REG_WIDTH, VAL_WIDTH, (uint64_t)ra);
        word = Bitpack_newu(word, VAL_WIDTH, 0, (uint64_t)val);
        return (Um_instruction)word;
}

static inline Um_instruction cmove(int ra, int rb, int rc)
{
	return three_register(0, ra, rb, rc);
}

static inline Um_instruction segload(int ra, int rb, int rc)
{
        return three_register(1, ra, rb, rc);
}

static inline Um_instruction segstore(int ra, int rb, int rc)
{
        return three_register(2, ra, rb, rc);
}

static inline Um_instruction add(int ra, int rb, int rc)
{
    	return three_register(3, ra, rb, rc);
}

static inline Um_instruction multiply(int ra, int rb, int rc)
{
	return three_register(4, ra, rb, rc);
}

static inline Um_instruction divide(int ra, int rb, int rc)
{
	return three_register(5, ra, rb, rc);
}

static inline Um_instruction nand(int ra, int rb, int rc)
{
    	return three_register(6, ra, rb, rc);
}

static inline Um_instruction halt(void)
{
	return three_register(7, 0, 0, 0);
}

static inline Um_instruction mapseg(int rb, int rc)
{
        return three_register(8, 0, rb, rc);
}

static inline Um_instruction unmapseg(int rc)
{
        return three_register(9, 0, 0, rc);
}

static inline Um_instruction output(int rc)
{
        return three_register(10, 0, 0, rc);
}

static inline Um_instruction input(int rc)
{
	return three_register(11, 0, 0, rc);
}

static inline Um_instruction loadprogram(int rb, int rc)
{
	return three_register(12, 0, rb, rc);
}

enum regs { r0 = 0, r1, r2, r3, r4, r5, r6, r7 };

extern void Um_write_sequence(FILE *output, Seq_T stream)
{
        for (int i = 0; i < Seq_length(stream); i++) {
                Um_instruction word = 
				(Um_instruction)(uintptr_t)Seq_get(stream, i);
                for(int i = 3; i >= 0; i--) {
                        char c = Bitpack_getu(word, 8, (i * 8)); 
                        fputc(c, output);
                }
        }
}

static inline void emit(Seq_T stream, Um_instruction inst)
{
	assert(sizeof(inst) <= sizeof(uintptr_t));
	Seq_addhi(stream, (void *)(uintptr_t) inst);
}

static inline Um_instruction get_inst(Seq_T stream, int i)
{
	assert(sizeof(Um_instruction) <= sizeof(uintptr_t));
	return (Um_instruction) (uintptr_t) (Seq_get(stream, i));
}

static inline void put_inst(Seq_T stream, int i, Um_instruction inst)
{
	assert(sizeof(inst) <= sizeof(uintptr_t));
	Seq_put(stream, i, (void *)(uintptr_t) inst);
}

void emit_halt_test(Seq_T stream)
{
	emit(stream, halt());
	emit(stream, loadval(r1, 'B'));
	emit(stream, output(r1));
	emit(stream, loadval(r1, 'a'));
	emit(stream, output(r1));
	emit(stream, loadval(r1, 'd'));
	emit(stream, output(r1));
	emit(stream, loadval(r1, '!'));
	emit(stream, output(r1));
	emit(stream, loadval(r1, '\n'));
	emit(stream, output(r1));
}

void emit_load_test(Seq_T stream)
{
        emit(stream, loadval(r1, 'N'));
	emit(stream, loadval(r2, 'O'));
	emit(stream, halt());
	emit(stream, output(r1));
	emit(stream, output(r2));
}

void emit_out_test(Seq_T stream)
{
	emit(stream, loadval(r1, 'O'));
	emit(stream, output(r1));
	emit(stream, loadval(r1, 'u'));
	emit(stream, output(r1));
	emit(stream, loadval(r1, 't'));
	emit(stream, output(r1));
	emit(stream, loadval(r2, '\n'));
	emit(stream, output(r2));
	emit(stream, halt());
}

void emit_add_test(Seq_T stream)
{
        emit(stream, loadval(r1, '4'));
        emit(stream, loadval(r2, '2'));
        emit(stream, add(r3, r1, r2));
	emit(stream, output(r1));
	emit(stream, loadval(r4, '+'));
	emit(stream, output(r4));
	emit(stream, output(r2));
	emit(stream, loadval(r4, '='));
	emit(stream, output(r4));
        emit(stream, output(r3));
        emit(stream, loadval(r1, '\n'));
        emit(stream, output(r1));
	emit(stream, halt());
}

void emit_in_test(Seq_T stream)
{
	emit(stream, input(r1));
	emit(stream, output(r1));
	emit(stream, loadval(r2, '\n'));
	emit(stream, output(r2));
	emit(stream, halt());
}

void emit_multiply_test(Seq_T stream)
{
    	emit(stream, loadval(r2, 9));
	emit(stream, loadval(r3, 4));
	emit(stream, loadval(r4, '='));
	emit(stream, output(r4));
	emit(stream, multiply(r1, r2, r3));
	emit(stream, output(r1));
	emit(stream, loadval(r4, '\n'));
	emit(stream, output(r4));
	emit(stream, halt());
}

void emit_divide_test(Seq_T stream)
{
    	emit(stream, loadval(r1, 104));
	emit(stream, loadval(r2, 2));
	emit(stream, loadval(r4, '='));
	emit(stream, output(r4));
	emit(stream, divide(r3, r1, r2));
	emit(stream, output(r3));
	emit(stream, loadval(r4, '\n'));
	emit(stream, output(r4));
	emit(stream, halt());
}

void emit_nand_test(Seq_T stream)
{
    	emit(stream, loadval(r1, 'f'));
	emit(stream, loadval(r2, 'e'));
	emit(stream, nand(r3, r1, r2));
        emit(stream, nand(r3, r3, r3));
	emit(stream, loadval(r4, '\n'));
	emit(stream, output(r3));
	emit(stream, output(r4));
}

void emit_cmov_test(Seq_T stream)
{
        emit(stream, loadval(r1, 'Y'));
        emit(stream, loadval(r2, 'B'));
        emit(stream, cmove(r1, r2, r0));
        emit(stream, output(r1));
        emit(stream, loadval(r1, 'e'));
        emit(stream, loadval(r2, 'a'));
        emit(stream, cmove(r1, r2, r0));
        emit(stream, output(r1));
        emit(stream, loadval(r1, 's'));
        emit(stream, loadval(r2, 'd'));
        emit(stream, cmove(r1, r2, r0));
        emit(stream, output(r1));
        emit(stream, loadval(r1, '}'));
        emit(stream, loadval(r2, '!'));
        emit(stream, loadval(r0, 100));
        emit(stream, cmove(r1, r2, r0));
        emit(stream, output(r1));
        emit(stream, loadval(r2, '\n'));
        emit(stream, cmove(r1, r2, r0));
        emit(stream, output(r1));
}

void emit_segload_test(Seq_T stream)
{
        emit(stream, loadval(r0, 100));
        emit(stream, loadval(r1, 0));
        emit(stream, loadval(r2, 19));
        emit(stream, segload(r0, r1, r2));
        emit(stream, loadval(r1, 100));
        emit(stream, loadval(r4, 'F'));
        emit(stream, cmove(r1, r4, r0));
        emit(stream, output(r1));
        emit(stream, loadval(r4, 'a'));
        emit(stream, cmove(r1, r4, r0));
        emit(stream, output(r1));
        emit(stream, loadval(r4, 'i'));
        emit(stream, cmove(r1, r4, r0));
        emit(stream, output(r1));
        emit(stream, loadval(r4, 'l'));
        emit(stream, cmove(r1, r4, r0));
        emit(stream, output(r1));
        emit(stream, loadval(r4, '\n'));
        emit(stream, output(r4));
        emit(stream, cmove(r0, r0, r0));
        emit(stream, halt());
}

void emit_segstore_test(Seq_T stream)
{
        emit(stream, loadval(r0, 0));
        emit(stream, loadval(r1, 14));
        emit(stream, loadval(r2, '{'));
        emit(stream, segstore(r0, r1, r2));
        emit(stream, segload(r2, r0, r1));
        emit(stream, output(r2));
        emit(stream, loadval(r2, '}'));
        emit(stream, segstore(r0, r1, r2));
	emit(stream, segload(r2, r0, r1));
	emit(stream, output(r2));
	emit(stream, loadval(r2, 29360128));
        emit(stream, loadval(r7, 64));
        emit(stream, multiply(r2, r2, r7));
	emit(stream, segstore(r0, r1, r2));
	emit(stream, loadval(r1, 'N'));
	emit(stream, output(r1));
	emit(stream, loadval(r1, 'O'));
	emit(stream, output(r1));
        emit(stream, output(r1));
        emit(stream, output(r1));
        emit(stream, loadval(r1, '!'));
        emit(stream, output(r1));
        emit(stream, output(r1));
        emit(stream, halt());
}

void emit_mapseg_test(Seq_T stream)
{
        emit(stream, loadval(r1, 6));
        emit(stream, mapseg(r0, r1));
        emit(stream, loadval(r1, 0));
        emit(stream, loadval(r4, 'P'));
        emit(stream, segstore(r0, r1, r4));
        emit(stream, loadval(r4, 'a'));
        emit(stream, loadval(r1, 1));
        emit(stream, segstore(r0, r1, r4));
        emit(stream, loadval(r4, 's'));
        emit(stream, loadval(r1, 2));
        emit(stream, segstore(r0, r1, r4));
        emit(stream, loadval(r4, 's'));
        emit(stream, loadval(r1, 3));
        emit(stream, segstore(r0, r1, r4));
        emit(stream, loadval(r4, '!'));
        emit(stream, loadval(r1, 4));
        emit(stream, segstore(r0, r1, r4));
        emit(stream, loadval(r4, '\n'));
        emit(stream, loadval(r1, 5));
        emit(stream, segstore(r0, r1, r4));
        for(int i = 0; i < 6; i++) {
                emit(stream, loadval(r1, i));
                emit(stream, segload(r5, r0, r1));
                emit(stream, output(r5));
        }
        emit(stream, halt());
}

void emit_unmapseg_test(Seq_T stream)
{
        emit(stream, loadval(r1, 3));
        emit(stream, mapseg(r0, r1));
        emit(stream, mapseg(r2, r1));
        emit(stream, loadval(r1, 0));
        emit(stream, loadval(r3, 'N'));
        emit(stream, segstore(r0, r1, r3));
        emit(stream, loadval(r3, 'O'));
        emit(stream, segstore(r2, r1, r3));
        emit(stream, unmapseg(r2));
        emit(stream, loadval(r1, 8));
        emit(stream, mapseg(r4, r1));
        emit(stream, loadval(r6, 'Y'));
        emit(stream, loadval(r0, 0));
        emit(stream, segstore(r4, r0, r6));
        emit(stream, segload(r7, r4, r0));
        emit(stream, output(r7));
        emit(stream, loadval(r1, '\n'));
        emit(stream, output(r1));
}

void emit_loadprog_test(Seq_T stream)
{
        emit(stream, loadval(r0, 'Y'));
        emit(stream, loadval(r2, 3));
        emit(stream, mapseg(r1, r2));
        emit(stream, loadval(r3, 17));
        emit(stream, loadval(r4, 19));
        emit(stream, loadval(r5, 20));
        emit(stream, loadval(r2, 0));
        emit(stream, segload(r7, r2, r3));
        emit(stream, segstore(r1, r2, r7));
        emit(stream, segload(r7, r2, r4));
        emit(stream, loadval(r2, 1));
        emit(stream, segstore(r1, r2, r7));
        emit(stream, loadval(r2, 0));
        emit(stream, segload(r7, r2, r5));
        emit(stream, loadval(r2, 2));
        emit(stream, segstore(r1, r2, r7));
        emit(stream, loadprogram(r1, r6));
        emit(stream, unmapseg(r1));
        emit(stream, loadval(r0, 'F'));
        emit(stream, output(r0));
        emit(stream, halt());
}

static void add_label(Seq_T stream, int location_to_patch, int label_value)
{
	Um_instruction inst = get_inst(stream, location_to_patch);
	unsigned k = Bitpack_getu(inst, 25, 0);
	inst = Bitpack_newu(inst, 25, 0, label_value + k);
	put_inst(stream, location_to_patch, inst);
}

static void emit_out_string(Seq_T stream, const char *s, int aux_reg)
{
        for (unsigned i = 0; i < strlen(s); i++) {
                emit(stream, loadval(aux_reg, s[i]));
                emit(stream, output(aux_reg));
        };
}

void emit_goto_test(Seq_T stream)
{
	int patch_L = Seq_length(stream);
	emit(stream, loadval(r7, 0));	     /* will be patched to 'r7 := L' */
	emit(stream, loadprogram(r0, r7));   /* should goto label L          */
	emit_out_string(stream, "GOTO failed.\n", r1);
	emit(stream, halt());
	/* define 'L' to be here */
	add_label(stream, patch_L, Seq_length(stream));	
	emit_out_string(stream, "GOTO passed.\n", r1);
	emit(stream, halt());
}
