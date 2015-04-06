/*******************************************************************************
* Name: uarray2.c
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
*  Summary: Implementation of a 2D array
*******************************************************************************/

#include <uarray2.h>

/*******************************************************************************
*                              PRIVATE REPRESENTATIONS
*******************************************************************************/
struct UArray2_T {
        int width;
        int height;
        UArray_T uarray;
};

/*******************************************************************************
*                            PRIVATE DECLARATIONS
*******************************************************************************/

/*-----------------------------------------------------------------------------*
| get_index
|       Purpose: finds the index in the representation that corresponds to 
|                element [i,j] in the 2D array 
*-----------------------------------------------------------------------------*/
int get_index(int i, int j, int width);

/*-----------------------------------------------------------------------------*
| get_i
|       Purpose: finds the i coordinate of the element stored at the given 
|                index in the representation
*-----------------------------------------------------------------------------*/
int get_i(int index, int width);

/*-----------------------------------------------------------------------------*
| get_j
|       Purpose: finds the j coordinate of the element stored at the given 
|                index in the representation
*-----------------------------------------------------------------------------*/
int get_j(int index, int width);


/*******************************************************************************
*                        DEFINITIONS (PUBLIC FUNCTIONS)
*******************************************************************************/

/*-----------------------------------------------------------------------------*
| UArray2_new
|       Purpose:   creates an instance of UArray2
|       Arguments: the dimensions of the 2D array to be created as ints, the 
|                  size of each element to be stored in the 2D array as a size_t
|       Returns:   a pointer to a 2D array
|       Fail cases:
|               - either of the dimensions passed is not positive
|               - element size is less than or equal to 0
|       Note: Negative values assigned to a size_t variable are converted to 
|             extremely large positive values, and thus cannot be checked for
*-----------------------------------------------------------------------------*/

UArray2_T UArray2_new (int dim1, int dim2, size_t element_size) {
        assert((dim1 > 0) && (dim2 > 0));
        assert(element_size > 0);
        int length = dim1 * dim2;
        UArray2_T new_array = malloc(sizeof(*new_array));
        new_array->width = dim1;
        new_array->height = dim2;
        new_array->uarray = UArray_new(length, element_size);
        return new_array;
}

/*-----------------------------------------------------------------------------*
| UArray2_free
|       Purpose:   frees the memory allocated for the given UArray2
|       Arguments: a pointer to a pointer to a 2D array
|       Returns:   -
|       Fail cases:
|               - the pointer to the pointer to the 2D array is null
|               - the pointer to the 2D array is null
*-----------------------------------------------------------------------------*/
void UArray2_free (UArray2_T *uarray2) {
        assert((uarray2 != NULL) && (*uarray2 != NULL));
        UArray2_T temp = *uarray2;
        UArray_T array = temp->uarray;
        UArray_free(&array);
        free(temp);
}

/*-----------------------------------------------------------------------------*
| UArray2_width
|       Purpose:   finds the width of the given UArray2
|       Arguments: a pointer to a 2D array
|       Returns:   the width of the 2D array as an int
|       Fail cases:
|               - the pointer to the 2D array is null
*-----------------------------------------------------------------------------*/
int UArray2_width(UArray2_T uarray2) {
        assert(uarray2 != NULL);
        return uarray2->width;
}

/*-----------------------------------------------------------------------------*
| UArray2_height
|       Purpose:   finds the height of the given UArray2
|       Arguments: a pointer to a 2D array
|       Returns:   the height of the 2D array as an int
|       Fail cases:
|               - the pointer to the 2D array is null
*-----------------------------------------------------------------------------*/
int UArray2_height(UArray2_T uarray2) {
        assert(uarray2 != NULL);
        return uarray2->height;
}

/*-----------------------------------------------------------------------------*
| UArray2_size
|       Purpose:   finds the size of each element of the given UArray2
|       Arguments: a pointer to a 2D array
|       Returns:   the size of each element in the 2D array as an int
|       Fail cases:
|               - the pointer to the 2D array is null
*-----------------------------------------------------------------------------*/
int UArray2_size(UArray2_T uarray2) {
        assert(uarray2 != NULL);
        return UArray_size(uarray2->uarray);
}

