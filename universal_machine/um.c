/*
 * Cori Jacoby
 * HW6
 * um.c
 * 11/7/14
 *
 * Implementation for universal machine
 */

#include "um.h"

#define BYTES_IN_WORD 4
#define BYTE 8
#define OP_WIDTH 4
#define OP_LSB 28
#define REG_WIDTH 3
#define VAL_WIDTH 25

/* initializes the UM memory and reads in the given file as it's program  */
Seq_T initialize_um(FILE *file, off_t file_bytes)
{
        assert((file_bytes % BYTES_IN_WORD) == 0);

        Seq_T memory = Seq_new(1);
        
        int words = file_bytes / BYTES_IN_WORD;
        seg_array program = new_seg_array(words);

        int counter = 0;
        while (counter < words) {
                program->array[counter] = read_in_word(file);
                counter++;
        }

        Seq_addhi(memory, program);
        return memory;
}

/* runs um program with the given memory */
void run_um(Seq_T mem)
{
        unsigned counter = 0;
        uint32_t regs[8];
        for (int i = 0; i < 8; i++) {
                regs[i] = 0;
        }

        Seq_T unmappedIDs = Seq_new(1);
	seg_array seg0 = Seq_get(mem, 0);
	unsigned max_instruct = seg0->length;
       
        while (counter < max_instruct) {
                
                uint32_t word = seg0->array[counter];
                unsigned instruction = get_opcode(word);
                        
                switch(instruction) {
                case 0:
                        regs[get_ra(word)] = cmov(regs[get_ra(word)], 
                                                  regs[get_rb(word)],
                                                  regs[get_rc(word)]);
                        break;
                case 1:
                        regs[get_ra(word)] = seg_load(mem, 
                                                     regs[get_rb(word)],
                                                     regs[get_rc(word)]);
                        break;
                case 2:
                        seg_store(mem, regs[get_ra(word)], regs[get_rb(word)],
                                 regs[get_rc(word)]);
                        break;
                case 3:
                        regs[get_ra(word)] = add(regs[get_rb(word)], 
                                                 regs[get_rc(word)]);
                        break;
                case 4:
                        regs[get_ra(word)] = multiply(regs[get_rb(word)],
                                                     regs[get_rc(word)]);
                        break;
                case 5:
                        regs[get_ra(word)] = divide(regs[get_rb(word)],
                                                      regs[get_rc(word)]);
                        break;
                case 6:
                        regs[get_ra(word)] = nand(regs[get_rb(word)],
                                                  regs[get_rc(word)]);
                        break;
                case 7:           
                        counter = max_instruct;
                        break;
                case 8:
                        regs[get_rb(word)] = map_seg(mem, unmappedIDs,
                                                    regs[get_rc(word)]);
                        break;
                case 9:
                        unmap_seg(unmappedIDs, regs[get_rc(word)]);
                        break;
                case 10:
                        output(regs[get_rc(word)]);
                        break;
                case 11: 
                        regs[get_rc(word)] = input();
                        break;
                case 12:
                        loadprogram(mem, regs[get_rb(word)]);
                        counter = (unsigned)regs[get_rc(word)];

                        seg0 = Seq_get(mem, 0);
                        max_instruct = seg0->length;
                        break;
                case 13:
                        regs[get_val_ra(word)] = loadval(word);
                        break;
                }
                if (instruction != 12) counter++;
        }
        Seq_free(&unmappedIDs);
        return;
}

/* frees memory associated with the UM */
void free_um(Seq_T *mem)
{
        for (int i = 0; i < Seq_length(*mem); i++) {
                free_seg_array(Seq_get(*mem, i));
        }

        Seq_free(mem);
}

/* returns the opcode from the uint32_t word */
unsigned get_opcode (uint32_t word)
{
        return Bitpack_getu(word, OP_WIDTH, OP_LSB);
}

/* returns register a from the uint32_t word */
unsigned get_ra (uint32_t word)
{
        return Bitpack_getu(word, REG_WIDTH, (REG_WIDTH * 2));
}

/* returns register b from the uint32_t word */
unsigned get_rb (uint32_t word)
{
        return Bitpack_getu(word, REG_WIDTH, (REG_WIDTH * 1));
}

/* returns register rc from the uint32_t word */
unsigned get_rc (uint32_t word)
{
        return Bitpack_getu(word, REG_WIDTH, (REG_WIDTH * 0));
}

/* returns register ra from the uint32_t word for the instruction loadval */
unsigned get_val_ra (uint32_t word)
{
        return Bitpack_getu(word, REG_WIDTH, VAL_WIDTH);
}

/* reads in the next word from the given file and returns it as a uint32_t */
uint32_t read_in_word(FILE *file)
{
        int byte_num = 3;
        uint32_t word = 0;
        unsigned c;
        while (byte_num >= 0) {
                c = fgetc(file);
                word = Bitpack_newu(word, BYTE, (byte_num * BYTE), c);
                byte_num--;
        }
        return word;
}

int main(int argc, char *argv[])
{
        if (argc == 1) {
                fprintf(stdout, "NO FILE SPECIFIED\n");
                exit(1);
        } else if (argc > 2) {
                fprintf(stdout, "TOO MANY ARGUMENTS\n");
                exit(1);
        }
 
        struct stat prog_stats;
        
        if (stat(argv[1], &prog_stats) == -1) {
                fprintf(stdout, "Program not valid");
                exit(1);
        }

        FILE *file = fopen(argv[1], "r");

        Seq_T mem = initialize_um(file, prog_stats.st_size);
        run_um(mem);
        free_um(&mem);
        fclose(file);

        return 0;
}
