/**
 * \file vector.h
 * \author RAZANAJATO RANAIVOARIVONY Harenome
 * \date 2014
 * \copyright LGPLv3
 */
/* Copyright © 2014 RAZANAJATO RANAIVOARIVONY Harenome.
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * and the GNU General Public License along with this program.
 * If not, see http://www.gnu.org/licenses/.
 */
#ifndef _CGC_VECTOR_H_
#define _CGC_VECTOR_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#include "cgc/common.h"
#include "cgc/types.h"

/**
 * \defgroup vectors_group Vectors
 */

////////////////////////////////////////////////////////////////////////////////
// Typedef.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief CGC vector.
 * \ingroup vectors_group
 *
 * CGC Vectors are generic vectors. Elements are stored contiguously inside
 * the vector.
 *
 * # Basics
 * To achieve genericity, CGC Vectors rely on 3 things:
 * - the size in bytes of the elements
 * - a copy function
 * - a cleaning function
 *
 * It is up to the user to properly write these 2 functions, if needed.
 *
 * Since elements are stored contiguously, the vector allocates memory for
 * several elements, and grows when more memory is needed.
 *
 * ## Element allocation
 * CGC Vectors allocate memory for several elements. If a size is provided at
 * the vector creation, it will be used as the default step for the vector's
 * growth (see cgc_vector_create()). Otherwise, the vector growth will use an
 * arbitrary step.
 *
 * ## Copy function
 * Since the CGC Vector could possibly hold any type of element, it might be
 * necessary to supply a way to properly copy elements into the vector.
 *
 * If no copy function was provided at the vector creation, a mere copy (using
 * \c memcpy) of the provided element will be done.
 *
 * For further information, see #cgc_copy_function.
 *
 * ## Cleaning function
 * A cleaning function may be required since the CGC Vector aims to be generic:
 * users should be able to store any type of element in the list. Elements could
 * themselves point to dynamically allocated memory. Thus this function shall,
 * if needed, free any memory an element of the list could supposedly hold.
 *
 * Note that the function should free any dynamically allocated memory held by
 * the element, but should not free the aforementioned element.
 *
 * For further information, see #cgc_clean_function.
 *
 * # CGC Vector creation and destruction
 * ## Dynamic allocation
 * CGC Vectors can be dynamically created using cgc_vector_create(). Such vectors
 * must be destroyed using cgc_vector_destroy().
 *
 * # Vector properties
 * One can determine whether a vector is empty using cgc_vector_is_empty(), get
 * its current size using cgc_vector_size(), and its current maximum size (the
 * size at which a resizing operation will occur) with cgc_vector_max_size().
 *
 * # Element access
 * ## Without changing the vector
 * There are three functions that return pointers to elements of the vector.
 * Please be aware that the pointers directly point to an element of the vector:
 * modifying it should be done with extreme care.
 *
 * Access function     | Pointer to
 * --------------------|---------------
 * cgc_vector_at()     | Nth element
 * cgc_vector_front()  | First element
 * cgc_vector_back()   | Last element
 *
 * Note that these functions do not:
 * - check whether the supplied vector is valid
 * - check whether the requested element exists.
 *
 * ## While modifying the vector
 * It is also possible to get an element while removing it from the vector.
 *
 * \b Important: Elements accessed in this fashion shall be freed by the user.
 *
 * Access function         | Element
 * ------------------------|----------
 * cgc_vector_pop_front()  | First element
 * cgc_vector_pop_back()   | Last element
 *
 * Note that removing the first element implies shifting the remainder of the
 * vector. If numerous removals of the first element are planned, consider
 * using cgc_list instead of cgc_vector.
 *
 * # Modification
 * ## Insertions
 *
 * Insertion function         | Insertion
 * ---------------------------|------------------------
 * cgc_vector_push_front()    | As the first element
 * cgc_vector_push_back()     | As the last element
 * cgc_vector_insert()        | Before the Nth element
 *
 * Note that insertions anywhere else than the back imply shifting the remainder
 * of the vector. If such insertions are to be used numerous times, consider
 * using cgc_list instead of cgc_vector.
 *
 * ## Deletions
 * Deletion function         | Deleted elements
 * --------------------------|-----------------------------
 * cgc_vector_pop_front()    | First element
 * cgc_vector_pop_back()     | Last element
 * cgc_vector_erase()        | Several contiguous elements
 * cgc_vector_clear()        | All elements
 */
