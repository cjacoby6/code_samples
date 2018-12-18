/*******************************************************************************
* Name: unblackedges.h
*
*  Created by: Laura Hofmann and Cori Jacoby
*
*  Created on: 9.19.2014
*
*  Last modified by: Laura Hofmann and Cori Jacoby
*
*  Last modified on: 9.22.2014
*
*  Assignment: HW2 Part D Interface, Comp40
*
*  Summary: Interface for unblackedges
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stack.h>
#include <stdbool.h>
#include <assert.h>
#include "pnmrdr.h"
#include "bit2.h"

/*******************************************************************************
*                               REPRESENTATIONS
*******************************************************************************/

/*------------------------------------------------------------------------------
|  Matrix_location
|  purpose:     stores a matrix location [i,j] as a pair of unsigned ints
|  note:        since there is no Matrix_loc constructor, the full defintion 
|               has been made available in this file to allow the client to
|               view its elements
|-------------------------------------------------------------------------------
*/

typedef struct Matrix_loc *Matrix_loc;

struct Matrix_loc {
        unsigned int i;
        unsigned int j;
};



/*******************************************************************************-
*                               PUBLIC DECLARATIONS
*******************************************************************************/

/*------------------------------------------------------------------------------
|  open_file
|  purpose:     opens input file 
|-------------------------------------------------------------------------------
*/
FILE *open_file(int argc, char *argv[]);

/*------------------------------------------------------------------------------
|  read_in_data
|  purpose:     reads in data from input file
|-------------------------------------------------------------------------------
*/
Bit2_T read_in_data(FILE *fileHandle);

/*------------------------------------------------------------------------------
|  populate_image
|  purpose:     stores input in 2D bit array
|-------------------------------------------------------------------------------
*/
Bit2_T populate_image(Pnmrdr_T reader, Pnmrdr_mapdata pnmData);

/*------------------------------------------------------------------------------
|  unblack_edges
|  purpose:     removes black edges from bitmap
|-------------------------------------------------------------------------------
*/
Bit2_T unblack_edges(Bit2_T image);

/*------------------------------------------------------------------------------
|  print_pbm
|  purpose:     prints a portable bitmap image to stdout
|-------------------------------------------------------------------------------
*/
void print_pbm(Bit2_T bitmap);

/*------------------------------------------------------------------------------
|  unblack_perimeter
|  purpose:     unblacks black bits on perimeter of image and adds to stack of 
|               unblacked edge bits
|------------------------------------------------------------------------------
*/
void unblack_perimeter(Bit2_T image, Stack_T unblacked_bits);

/*------------------------------------------------------------------------------
|  unblack_neighbors
|  purpose:     unblacks black bits that are neighbors of formerly black edge 
|               pixels
|------------------------------------------------------------------------------
*/
void unblack_neighbors(Bit2_T image, Stack_T unblacked_bits);

/*------------------------------------------------------------------------------
|  add_black_neighbors
|  purpose:     unblacks black bits that are neighbors of a given formerly black 
|               edge pixel
|-------------------------------------------------------------------------------
*/
void add_black_neighbors(Bit2_T image, Stack_T unblacked_bits, Matrix_loc loc);

/*------------------------------------------------------------------------------
|  is_black
|  purpose:     checks if a given bit is black
|-------------------------------------------------------------------------------
*/
Matrix_loc is_black(Bit2_T image, Matrix_loc loc);

/*------------------------------------------------------------------------------
|  add_to_stack
|  purpose:     unblacks black bit and adds it to stack of unblacked edge bits
|-------------------------------------------------------------------------------
*/
void add_to_stack(Bit2_T image, Stack_T unblacked_bits, Matrix_loc loc);
        
/*------------------------------------------------------------------------------
|  michael_jacksonify
|  purpose:      whites a black bit
|-------------------------------------------------------------------------------
*/
void michael_jacksonify(Bit2_T image, Matrix_loc loc);

/*------------------------------------------------------------------------------
|  valid_black_neighbor                                     
|  purpose:     checks whether a given neighbor bit is both black and in the
|               array
|-------------------------------------------------------------------------------
*/
Matrix_loc valid_black_neighbor (Bit2_T image, Matrix_loc loc);

/*------------------------------------------------------------------------------
|  in_map                                       
|  purpose:     checks whether a given matrix location is within the bounds of
|               the 2D bit array
|-------------------------------------------------------------------------------
*/
bool in_map(Bit2_T bitmap, Matrix_loc loc);

/*------------------------------------------------------------------------------
|  print_bit                                    
|  purpose:    prints the given bit
|-------------------------------------------------------------------------------
*/
void print_bit(int i, int j, Bit2_T bit2, int bit, void *cl);

/*------------------------------------------------------------------------------
|  read_bit                                    
|  purpose:    reads in given bit
|-------------------------------------------------------------------------------
*/
void read_bit(int i, int j, Bit2_T bit2, int bit, void *cl);

/*------------------------------------------------------------------------------
|  black_up                                  
|  purpose:    checks whether the bit above the given bit (up neighbor) is a
|              valid black bit
|-------------------------------------------------------------------------------
*/
Matrix_loc black_up(Bit2_T image, Matrix_loc loc);

/*------------------------------------------------------------------------------
| black_down
|  purpose:    checks whether the bit below the given bit (down neighbor) is a
|              valid black bit
|-------------------------------------------------------------------------------
*/
Matrix_loc black_down(Bit2_T image, Matrix_loc loc);

/*------------------------------------------------------------------------------
|  black_left
|  purpose:    checks whether the bit to the left of the given bit (left 
|              neighbor) is a valid black bit
|-------------------------------------------------------------------------------
*/
Matrix_loc black_left(Bit2_T image, Matrix_loc loc);

/*------------------------------------------------------------------------------
| black_right
|  purpose:    checks whether the bit to the right of the given bit (right 
|              neighbor) is a valid black bit
|-------------------------------------------------------------------------------
*/
Matrix_loc black_right(Bit2_T image, Matrix_loc loc);

