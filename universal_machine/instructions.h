/*
 * Cori Jacoby
 * HW6
 * instructions.h
 * 11/4/14
 *
 * Interface for instruction implementation
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <seq.h>
#include <assert.h>
#include <bitpack.h>

uint32_t cmov(uint32_t ra, uint32_t rb, uint32_t rc);

uint32_t seg_load(Seq_T mem, uint32_t rb, uint32_t rc);

void seg_store(Seq_T mem, uint32_t ra, uint32_t rb, uint32_t rc);

uint32_t add(uint32_t rb, uint32_t rc);

uint32_t multiply(uint32_t rb, uint32_t rc);

uint32_t divide(uint32_t rb, uint32_t rc);

uint32_t nand(uint32_t rb, uint32_t rc);

uint32_t map_seg(Seq_T mem, Seq_T unmappedIDs, uint32_t rc);

void unmap_seg(Seq_T unmappedIDs, uint32_t rc);

void output(uint32_t rc);

uint32_t input();

void loadprogram(Seq_T mem, uint32_t rb);

uint32_t loadval(uint32_t word);
