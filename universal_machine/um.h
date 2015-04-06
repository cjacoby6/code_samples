/*
 * Cori Jacoby
 * HW6
 * um.h
 * 11/7/14
 *
 * Interface for universal machine implementation
 */

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <bitpack.h>
#include <seq.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "instructions.h"
#include "segments.h"

/* initializes the UM memory and reads in the given file as it's program */
Seq_T initialize_um(FILE *file, off_t file_bytes);

/* runs UM with the given memory */
void run_um(Seq_T mem);

/* frees memory associated with the UM */
void free_um(Seq_T *mem);

/* returns the opcode from the uint32_t word */
unsigned get_opcode (uint32_t word);

/* returns ra from the uint32_t word */
unsigned get_ra (uint32_t word);

/* returns rb from the uint32_t word */
unsigned get_rb (uint32_t word);

/* returns rc from the uint32_t word */
unsigned get_rc (uint32_t word);

/* returns ra from the uint32_t word used in the loadval instruction */
unsigned get_val_ra(uint32_t word);

/* reads in the next word from the given file and returns it as a uint32_t */
uint32_t read_in_word(FILE *file);
