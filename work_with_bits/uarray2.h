/*******************************************************************************
* Name: uarray2.h
*
*  Created by: Laura Hofmann and Cori Jacoby
*
*  Created on: 9.13.2014
*
*  Last modified by: Laura Hofmann and Cori Jacoby
*
*  Last modified on: 9.22.2014
*
*  Assignment: HW2 Design, Comp40
*
*  Summary: Interface for implementation of a 2D unboxed array 
*******************************************************************************/

#ifndef UARRAY2_INCLUDED
#define UARRAY2_INCLUDED
#include <uarray.h> 
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/*******************************************************************************
*                            PUBLIC DECLARATIONS
*******************************************************************************/
#define T UArray2_T 

typedef struct T *T;

/*-----------------------------------------------------------------------------*
| UArray2_new
|       Purpose: creates an instance of UArray2
*-----------------------------------------------------------------------------*/

T UArray2_new (int dim1, int dim2, size_t element_size);

/*-----------------------------------------------------------------------------*
| UArray2_free
|       Purpose: frees the memory allocated for the given UArray2
*-----------------------------------------------------------------------------*/
void UArray2_free (T *uarray2);

/*-----------------------------------------------------------------------------*
| UArray2_width
|       Purpose: finds the width of the given UArray2
*-----------------------------------------------------------------------------*/
int UArray2_width(T uarray2);

/*-----------------------------------------------------------------------------*
| UArray2_height
|       Purpose: finds the height of the given UArray2
|-----------------------------------------------------------------------------*/
int UArray2_height(T uarray2);

/*-----------------------------------------------------------------------------*
| UArray2_size
|       Purpose: finds the size of each element of the given UArray2
*-----------------------------------------------------------------------------*/
int UArray2_size(T uarray2);

/*-----------------------------------------------------------------------------*
| UArray2_at
|       Purpose: finds the element stored at the specified matrix location of 
|                the given UArray2
*-----------------------------------------------------------------------------*/
void *UArray2_at(T uarray2, int i, int j);

/*-----------------------------------------------------------------------------*
| UArray2_map_col_major
|       Purpose: maps a function onto every element of the given UArray2 in col
|                major order
*-----------------------------------------------------------------------------*/
void UArray2_map_col_major(T uarray2, void apply(int i, int j, 
                           UArray2_T uarray2, void *value, void *cl), void *cl);

/*-----------------------------------------------------------------------------*
| UArray2_map_row_major
|       Purpose: maps a function onto every element of the given UArray2 in row
|                major order
*-----------------------------------------------------------------------------*/
void UArray2_map_row_major(T uarray2, void apply(int i, int j, 
                           UArray2_T uarray2, void *value, void *cl), void *cl);

#undef T
#endif