/*-----------------------------------------------------------------------------*
| UArray2_at
|       Purpose:   finds the element stored at the specified matrix location of 
|                  the given UArray2
|       Arguments: a pointer to a 2D array, the matrix location to be accessed
|                  as a pair of ints
|       Returns:   a pointer to the element at the given matrix location
|       Fail cases:
|               - the pointer to the 2D array is null
|               - the matrix location lies outside of the bounds of the 2D array
*-----------------------------------------------------------------------------*/
void *UArray2_at(UArray2_T uarray2, int i, int j) {
        assert(uarray2 != NULL);
        assert((i >= 0 && i < UArray2_width(uarray2)) && 
               (j >= 0 && j < UArray2_height(uarray2)));
        int index = get_index(i, j, UArray2_height(uarray2));
        return UArray_at(uarray2->uarray, index);
}

/*-----------------------------------------------------------------------------*
| UArray2_map_col_major
|       Purpose:   maps the given function onto every element of the given 
|                  UArray2 in col major order
|       Arguments: a pointer to the 2D array, a pointer to a void apply 
|                  function, the closure as a void *
|       Returns:   -
|       Fail cases:
|               - the pointer to the 2D array is null
|               - the pointer to the apply function is null
*-----------------------------------------------------------------------------*/
void UArray2_map_col_major(UArray2_T uarray2, void apply(int i, int j, 
                           UArray2_T uarray2, void *value, void *cl), 
                           void *cl) {
        assert(uarray2 != NULL);
        assert(apply != NULL);
        int index;
        int i;
        int j;
        for (index = 0; index < UArray_length(uarray2->uarray); index ++) {
                i = get_i(index, UArray2_height(uarray2));
                j = get_j(index, UArray2_height(uarray2));
                apply(i, j, uarray2, UArray2_at(uarray2, i, j), cl);
        };
}

/*-----------------------------------------------------------------------------*
| UArray2_map_row_major
|       Purpose:   maps the given function onto every element of the given 
|                  UArray2 in row major order
|       Arguments: a pointer to the 2D array, a pointer to a void apply 
|                  function, the closure as a void *
|       Returns:   -
|       Fail cases:
|               - the pointer to the 2D array is null
|               - the pointer to the apply function is null
*-----------------------------------------------------------------------------*/
void UArray2_map_row_major(UArray2_T uarray2, void apply(int i, int j, 
                           UArray2_T uarray2, void *value, void *cl), 
                           void *cl) {
        assert(uarray2 != NULL);
        assert(apply != NULL);
        int i;
        int j;
        for (j = 0; j < UArray2_height(uarray2); j++) {
                for (i = 0; i < UArray2_width(uarray2); i++) { 
                        apply(i, j, uarray2, UArray2_at(uarray2, i, j), cl);
                };
        };
}
        
/*******************************************************************************
*                        DEFINITIONS (PRIVATE FUNCTIONS)
*******************************************************************************/

/*-----------------------------------------------------------------------------*
| get_index
|       Purpose:    finds the index in the representation that corresponds to 
|                   element [i,j] in the 2D array 
|       Arguments:  the matrix location as a pair of ints, the height of the 2D
|                   array as an int
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
|       Purpose:    finds the i coordinate of the element stored at the given 
|                   index in the representation
|       Arguments:  the index in the representation as an int, the height of 
|                   the 2D array as an int
|       Returns:    the i coordinate of the element stored at the given index in
|                   the representation as an int
|       Fail cases: none, as get_i can only be called by parent functions 
|                   which ensure get_i only receives valid arguments
*-----------------------------------------------------------------------------*/
int get_i (int index, int height) {
        return index / height;
}

/*-----------------------------------------------------------------------------*
| get_j
|       Purpose:    finds the j coordinate of the element stored at the given 
|                   index in the representation
|       Arguments:  the index in the representation as an int, the height of 
|                   the 2D array as an int
|       Returns:    the j coordinate of the element stored at the given index in
|                   the representation as an int
|       Fail cases: none, as get_j can only be called by parent functions 
|                   which ensure get_j only receives valid arguments
*-----------------------------------------------------------------------------*/
int get_j (int index, int height) {
        int i = index / height;
        return (index - i * height);
}