typedef struct cgc_vector
{
    size_t _size;                   /**<- Size. */
    size_t _max_size;               /**<- Max size. */
    size_t _size_step;              /**<- Size step. */
    size_t _element_size;           /**<- Element size. */
    void * _content;                /**<- Content. */
    cgc_copy_function _copy_fun;    /**<- Copy function. */
    cgc_clean_function _clean_fun;  /**<- Clean function. */
} cgc_vector;

////////////////////////////////////////////////////////////////////////////////
// Dynamic creation and destruction.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Create a new cgc_vector
 * \param element_size Size of the elements.
 * \param copy_fun Copy function
 * \param clean_fun Cleaning function
 * \param size Vector size.
 * \relatesalso cgc_vector
 * \return pointer to a cgc_vector.
 * \retval NULL if the vector could not be allocated.
 * \note Vectors obtained this way must be detroyed using cgc_vector_destroy().
 * \note In case of failure, \c errno may be set to \c ENOMEM.
 * \note A call to this function may change the value of \c errno.
 */
cgc_vector * cgc_vector_create (size_t element_size, cgc_copy_function copy_fun, cgc_clean_function clean_fun, size_t size);

/**
 * \brief Free a cgc_vector.
 * \param vector Vector.
 * \relatesalso cgc_vector
 * \note It is safe to pass a \c NULL pointer to this function.
 * \warning It is strongly advised to use this function only on vectors obtained
 * via cgc_vector_create().
 */
void cgc_vector_destroy (cgc_vector * vector);

////////////////////////////////////////////////////////////////////////////////
// Initialization and cleaning.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Initialize a cgc_vector.
 * \param[in,out] vector Vector.
 * \param[in] element_size Element size.
 * \param[in] copy_fun Copy function.
 * \param[in] clean_fun Cleaning function.
 * \param[in] size Vector size.
 * \relatesalso cgc_vector
 * \retval 0 in case of success.
 * \retval -1 if one of the argumens is \c NULL. \c errno shall be set to
 * \retval -2 in case of failure because of \c malloc.
 * \c EINVAL.
 */
int cgc_vector_init (cgc_vector * vector, size_t element_size, cgc_copy_function copy_fun, cgc_clean_function clean_fun, size_t size);

/**
 * \brief Clean a cgc_vector.
 * \param[in,out] vector Vector.
 * \retval 0 in case of success.
 * \retval -1 if \c vector is \c NULL. \c errno shall be set to \c EINVAL.
 * \relatesalso cgc_vector
 */
int cgc_vector_clean (cgc_vector * vector);

////////////////////////////////////////////////////////////////////////////////
// Copy.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Copy a vector.
 * \param vector Vector.
 * \return A pointer to the copy in case of success. \c NULL in case of failure.
 * \retval NULL if the vector could not be copied or \c vector is \c NULL.
 * \note Vectors obtained this way must be destroyed using cgc_vector_destroy().
 * \note A call to this function may change the value of \c errno.
 */
cgc_vector * cgc_vector_copy (const cgc_vector * vector);

/**
 * \brief Copy a vector into \c destination.
 * \param[in] original The original.
 * \param[in,out] destination The destination of the copy.
 * \retval 0 in case of success.
 * \retval -1 if one of the arguments is \c NULL. \c errno may be set to
 * \c EINVAL.
 * \relatesalso cgc_vector
 * \warning This function will overwrite the destination! It is up to the user
 * to first clean \c destination, if needed.
 */
int cgc_vector_copy_into (const cgc_vector * original, cgc_vector * destination);

////////////////////////////////////////////////////////////////////////////////
// Swap.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Swap the contents of two vectors.
 * \param[in,out] a First vector.
 * \param[in,out] b Second vector.
 * \relatesalso cgc_vector
 * \pre a != NULL && b != NULL
 * \return 0 in case of success.
 * \return -1 if at least of the argumes is \c NULL. \c errno may be set to
 * \c EINVAL.
 * \note A cal to this function may change the value of \c errno.
 */
int cgc_vector_swap (cgc_vector * a, cgc_vector * b);

////////////////////////////////////////////////////////////////////////////////
// Properties getters.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Check whether a CGC vector is empty.
 * \param vector Vector.
 * \retval true if the vector is empty.
 * \retval false otherwise.
 * \relatesalso cgc_vector
 * \pre \c vector != \c NULL
 */
bool cgc_vector_is_empty (const cgc_vector * vector);

/**
 * \brief Get the vector size.
 * \param vector Vector.
 * \return Size.
 * \relatesalso cgc_vector
 * \pre \c vector != \c NULL
 */
size_t cgc_vector_size (const cgc_vector * vector);

/**
 * \brief Get the vector max size.
 * \param vector Vector.
 * \return Max size.
 * \relatesalso cgc_vector
 * \pre \c vector != \c NULL
 */
