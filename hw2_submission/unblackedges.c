/*******************************************************************************
* Name: unblackedges.c
*
*  Created by: Laura Hofmann and Cori Jacoby
*
*  Created on: 9.20.2014
*
*  Last modified by: Laura Hofmann and Cori Jacoby
*
*  Last modified on: 9.22.2014
*
*  Assignment: HW2 Part D Implementation, Comp40
*
*  Summary: Implementation for unblackedges
*******************************************************************************/

#include <unblackedges.h>

/*******************************************************************************
*                               REPRESENTATIONS
*******************************************************************************/

const int WHITE = 0;
const int BLACK = 1;


/*******************************************************************************
*                                   CLIENT
*******************************************************************************/

int main (int argc, char *argv[]) {
        FILE *fileHandle = open_file(argc, argv);
        Bit2_T image = read_in_data(fileHandle);
        fclose(fileHandle);
        image = unblack_edges(image);
        print_pbm(image);
        Bit2_free(&image);
        return 0;
}

/*******************************************************************************
*                                DECLARATIONS
*******************************************************************************/

/*
*       Note on fail cases and asserts: 
*               some functions list fail cases that they do not directly assert
*               because they themselves do not process the argument that will
*               cause them to fail. Instead, the assertion is left to the child
*               functions that do process the argument.
*/

/*------------------------------------------------------------------------------
|  open_file
|  purpose:     opens input file 
|  arguments:   program name and possibly the name of a portable bitmap file
|  returns:     pointer to file
|  fail cases:
|        - more than one argument given
|        - program is promised a file but no such file is delivered
|  note:        adapted from function originally written for brightness.c by
|               Laura Hofmann and Chad Myles
|-------------------------------------------------------------------------------
*/
FILE *open_file(int argc, char *argv[]) {
        assert (argc <= 2);
        FILE *fileHandle = NULL;
        if (argc == 1) { 
                fileHandle = stdin;
        } else {
                fileHandle = fopen(argv[1], "r"); 
        }
        assert (fileHandle != NULL);
        return fileHandle;
}

/*------------------------------------------------------------------------------
|  read_in_data
|  purpose:     reads in data from input file
|  arguments:   pointer to input file
|  returns:     pointer to 2D bit array
|  fail cases:  
|        - Pnmrdr_new fails
|        - file given is not a pnm file (causes pnmrdr to fail)
|        - file given is a pnm but not a portable bitmap
|  note:        function adapted from read_in_data function written by Laura 
|               Hofmann and Cori Jacoby in sudoku.c
|-------------------------------------------------------------------------------
*/
Bit2_T read_in_data(FILE *fileHandle) {
        struct Pnmrdr_T *reader = Pnmrdr_new(fileHandle);
        assert(reader != NULL);
        Pnmrdr_mapdata pnmData = Pnmrdr_data(reader);
        assert(pnmData.type == Pnmrdr_bit);
        Bit2_T image = populate_image(reader, pnmData);
        Pnmrdr_free(&reader);
        return image;
}

/*------------------------------------------------------------------------------
|  populate_image
|  purpose:     stores input in 2D bit array
|  arguments:   a Pnmrdr reader and data from the header of the input file as a
|               Pnmrdr_mapdata struct
|  returns:     pointer to 2D bit array
|  fail cases:  
|       - reader is null
|-------------------------------------------------------------------------------
*/
Bit2_T populate_image(Pnmrdr_T reader, Pnmrdr_mapdata pnmData) {
        Bit2_T image = Bit2_new(pnmData.width, pnmData.height);
        Bit2_map_row_major(image, read_bit, reader);
        return image;
}

/*------------------------------------------------------------------------------
|  unblack_edges
|  purpose:     removes black edges from bitmap
|  arguments:   a pointer to the image to be unblacked as a Bit2_T
|  returns:     a pointer to the bitmap with unblacked edges as a Bit2_T
|  fail cases:  
|       - image is null 
|-------------------------------------------------------------------------------
*/
Bit2_T unblack_edges(Bit2_T image){
        Stack_T unblacked_bits = Stack_new();
        unblack_perimeter(image, unblacked_bits);
        unblack_neighbors(image, unblacked_bits);
        Stack_free(&unblacked_bits);
        return image;
}

/*------------------------------------------------------------------------------
|  print_pbm
|  purpose:     prints a portable bitmap image to stdout
|  arguments:   a pointer to the bitmap to be printed
|  returns:     -
|  fail cases:  
|       - image is null 
|-------------------------------------------------------------------------------
*/
void print_pbm(Bit2_T bitmap) {
        fprintf(stdout, "P1\n%u %u\n", Bit2_width(bitmap), Bit2_height(bitmap));
        Bit2_map_row_major(bitmap, print_bit, NULL);
}

