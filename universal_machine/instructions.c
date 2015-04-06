/*
 * Cori Jacoby
 * HW6
 * instructions.c
 * 11/4/14
 *
 * Interface for segment implementation
 */

#include "instructions.h"
#include "segments.h"

#define VAL_WIDTH 25

uint32_t cmov(uint32_t ra, uint32_t rb, uint32_t rc)
{
        if(rc != 0) {
                return rb;
        }
        else return ra;
}

uint32_t seg_load(Seq_T mem, uint32_t rb, uint32_t rc)
{
        return get_segment_val(mem, (int)rb, (int)rc);
}

void seg_store(Seq_T mem, uint32_t ra, uint32_t rb, uint32_t rc)
{
        store_segment_val(mem, (int)ra, (int)rb, rc);
}

uint32_t add(uint32_t rb, uint32_t rc)
{
        return (rb + rc);
}

uint32_t multiply(uint32_t rb, uint32_t rc)
{
        return (rb * rc);
}

uint32_t divide(uint32_t rb, uint32_t rc)
{
        return (rb / rc);
}

uint32_t nand(uint32_t rb, uint32_t rc)
{
        return ~(rb & rc);
}

uint32_t map_seg(Seq_T mem, Seq_T unmappedIDs, uint32_t rc)
{
        return (uint32_t)new_segment(rc, mem, unmappedIDs);
}

void unmap_seg(Seq_T unmappedIDs, uint32_t rc)
{
        free_segment((int)rc, unmappedIDs);
}

void output(uint32_t rc)
{
        assert(rc <= 255);
        printf("%c", rc);
}

uint32_t input()
{
        return (uint32_t)getchar();
}

void loadprogram(Seq_T mem, uint32_t rb)
{
        if (rb != 0) {
                replace_segment_val(mem, (int)rb);
        }
}

uint32_t loadval(uint32_t word)
{
        uint32_t val = (uint32_t)Bitpack_getu((uint64_t)word, VAL_WIDTH, 0);
        return val;
}
