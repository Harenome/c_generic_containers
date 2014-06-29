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

////////////////////////////////////////////////////////////////////////////////
// Typedef.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief CGC vector
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
// New, free.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Create a new cgc_vector
 * \param element_size Size of the elements.
 * \param copy_fun Copy function
 * \param clean_fun Cleaning function
 * \param size Vector size.
 * \relatesalso cgc_vector
 * \return pointer to a cgc_vector.
 */
cgc_vector * cgc_vector_new (size_t element_size, cgc_copy_function copy_fun, cgc_clean_function clean_fun, size_t size);

/**
 * \brief Free a cgc_vector.
 * \param vector Vector.
 * \relatesalso cgc_vector
 */
void cgc_vector_free (cgc_vector * vector);

////////////////////////////////////////////////////////////////////////////////
// Properties getters.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Check whether a CGC vector is empty.
 * \param vector Vector.
 * \retval true if the vector is empty.
 * \retval false otherwise.
 * \relatesalso cgc_vector
 */
bool cgc_vector_is_empty (const cgc_vector * vector);

/**
 * \brief Get the vector size.
 * \param vector Vector.
 * \return Size.
 * \relatesalso cgc_vector
 */
size_t cgc_vector_size (const cgc_vector * vector);

/**
 * \brief Get the vector max size.
 * \param vector Vector.
 * \return Max size.
 * \relatesalso cgc_vector
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
 */
void * cgc_vector_at (cgc_vector * vector, size_t i);

/**
 * \brief Get the first element.
 * \param vector Vector.
 * \return First element.
 * \relatesalso cgc_vector
 */
void * cgc_vector_front (cgc_vector * vector);

/**
 * \brief Get the last element.
 * \param vector Vector.
 * \return Last element.
 * \relatesalso cgc_vector
 */
void * cgc_vector_back (cgc_vector * vector);

////////////////////////////////////////////////////////////////////////////////
// Modifiers.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Push front an element.
 * \param vector Vector.
 * \param element Element.
 * \retval 0 in case of success
 * \retval -1 if one of the arguments is \c NULL. \c errno shall be set to
 * \c EINVAL.
 * \return This function shall return 0 in case of success, a negative integer
 * in case of failure.
 * \note The supplied element will be copied into the list. It is safe to free
 * \c element afterwards.
 * \relatesalso cgc_vector
 */
int cgc_vector_push_front (cgc_vector * vector, void * element);

/**
 * \brief Push back an element.
 * \param vector Vector.
 * \param element Element.
 * \retval 0 in case of success
 * \retval -1 if one of the arguments is \c NULL. \c errno shall be set to
 * \c EINVAL.
 * \return This function shall return 0 in case of success, a negative integer
 * in case of failure.
 * \note The supplied element will be copied into the list. It is safe to free
 * \c element afterwards.
 * \relatesalso cgc_vector
 */
int cgc_vector_push_back (cgc_vector * vector, void * element);

/**
 * \brief Pop front an element.
 * \param vector Vector.
 * \return element
 * \relatesalso cgc_vector
 */
void * cgc_vector_pop_front (cgc_vector * vector);

/**
 * \brief Pop back an element.
 * \param vector Vector.
 * \return element
 * \relatesalso cgc_vector
 */
void * cgc_vector_pop_back (cgc_vector * vector);

/**
 * \brief Insert an element at index \c i.
 * \param vector Vector.
 * \param i Index.
 * \param element Element.
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
int cgc_vector_insert (cgc_vector * vector, size_t i, void * element);

/**
 * \brief Clear the vector.
 * \param vector
 * \relatesalso cgc_vector
 * \return This function shall return 0 in case of success, a negative integer
 * in case of failure.
 * \retval 0 in case of success.
 * \retval -1 if \c vector is \c NULL. \c errno shall be set to \c EINVAL.
 */
int cgc_vector_clear (cgc_vector * vector);

/**
 * \brief Erase the content from \c start to \c end.
 * \param vector Vector.
 * \param start Start.
 * \param end End.
 * \relatesalso cgc_vector
 * \return This function shall return 0 in case of success, a negative integer
 * in case of failure.
 * \retval 0 in case of success.
 * \retval -1 if one of the arguments is NULL. \c errno shall be set to \c EINVAL.
 */
int cgc_vector_erase (cgc_vector * vector, size_t start, size_t end);

#endif /* _CGC_VECTOR_H_ */