/*------------------------------------------------------------------------------
|  unblack_perimeter
|  purpose:     unblacks black bits on perimeter of image and adds to stack of 
|               unblacked edge bits
|  arguments:   the image to be unblacked as a Bit2_T, a pointer to the stack
|               of unblacked black edge bits
|  returns:     -
|  fail cases:
|       - image is null
|-------------------------------------------------------------------------------
*/
void unblack_perimeter(Bit2_T image, Stack_T unblacked_bits) {
        for (int i = 0; i < Bit2_width(image); i++) {
                Matrix_loc min_bit = malloc(sizeof(struct Matrix_loc));
                Matrix_loc max_bit = malloc(sizeof(struct Matrix_loc));
                min_bit->j = 0;
                max_bit->j = (Bit2_height(image) - 1);
                min_bit->i = i;
                max_bit->i = i;
                add_to_stack(image, unblacked_bits, is_black(image, min_bit));
                add_to_stack(image, unblacked_bits, is_black(image, max_bit));
        }
        for (int j = 1; j < Bit2_height(image)-1; j++) {
                Matrix_loc min_bit = malloc(sizeof(struct Matrix_loc));
                Matrix_loc max_bit = malloc(sizeof(struct Matrix_loc));
                min_bit->i = 0;
                max_bit->i = (Bit2_width(image) - 1);
                min_bit->j = j;
                max_bit->j = j;
                add_to_stack(image, unblacked_bits, is_black(image, min_bit));
                add_to_stack(image, unblacked_bits, is_black(image, max_bit));
        }
}

/*------------------------------------------------------------------------------
|  unblack_neighbors
|  purpose:     unblacks black bits that are neighbors of formerly black edge 
|               pixels
|  arguments:   the image to be unblacked as a Bit2_T, a pointer to the stack
|               of unblacked black edge bits
|  returns:     -
|  fail cases:
|       - image is null
|       - there is a null pointer on the stack
|-------------------------------------------------------------------------------
*/
void unblack_neighbors(Bit2_T image, Stack_T unblacked_bits) {
        Matrix_loc current_bit;
        while (Stack_empty(unblacked_bits) == 0) {
                current_bit = Stack_pop(unblacked_bits);
                add_black_neighbors(image, unblacked_bits, current_bit);
                free(current_bit);
        }
}

/*------------------------------------------------------------------------------
|  add_black_neighbors
|  purpose:     unblacks black bits that are neighbors of a given formerly black
|               edge pixel
|  arguments:   the image to be unblacked as a Bit2_T, a pointer to the stack 
|               of unblacked black edge bits, the matrix location of the bit as 
|               a Matrix_loc
|  returns:     -
|  fail cases:  
|       - image is null
|       - loc is null
|-------------------------------------------------------------------------------
*/
void add_black_neighbors(Bit2_T image, Stack_T unblacked_bits, Matrix_loc loc) {
        add_to_stack(image, unblacked_bits, black_up(image, loc));
        add_to_stack(image, unblacked_bits, black_right(image, loc));
        add_to_stack(image, unblacked_bits, black_down(image, loc));
        add_to_stack(image, unblacked_bits, black_left(image, loc));
}

/*------------------------------------------------------------------------------
|  is_black
|  purpose:     checks if the given bit is black
|  arguments:   the image as a Bit2_T, the matrix location of the bit as a 
|               Matrix_loc
|  returns:     the matrix location if the bit is black, null if it is white.
|  fail cases:  
|       - image is null
|       - loc is null
|-------------------------------------------------------------------------------
*/
Matrix_loc is_black(Bit2_T image, Matrix_loc loc) {
        assert(loc != NULL);
        int i = loc->i;
        int j = loc->j;
        if (Bit2_get(image, i, j) == BLACK) {
                return loc;
        } else {
                free(loc);
                return NULL;
        }
}

/*------------------------------------------------------------------------------
|  add_to_stack
|  purpose:     unblacks black bit and adds it to stack of unblacked edge bits
|  arguments:   the image as a Bit2_T, the matrix location of the bit as a 
|               Matrix_loc
|  returns:     -
|  fail cases:  
|       - image is null
|-------------------------------------------------------------------------------
*/
void add_to_stack(Bit2_T image, Stack_T unblacked_bits, Matrix_loc loc) {
        if (loc != NULL) {
                michael_jacksonify(image, loc);
                Stack_push(unblacked_bits, loc);
        }
}

/*------------------------------------------------------------------------------
|  michael_jacksonify
|  purpose:      whites a black bit
|  arguments:    the image as a Bit2_T, the matrix location of the bit to be
|                unblacked as a Matrix_loc
|  returns:      -
|  fail cases:
|       - image is null
|-------------------------------------------------------------------------------
*/
void michael_jacksonify(Bit2_T image, Matrix_loc loc) {
        assert(loc != NULL);
        int i = loc->i;
        int j = loc->j;
        Bit2_put(image, i, j, WHITE);
}

/*------------------------------------------------------------------------------
|  valid_black_neighbor                                     
|  purpose:     checks whether a given neighbor bit is both black and in the
|               array
|  arguments:   the image as a Bit2_T, the matrix location of the bit as a 
|               Matrix_loc
|  returns:     null if invalid, otherwise the matrix location of the valid 
|               black neighbor
|  fail cases:  
|       - image is null
|       - loc is null
|-------------------------------------------------------------------------------
*/
Matrix_loc valid_black_neighbor (Bit2_T image, Matrix_loc loc) {
        if (in_map(image, loc) == 0) {
                free(loc);
                return NULL;
        } else return is_black(image, loc);
}

