/*******************************************************************************
* Name: sudoku.c
*
*  Created by: Laura Hofmann and Cori Jacoby
*
*  Created on: 9.17.2014
*
*  Last modified by: Laura Hofmann and Cori Jacoby
*
*  Last modified on: 9.17.2014
*
*  Assignment: HW2 Part C Interface, Comp40
*
*  Summary: Implementation of Sudoku Checker
*******************************************************************************/

#include"sudoku.h"


/*******************************************************************************
*                               REPRESENTATIONS
*******************************************************************************/

const unsigned SUDOKU_DIM = 9;
const int PRESENT = 1;

/*******************************************************************************
*                                   CLIENT
*******************************************************************************/

int main (int argc, char *argv[]) {
        FILE *fileHandle = open_file (argc, argv);
        UArray2_T sudoku = read_in_data(fileHandle);
        fclose(fileHandle);
        check_sudoku(sudoku);
        UArray2_free(&sudoku);
        exit(0);
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
|  arguments:   program name and possibly the name of a portable graymap file
|  returns:     pointer to file
|  fail cases: 
|        - more than one argument given
|        - program is promised a file but no such file is delivered
|  note: function originally written for brightness.c by Laura Hofmann and 
|        Chad Myles
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
|  returns:     pointer to 2D array of pixel intensities from input file
|  fail cases:  
|        - file given is not a pnm file (causes pnmrdr to fail)
|        - file given is a pnm but not a portable graymap
|        - dimensions of file given are not those of a sudoku as defined in 
|          specifications
|        - denominator of file (maximum intensity of a pixel) is 9 as defined in
|          specifications
|  note:        function adapted from processData function written by Laura 
|               Hofmann and Chad Myles in brightness.c
|-------------------------------------------------------------------------------
*/
UArray2_T read_in_data(FILE *fileHandle) {
        struct Pnmrdr_T *reader = Pnmrdr_new(fileHandle);
        assert(reader != NULL);
        Pnmrdr_mapdata pnmData = Pnmrdr_data(reader);
        assert(pnmData.type == Pnmrdr_gray);
        assert((pnmData.width == SUDOKU_DIM) && (pnmData.height == SUDOKU_DIM));
        assert(pnmData.denominator == SUDOKU_DIM);
        UArray2_T sudoku = populate_sudoku(reader);
        Pnmrdr_free(&reader); 
        return sudoku;
}

/*------------------------------------------------------------------------------
|  populate_sudoku
|  purpose:     populates 2D array with ordered pixel intensities from input 
|               file
|  arguments:   a Pnmrdr reader
|  returns:     pointer to 2D array of pixel intensities from input file
|  fail cases:  
|        - reader is null
|-------------------------------------------------------------------------------
*/
UArray2_T populate_sudoku(Pnmrdr_T reader) {
        assert(reader != NULL);
        UArray2_T sudoku = UArray2_new(SUDOKU_DIM, SUDOKU_DIM, 
                                       sizeof(unsigned));
        UArray2_map_row_major(sudoku, read_pixel, reader);
        return sudoku;
}

/*------------------------------------------------------------------------------
|  check_sudoku
|  purpose:     checks that the given sudoku is correctly solved according to
|                specifications
|  arguments:   a sudoku as a UArray2_T
|  returns:     -
|  fail cases:  
|       - sudoku is null
|-------------------------------------------------------------------------------
*/
void check_sudoku (UArray2_T sudoku){
        assert(sudoku != NULL);
        check_cols(sudoku);
        check_rows(sudoku);
        check_squares(sudoku);
}

/*------------------------------------------------------------------------------
|  check_cols
|  purpose:     checks that each column in the given sudoku has exactly one of
|               each number from 1 to 9 inclusive
|  arguments:   a sudoku as a UArray2_T
|  returns:     -
|  fail cases:  
|       - sudoku is null
|-------------------------------------------------------------------------------
*/
void check_cols(UArray2_T sudoku) {
        assert(sudoku != NULL);
        for (unsigned i = 0; i < SUDOKU_DIM; i++) {
                Bit_T col_vals = Bit_new(SUDOKU_DIM + 1);
                for (unsigned j = 0; j < SUDOKU_DIM; j++) {
                        Bit_put(col_vals, *(unsigned *)UArray2_at(sudoku, i, j),
                                PRESENT);
                }
                if (!check_values(col_vals)) {
                        Bit_free(&col_vals);
                        UArray2_free(&sudoku);
                        exit(1);
                } else Bit_free(&col_vals);
        }
}

/*------------------------------------------------------------------------------
|  check_rows
|  purpose:     checks that each row in the given sudoku has exactly one of each
|               number from 1 to 9 inclusive
|  arguments:   a sudoku as a UArray2_T
|  returns:     -
|  fail cases:  
|       - sudoku is null
|-------------------------------------------------------------------------------
*/
void check_rows(UArray2_T sudoku) {
        for (unsigned j = 0; j < SUDOKU_DIM; j++) {
                Bit_T row_vals = Bit_new(SUDOKU_DIM + 1);
                for (unsigned i = 0; i < SUDOKU_DIM; i++) {
                        Bit_put(row_vals, *(unsigned *)UArray2_at(sudoku, i, j),
                                PRESENT);
                }
                if(!check_values(row_vals)){
                        Bit_free(&row_vals);
                        UArray2_free(&sudoku);
                        exit(1);
                } else Bit_free(&row_vals);
        }
}

/*------------------------------------------------------------------------------
|  check_squares
|  purpose:     checks that each square in the given sudoku has exactly one of 
|               each number from 1 to 9 inclusive
|  arguments:   a sudoku as a UArray2_T
|  returns:     -
|  fail cases:  
|       - sudoku is null
|-------------------------------------------------------------------------------
*/
void check_squares(UArray2_T sudoku) {
      assert(sudoku != NULL);
      for (int round = 0; round < 3; round++) {
              Bit_T sq_vals_1 = Bit_new(SUDOKU_DIM + 1);
              Bit_T sq_vals_2 = Bit_new(SUDOKU_DIM + 1);
              Bit_T sq_vals_3 = Bit_new(SUDOKU_DIM + 1);
              for (int j = (round * 3); j < ((round * 3) + 3); j++) {
                      for (int i = 0; i < 3; i++) {
                              Bit_put(sq_vals_1, 
                                      *(unsigned *)UArray2_at(sudoku, i, j), 1);
                              Bit_put(sq_vals_2, 
                                      *(unsigned *)UArray2_at(sudoku, 
                                                              (i + 3), j), 1);
                              Bit_put(sq_vals_3, 
                                      *(unsigned *)UArray2_at(sudoku, 
                                                              (i + 6), j), 1);
                      }
              }
              if (!check_values(sq_vals_1)) {
                      Bit_free(&sq_vals_1);
                      Bit_free(&sq_vals_2);
                      Bit_free(&sq_vals_3);
                      UArray2_free(&sudoku);
                      exit(1);
              } else Bit_free(&sq_vals_1);
              if (!check_values(sq_vals_2)) {
                      Bit_free(&sq_vals_2);
                      Bit_free(&sq_vals_3);
                      UArray2_free(&sudoku);;
                      exit(1);
              } else Bit_free(&sq_vals_2);
              if (!check_values(sq_vals_3)) {
                      Bit_free(&sq_vals_3);
                      UArray2_free(&sudoku);
                      exit(1);
              } else Bit_free(&sq_vals_3);
      }
}

/*------------------------------------------------------------------------------
|  check_values
|  purpose:     checks that just the numbers 1 - 9 inclusive are represented in 
|               the given bit vector
|  arguments:   a bit vector of length 10
|  returns:     0 if false, 1 if trueS
|  fail cases:
|       - numbers is null
|-------------------------------------------------------------------------------
*/
bool check_values(Bit_T numbers) {
        assert(numbers != NULL);
        if ((Bit_get(numbers, 0) == PRESENT) ||
            ((unsigned)Bit_count(numbers) != SUDOKU_DIM)) {                
                return 0; 
        }
        return 1;
}

/*------------------------------------------------------------------------------
|  read_pixel
|  purpose:     reads in the given pixel
|  arguments:   the matrix location of the pixel to be read as a pair of ints
|               and the sudoku as a UArray2_T, a pointer to any value, a closure
|  returns:     -
|  fail cases:  
|        - cl is null
|  note:       closure must be a Pnmrdr_T, but this cannot be easily asserted
|              if not, Pnmrdr should raise an exception
|-------------------------------------------------------------------------------
*/
void read_pixel(int i, int j, UArray2_T uarray2, void *value, void *cl) {
        assert(cl != NULL);
        *((unsigned *)value) = Pnmrdr_get(cl);
        (void)i;
        (void)j;
        (void)uarray2;

}