size_t cgc_vector_max_size (const cgc_vector * vector);

////////////////////////////////////////////////////////////////////////////////
// Access.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Get the element at index \c i.
 * \param vector Vector.
 * \param i Index.
 * \return element.
 * \relatesalso cgc_vector
 * \pre cgc_vector_size(vector) > \c i
 * \pre \c vector != \c NULL
 * \warning This function does not check whether the supplied vector is valid!
 * \warning This function does not check whether the supplied vector is long
 * enough!
 */
void * cgc_vector_at (const cgc_vector * vector, size_t i);

/**
 * \brief Get the first element.
 * \param vector Vector.
 * \return First element.
 * \relatesalso cgc_vector
 * \pre cgc_vector_is_empty(vector) == \c false
 * \pre \c vector != \c NULL
 * \warning This function does not check whether the supplied vector is valid!
 * \warning This function does not check whether the supplied vector is long
 * enough!
 */
void * cgc_vector_front (const cgc_vector * vector);

/**
 * \brief Get the last element.
 * \param vector Vector.
 * \return Last element.
 * \relatesalso cgc_vector
 * \pre cgc_vector_is_empty(vector) == \c false
 * \pre \c vector != \c NULL
 * \warning This function does not check whether the supplied vector is valid!
 * \warning This function does not check whether the supplied vector is long
 * enough!
 */
void * cgc_vector_back (const cgc_vector * vector);

////////////////////////////////////////////////////////////////////////////////
// Modifiers.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Push front an element.
 * \param[in,out] vector Vector.
 * \param[in] element Element.
 * \retval 0 in case of success
 * \retval -1 if one of the arguments is \c NULL. \c errno shall be set to
 * \c EINVAL.
 * \return This function shall return 0 in case of success, a negative integer
 * in case of failure.
 * \note The supplied element will be copied into the list. It is safe to free
 * \c element afterwards.
 * \relatesalso cgc_vector
 */
int cgc_vector_push_front (cgc_vector * vector, const void * element);

/**
 * \brief Push back an element.
 * \param[in,out] vector Vector.
 * \param[in] element Element.
 * \retval 0 in case of success
 * \retval -1 if one of the arguments is \c NULL. \c errno shall be set to
 * \c EINVAL.
 * \return This function shall return 0 in case of success, a negative integer
 * in case of failure.
 * \note The supplied element will be copied into the list. It is safe to free
 * \c element afterwards.
 * \relatesalso cgc_vector
 */
int cgc_vector_push_back (cgc_vector * vector, const void * element);

/**
 * \brief Pop front an element.
 * \param[in,out] vector Vector.
 * \return element
 * \relatesalso cgc_vector
 */
void * cgc_vector_pop_front (cgc_vector * vector);

/**
 * \brief Pop back an element.
 * \param[in,out] vector Vector.
 * \return element
 * \relatesalso cgc_vector
 */
void * cgc_vector_pop_back (cgc_vector * vector);

/**
 * \brief Insert an element at index \c i.
 * \param[in,out] vector Vector.
 * \param[in] i Index.
 * \param[in] element Element.
 * \relatesalso cgc_vector
 * \return This function shall return 0 in case of success, a negative integer
 * in case of failure.
 * \retval 0 in case of success.
 * \retval -1 if one of the arguments is \c NULL. \c errnor shall be set to
 * \c EINVAL.
 * \note If \c i is greater that the size of the list, the \c element will be
 * inserted at the end of the list.
 * \note The supplied element will be copied into the list. It is safe to free
 * \c element afterwards, if it was malloc'd.
 */
int cgc_vector_insert (cgc_vector * vector, size_t i, const void * element);

/**
 * \brief Clear the vector.
 * \param[in,out] vector
 * \relatesalso cgc_vector
 * \return This function shall return 0 in case of success, a negative integer
 * in case of failure.
 * \retval 0 in case of success.
 * \retval -1 if \c vector is \c NULL. \c errno shall be set to \c EINVAL.
 */
int cgc_vector_clear (cgc_vector * vector);

/**
 * \brief Erase the content from \c start to \c end.
 * \param[in,out] vector Vector.
 * \param[in] start Start.
 * \param[in] end End.
 * \relatesalso cgc_vector
 * \return This function shall return 0 in case of success, a negative integer
 * in case of failure.
 * \retval 0 in case of success.
 * \retval -1 if one of the arguments is NULL. \c errno shall be set to \c EINVAL.
 */
int cgc_vector_erase (cgc_vector * vector, size_t start, size_t end);

#endif /* _CGC_VECTOR_H_ */