/*------------------------------------------------------------------------------
|  in_map
|  purpose:     checks whether a given matrix location is within the bounds of
|               the 2D bit array
|  arguments:   a pointer to the bitmap, the matrix location as a Matrix_loc
|  returns:     1 if location is in bitmap, 0 otherwise
|  fail cases:  
|       - bitmap is null
|       - loc is null
|-------------------------------------------------------------------------------
*/
bool in_map(Bit2_T bitmap, Matrix_loc loc) {
        assert(loc != NULL);
        int i = loc->i;
        int j = loc->j;
        if ((i >= 0 && i < Bit2_width(bitmap)) &&
            (j >= 0 && j < Bit2_height(bitmap))) {
                return 1;
        } else return 0;
}

/*------------------------------------------------------------------------------
|  print_bit
|  purpose:    prints the given bit
|  arguments:  the matrix location of the bit to be printed as a pair of ints,
|              the bitmap as a Bit2_T, any integer value
|  returns:    -
|  fail cases:
|       - bit2 is null
|  note:       any integer value can be passed as bit, as it is not actually
|              utilized by the function
|-------------------------------------------------------------------------------
*/
void print_bit(int i, int j, Bit2_T bit2, int bit, void *cl) {
        (void)cl;
        bit = Bit2_get(bit2, i, j);
        fprintf(stdout, "%u", bit);
        if (i == (Bit2_width(bit2) -1)) {
                fprintf(stdout, "\n");
        } else fprintf(stdout, " ");
}

/*------------------------------------------------------------------------------
|  read_bit
|  purpose:    reads in the given bit
|  arguments:  the matrix location of the bit to be read as a pair of ints and 
|              the bitmap as a Bit2_T, any integer value, a closure
|  returns:    -
|  fail cases: 
|       - cl is null
|  note:       closure must be a Pnmrdr_T, but this cannot be easily asserted
|              if not, Pnmrdr should raise an exception
|-------------------------------------------------------------------------------
*/
void read_bit(int i, int j, Bit2_T bit2, int bit, void *cl) {
        assert(cl != NULL);
        bit = Pnmrdr_get(cl);
        Bit2_put(bit2, i, j, bit);
}

/*------------------------------------------------------------------------------
| black_up
|  purpose:    checks whether the bit above the given bit (up neighbor) is a
|              valid black bit
|  arguments:  the image as a Bit2_T, the matrix location of the given bit as a
|              Matrix_loc
|  returns:    -
|  fail cases: 
|              - image is null
|-------------------------------------------------------------------------------
*/
Matrix_loc black_up(Bit2_T image, Matrix_loc loc) {
        assert(loc != NULL);
        Matrix_loc up_loc = malloc(sizeof(struct Matrix_loc));
        up_loc->i = loc->i;
        up_loc->j = loc->j - 1;
        return valid_black_neighbor(image, up_loc);
}

/*------------------------------------------------------------------------------
| black_down
|  purpose:    checks whether the bit below the given bit (down neighbor) is a
|              valid black bit
|  arguments:  the image as a Bit2_T, the matrix location of the given bit as a
|              Matrix_loc
|  returns:    -
|  fail cases: 
|             - image is null
|-------------------------------------------------------------------------------
*/
Matrix_loc black_down(Bit2_T image, Matrix_loc loc) {
        assert(loc != NULL);
        Matrix_loc down_loc = malloc(sizeof(struct Matrix_loc));
        down_loc->i = loc->i;
        down_loc->j = loc->j + 1;
        return valid_black_neighbor(image, down_loc);
}

/*------------------------------------------------------------------------------
| black_left
|  purpose:    checks whether the bit to the left of the given bit (left 
|              neighbor) is a valid black bit
|  arguments:  the image as a Bit2_T, the matrix location of the given bit as a
|              Matrix_loc
|  returns:    -
|  fail cases: 
|             - image is null
|-------------------------------------------------------------------------------
*/
Matrix_loc black_left(Bit2_T image, Matrix_loc loc) {
        assert(loc != NULL);
        Matrix_loc left_loc = malloc(sizeof(struct Matrix_loc));
        left_loc->i = loc->i - 1;
        left_loc->j = loc->j;
        return valid_black_neighbor(image, left_loc);
}

/*------------------------------------------------------------------------------
| black_right
|  purpose:    checks whether the bit to the right of the given bit (right 
|              neighbor) is a valid black bit
|  arguments:  the image as a Bit2_T, the matrix location of the given bit as a
|              Matrix_loc
|  returns:    -
|  fail cases: 
|             - image is null
|-------------------------------------------------------------------------------
*/
Matrix_loc black_right(Bit2_T image, Matrix_loc loc) {
        assert(loc != NULL);
        Matrix_loc right_loc = malloc(sizeof(struct Matrix_loc));
        right_loc->i = loc->i + 1;
        right_loc->j = loc->j;
        return valid_black_neighbor(image, right_loc);
}

