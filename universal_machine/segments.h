/*
 * Cori Jacoby
 * HW6
 * segments.h
 * 11/4/14
 *
 * Interface for segment implementation
 */

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <bitpack.h>
#include <uarray.h>
#include <seq.h>

typedef struct {
	int length;
	uint32_t *array;
} *seg_array;

/* returns the segment ID of a segment with the given size */
uint32_t new_segment(int words, Seq_T storage, Seq_T unmappedIDs);

/*
 *  frees the value associated with the given segment and marks it as unmapped 
 */
void free_segment(uint32_t segID, Seq_T unmappedIDs);

/* duplicates the given segment and returns the segment ID of the new segment */
uint32_t dup_segment(uint32_t segID, Seq_T storage, Seq_T unmappedIDs);

/* adds the given segment to the given sequence of unmapped segments */
void unmap_segment(uint32_t segID, Seq_T unmapped_segs);

/* 
 * returns the word value from the  given memory using the given segment and 
 * offset as ints
 */
uint32_t get_segment_val(Seq_T mem, int segment, int offset);

/* 
 * stores the given word value into the given memory using the given segment 
 * and offset value as ints
 */
void store_segment_val(Seq_T mem, int seg, int offset, uint32_t val);

/*
 * replaces the value in the zeroth segment with the corresponding value to the
 * given original segment, identified by its uint32_t, in the given memory
 */
void replace_segment_val(Seq_T mem, int id);

/* returns a seg_array initialized for the given number of words */
seg_array new_seg_array(int words);

/* frees a seg array */
void free_seg_array(seg_array val);
