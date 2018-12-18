/*******************************************************************************
* Name: bit2.c
*
*  Created by: Laura Hofmann and Cori Jacoby
*
*  Created on: 9.15.2014
*
*  Last modified by: Laura Hofmann and Cori Jacoby
*
*  Last modified on: 9.22.2014
*
*  Assignment: HW2 Design, Comp40
*
*  Summary: Implementation of a 2D bit array 
*******************************************************************************/

#include <bit2.h>

/*******************************************************************************
*                           PRIVATE REPRESENTATIONS
*******************************************************************************/

struct Bit2_T {
        int width;
        int height;
        Bit_T bitmap;
};

/*******************************************************************************
*                            PRIVATE DECLARATIONS
*******************************************************************************/

/*-----------------------------------------------------------------------------*
| get_index
|       Purpose: finds the index in the representation that corresponds to 
|                bit [i,j] in the 2D bit array 
*-----------------------------------------------------------------------------*/
int get_index(int i, int j, int width);

/*-----------------------------------------------------------------------------*
| get_i
|       Purpose: finds the i coordinate of the bit stored at the given index
|                in the representation
*-----------------------------------------------------------------------------*/
int get_i(int index, int width);

/*-----------------------------------------------------------------------------*
| get_j
|       Purpose: finds the j coordinate of the bit stored at the given index
|                in the representation
*-----------------------------------------------------------------------------*/
int get_j(int index, int width);


/*******************************************************************************
*                       DEFINITIONS (PUBLIC FUNCTIONS)
*******************************************************************************/

/*-----------------------------------------------------------------------------*
| Bit2_new
|       Purpose:   creates an instance of Bit2
|       Arguments: the dimensions of the 2D bit array to be created as ints
|       Returns:   a pointer to a 2D bit array
|       Fail cases:
|               - either of the dimensions passed is not positive
*-----------------------------------------------------------------------------*/
Bit2_T Bit2_new (int dim1, int dim2) {
        assert((dim1 > 0) && (dim2 > 0));
        int length = dim1 * dim2;
        Bit2_T new_bitmap = malloc(sizeof(*new_bitmap));
        new_bitmap->width = dim1;
        new_bitmap->height = dim2;
        new_bitmap->bitmap = Bit_new(length);
        return new_bitmap;
}

/*-----------------------------------------------------------------------------*
| Bit2_free
|       Purpose:   frees the memory allocated for the given Bit2 
|       Arguments: a pointer to a pointer to a 2D bit array
|       Returns:   -
|       Fail cases:
|               - the pointer to the pointer to the 2D bit array is null
|               - the pointer to the 2D bit array is null
*-----------------------------------------------------------------------------*/
void Bit2_free (Bit2_T *bit2) {
        assert((bit2 != NULL) && (*bit2 != NULL));
        Bit2_T temp = *bit2;
        Bit_T bit_array = temp->bitmap;
        Bit_free(&bit_array);
        free(temp);
}

/*-----------------------------------------------------------------------------*
| Bit2_width
|       Purpose:   finds the width of the given Bit2
|       Arguments: a pointer to a 2D bit array
|       Returns:   the width of the 2D bit array as an int
|       Fail cases:
|               - the pointer to the 2D bit array is null
*-----------------------------------------------------------------------------*/
int Bit2_width(Bit2_T bit2) { 
        assert(bit2 != NULL);
        return bit2->width;
}

/*-----------------------------------------------------------------------------*
| Bit2_height
|       Purpose:   finds the height of the given Bit2
|       Arguments: a pointer to a 2D bit array
|       Returns:   the height of the 2D bit array as an int
|       Fail cases:
|               - the pointer to the 2D bit array is null
*-----------------------------------------------------------------------------*/
int Bit2_height(Bit2_T bit2) {
        assert(bit2 != NULL);
        return bit2->height;
}

/*-----------------------------------------------------------------------------*
| Bit2_put
|       Purpose:   Inserts the given value into the given Bit2 at the specified
|                  matrix location
|       Arguments: a pointer to the 2D array, the matrix location as a pair of 
|                  ints, the value to be inserted as an int
|       Returns:   returns the value previously stored at the given location as
|                  an int
|       Fail cases:
|               - the pointer to the 2D bit array is null
|               - the given matrix location lies outside the bounds of the 2D
|                 bit array
|               - the value to be inserted is not 0 or 1
*-----------------------------------------------------------------------------*/
int Bit2_put(Bit2_T bit2, int i, int j, int value) {
        assert(bit2 != NULL);
        assert((i >= 0 && i < Bit2_width(bit2)) && 
               (j >= 0 && j < Bit2_height(bit2)));
        assert(value == 0 || value == 1);
        int index = get_index(i, j, Bit2_height(bit2));
        return (Bit_put(bit2->bitmap, index, value));
}

