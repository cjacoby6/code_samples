/*
 * Cori Jacoby
 * HW6
 * segments.h
 * 11/4/14
 *
 * Interface for segment implementation
 */

#include <stdio.h>

#include "segments.h"

uint32_t new_segment(int words, Seq_T storage, Seq_T unmappedIDs)
{
    	uint32_t newid;
	seg_array val;
        val = new_seg_array(words);

	if (Seq_length(unmappedIDs) != 0) {
	    	newid = (uint32_t)(uintptr_t)Seq_remlo(unmappedIDs);
		free_seg_array(Seq_put(storage, (int)newid, val));
	} else {
		newid = (uint32_t)Seq_length(storage);
		Seq_addhi(storage, val);
	}
	return newid;
}

void free_segment(uint32_t segID, Seq_T unmappedIDs)
{
	Seq_addhi(unmappedIDs, (void *)(uintptr_t)segID);
}

uint32_t get_segment_val(Seq_T mem, int seg, int offset)
{
        seg_array seg_val = Seq_get(mem, seg);
        return seg_val->array[offset];
}

void store_segment_val(Seq_T mem, int seg, int offset, uint32_t val)
{
        seg_array seg_value = Seq_get(mem, seg);
        seg_value->array[offset] = val;
}

void replace_segment_val(Seq_T mem, int id)
{
        seg_array prog = Seq_get(mem, id);
        seg_array newprog = new_seg_array(prog->length);
        
        for (int i = 0; i < newprog->length; i++) {
                newprog->array[i] = prog->array[i];
        }
                       
        seg_array oldprog = Seq_put(mem, 0, newprog);
        free_seg_array(oldprog);
}

seg_array new_seg_array(int words)
{
        seg_array seg_val = malloc(sizeof(*seg_val));

        seg_val->length = words;
        seg_val->array = malloc(sizeof(uint32_t) * words);
        for (int i = 0; i < words; i++) {
                seg_val->array[i] = 0;
        }
        return seg_val;
}

void free_seg_array(seg_array val)
{
        if(val->length == 0) {
                free(val);
        } else {
                free(val->array);
                free(val);
        }
}
