/*******************************************************************************
* Name: sudoku.h
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
*  Summary: Header for Sudoku Checker
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include<setjmp.h>
#include <stdbool.h>
#include"pnmrdr.h"
#include"uarray2.h"
#include<bit.h>
#include<assert.h>

/*******************************************************************************-
*                              PUBLIC DECLARATIONS
*******************************************************************************/

/*------------------------------------------------------------------------------
| open_file
|  purpose:     opens input file
|-------------------------------------------------------------------------------
*/
FILE *open_file(int argc, char *argv[]);

/*------------------------------------------------------------------------------
|  read_in_data
|  purpose:     reads in data from input file
|-------------------------------------------------------------------------------
*/
UArray2_T read_in_data(FILE *fileHandle);

/*------------------------------------------------------------------------------
|  populate_sudoku
|  purpose:     populates 2D array with ordered pixel intensities from input 
|               file
|-------------------------------------------------------------------------------
*/
UArray2_T populate_sudoku (Pnmrdr_T reader);

/*------------------------------------------------------------------------------
|  check_sudoku
|  purpose:     checks that the given sudoku is correctly solved according to
                specifications
|-------------------------------------------------------------------------------
*/
void check_sudoku(UArray2_T sudoku);

/*------------------------------------------------------------------------------
|  check_cols
|  purpose:     checks that each column in the given sudoku has exactly one of
|               each number from 1 to 9 inclusive
|-------------------------------------------------------------------------------
*/
void check_cols(UArray2_T sudoku);

/*------------------------------------------------------------------------------
|  check_rows
|  purpose:     checks that each row in the given sudoku has exactly one of each
|               number from 1 to 9 inclusive
|-------------------------------------------------------------------------------
*/
void check_rows(UArray2_T sudoku);

/*------------------------------------------------------------------------------
|  check_squares
|  purpose:     checks that each square in the given sudoku has exactly one of 
|               each number from 1 to 9 inclusive
|-------------------------------------------------------------------------------
*/
void check_squares(UArray2_T sudoku);

/*------------------------------------------------------------------------------
|  check_values
|  purpose:     checks that just the numbers 1 - 9 inclusive are present in the
|               given bit vector
|-------------------------------------------------------------------------------
*/
bool check_values(Bit_T numbers);

/*------------------------------------------------------------------------------
|  read_pixel
|  purpose:     reads in the given pixel
|-------------------------------------------------------------------------------
*/
void read_pixel(int i, int j, UArray2_T uarray2, void *value, void *cl);