/*-----------------------------------------------------------------------------*
| Bit2_get
|       Purpose:   Finds the element stored at the given location of the 
|                  specified Bit2
|       Arguments: a pointer to the 2D bit array, the matrix location as a pair
|                  of ints
|       Returns:   the value stored at the given matrix location of the 2D bit 
|                  array as an int
|       Fail cases:
|               - the pointer to the 2D bit array is null
|               - the given matrix location lies outside the bounds of the 2D
|                 bit array
*-----------------------------------------------------------------------------*/
int Bit2_get(Bit2_T bit2, int i, int j) {
        assert(bit2 != NULL);
        assert((i >= 0 && i < Bit2_width(bit2)) &&
               (j >= 0 && j < Bit2_height(bit2)));
        int index = get_index(i, j, Bit2_height(bit2));
        return Bit_get(bit2->bitmap, index);
}

/*-----------------------------------------------------------------------------*
| Bit2_map_col_major
|       Purpose:   maps the given function onto every element of the given Bit2 
|                  in col major order
|       Arguments: a pointer to the 2D bit array, a pointer to a void apply 
|                  function, the closure as a void *
|       Returns:   -
|       Fail cases:
|               - the pointer to the 2D bit array is null
|               - the pointer to the apply function is null
*-----------------------------------------------------------------------------*/
void Bit2_map_col_major(Bit2_T bit2, void apply(int i, int j, Bit2_T bit2, 
                        int value, void *cl), void *cl) {
        assert(bit2 != NULL);
        assert(apply != NULL);
        int index;
        int i;
        int j;
        for (index = 0; index < Bit_length(bit2->bitmap); index++) {
                i = get_i(index, Bit2_height(bit2));
                j = get_j(index, Bit2_height(bit2));
                apply(i, j, bit2, Bit2_get(bit2, i, j), cl);
        }
}

/*-----------------------------------------------------------------------------*
| Bit2_map_row_major
|       Purpose:   maps the given function onto every element of the given Bit2 
|                  in row major order
|       Arguments: a pointer to the 2D bit array, a pointer to a void apply 
|                  function, the closure as a void *
|       Returns:   -
|       Fail cases:
|               - the pointer to the 2D bit array is null
|               - the pointer to the apply function is null
*-----------------------------------------------------------------------------*/
void Bit2_map_row_major(Bit2_T bit2, void apply(int i, int j, Bit2_T bit2, 
                        int value, void *cl), void *cl){
        assert(bit2 != NULL);
        assert(apply != NULL);
        int i;
        int j;
        for (j = 0; j < Bit2_height(bit2); j++) {
                for (i = 0; i < Bit2_width(bit2); i++) { 
                        apply(i, j, bit2, Bit2_get(bit2, i, j), cl);
                };
        };
}

/*******************************************************************************
*                        DEFINITIONS (PRIVATE FUNCTIONS)
*******************************************************************************/

/*-----------------------------------------------------------------------------*
| get_index
|       Purpose:    finds the index in the representation that corresponds to 
|                   bit [i,j] in the 2D bit array 
|       Arguments:  the matrix location as a pair of ints, the height of the 2D
|                   bit array as an int
|       Returns:    the index in the representation as an int
|       Fail cases: none, as get_index can only be called by parent functions 
|                   which ensure get_index only receives valid arguments
*-----------------------------------------------------------------------------*/
int get_index(int i, int j, int height) {
        int index = i * height + j;
        return index;
}

/*-----------------------------------------------------------------------------*
| get_i
|       Purpose:    finds the i coordinate of the bit stored at the given 
|                   index in the representation
|       Arguments:  the index in the representation as an int, the height of 
|                   the 2D bit array as an int
|       Returns:    the i coordinate of the bit stored at the given index in the
|                   representation as an int
|       Fail cases: none, as get_i can only be called by parent functions 
|                   which ensure get_i only receives valid arguments
*-----------------------------------------------------------------------------*/
int get_i (int index, int height) {
        return index / height;
}

/*-----------------------------------------------------------------------------*
| get_j
|       Purpose:    finds the j coordinate of the bit stored at the given 
|                   index in the representation
|       Arguments:  the index in the representation as an int, the height of 
|                   the 2D bit array as an int
|       Returns:    the j coordinate of the bit stored at the given index in the
|                   representation as an int
|       Fail cases: none, as get_j can only be called by parent functions 
|                   which ensure get_j only receives valid arguments
*-----------------------------------------------------------------------------*/
int get_j (int index, int height) {
        int i = index / height;
        return (index - i * height);
}

