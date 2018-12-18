/*******************************************************************************
* Name: bit2.h
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
*  Summary: Interface for implementation of a 2D bit array
*******************************************************************************/

#ifndef BIT2_INCLUDED
#define BIT2_INCLUDED
#include <bit.h>
#include <stdio.h>
#include <stdlib.h>
#include<assert.h>

/*******************************************************************************
*                            PUBLIC DECLARATIONS
*******************************************************************************/
#define T Bit2_T 

typedef struct T *T;

/*-----------------------------------------------------------------------------*
| Bit2_new
|       Purpose: creates an instance of Bit2
*-----------------------------------------------------------------------------*/
T Bit2_new (int dim1, int dim2); 

/*-----------------------------------------------------------------------------*
| Bit2_free
|       Purpose: frees the memory allocated for the given Bit2
*-----------------------------------------------------------------------------*/
void Bit2_free (T *Bit2);

/*-----------------------------------------------------------------------------*
| Bit2_width
|       Purpose: finds the width of the given Bit2
*-----------------------------------------------------------------------------*/
int Bit2_width(T Bit2);

/*-----------------------------------------------------------------------------*
| Bit2_height
|       Purpose: finds the height of the given Bit2
*-----------------------------------------------------------------------------*/
int Bit2_height(T Bit2);

/*-----------------------------------------------------------------------------*
| Bit2_put
|       Purpose: inserts the given value into the given Bit2 at the specified
|                matrix location
|       Note:    returns the value previously stored at the given location
*-----------------------------------------------------------------------------*/
int Bit2_put(T Bit2, int i, int j, int value);

/*-----------------------------------------------------------------------------*
| Bit2_get
|       Purpose: finds the element stored at the given location of the specified
|                Bit2
*-----------------------------------------------------------------------------*/
int Bit2_get(T Bit2, int i, int j);

/*-----------------------------------------------------------------------------*
| Bit2_map_col_major
|       Purpose: maps the given function onto every element of the given Bit2 in
|                col major order
*-----------------------------------------------------------------------------*/
void Bit2_map_col_major(Bit2_T bit2, void apply(int i, int j, Bit2_T bit2, 
                        int value, void *cl), void *cl);
                        
/*-----------------------------------------------------------------------------*
| Bit2_map_row_major
|       Purpose: Maps the given function onto every element of the given Bit2 in
|                row major order
*-----------------------------------------------------------------------------*/
void Bit2_map_row_major(T Bit2, void apply(int i, int j, Bit2_T a, int b,
                        void *p1), void *cl);

#undef T
#endif

