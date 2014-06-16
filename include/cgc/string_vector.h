/**
 * \file string_vector.h
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
#ifndef _CGC_STRING_VECTOR_H_
#define _CGC_STRING_VECTOR_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "cgc/vector.h"

////////////////////////////////////////////////////////////////////////////////
// Typedef.
////////////////////////////////////////////////////////////////////////////////

/**
 * \class cgc_string_vector
 * \sa cgc_vector
 */
/**
 * \brief String vector.
 * \sa cgc_vector
 */
typedef cgc_vector cgc_string_vector;

////////////////////////////////////////////////////////////////////////////////
// New, free.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Create a new cgc_string_vector
 * \param element_size Size of the elements.
 * \param size Vector size.
 * \relatesalso cgc_string_vector
 * \return pointer to a cgc_string_vector.
 */
cgc_string_vector * cgc_string_vector_new (size_t size);

/**
 * \brief Free a cgc_string_vector.
 * \param vector.
 * \relatesalso cgc_string_vector
 */
void cgc_string_vector_free (cgc_string_vector * vector);

////////////////////////////////////////////////////////////////////////////////
// Properties getters.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Check whether a CGC vector is empty.
 * \param vector Vector.
 * \retval true if the vector is empty.
 * \retval false otherwise.
 * \relatesalso cgc_string_vector
 */
bool cgc_string_vector_is_empty (const cgc_string_vector * vector);

/**
 * \brief Get the vector size.
 * \param vector Vector.
 * \return Size.
 * \relatesalso cgc_string_vector
 */
size_t cgc_string_vector_size (const cgc_string_vector * vector);

/**
 * \brief Get the vector max size.
 * \param vector Vector.
 * \return Max size.
 * \relatesalso cgc_string_vector
 */
size_t cgc_string_vector_max_size (const cgc_string_vector * vector);

////////////////////////////////////////////////////////////////////////////////
// Access.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Get the element at index \c i.
 * \param vector Vector.
 * \param i Index.
 * \return element.
 * \relatesalso cgc_string_vector
 */
char * cgc_string_vector_at (cgc_string_vector * vector, size_t i);

/**
 * \brief Get the first element.
 * \param vector.
 * \return First element.
 * \relatesalso cgc_string_vector
 */
char * cgc_string_vector_front (cgc_string_vector * vector);

/**
 * \brief Get the last element.
 * \param vector.
 * \return Last element.
 * \relatesalso cgc_string_vector
 */
char * cgc_string_vector_back (cgc_string_vector * vector);

////////////////////////////////////////////////////////////////////////////////
// Modifiers.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Push front an element.
 * \param vector Vector.
 * \param element Element.
 * \relatesalso cgc_vector
 */
int cgc_string_vector_push_front (cgc_string_vector * vector, const char * string);

/**
 * \brief Push back an element.
 * \param vector Vector.
 * \param element Element.
 * \relatesalso cgc_string_vector
 */
int cgc_string_vector_push_back (cgc_string_vector * vector, const char * string);

/**
 * \brief Pop front an element.
 * \param vector Vector.
 * \param element Element.
 * \return element
 * \relatesalso cgc_vector
 */
char * cgc_string_vector_pop_front (cgc_string_vector * vector);

/**
 * \brief Pop back an element.
 * \param vector Vector.
 * \param element Element.
 * \return element
 * \relatesalso cgc_string_vector
 */
char * cgc_string_vector_pop_back (cgc_string_vector * vector);

/**
 * \brief Insert an element at index \c i.
 * \param vector Vector.
 * \param i Index.
 * \param element Element.
 * \relatesalso cgc_string_vector
 */
int cgc_string_vector_insert (cgc_string_vector * vector, size_t i, const char * string);

/**
 * \brief Clear the vector.
 * \param vector
 * \relatesalso cgc_string_vector
 */
void cgc_string_vector_clear (cgc_string_vector * vector);

/**
 * \brief Erase the content from \c start to \c end.
 * \param vector Vector.
 * \param start Start.
 * \param end End.
 * \relatesalso cgc_vector
 */
int cgc_string_vector_erase (cgc_string_vector * vector, size_t start, size_t end);

#endif /* _CGC_STRING_VECTOR_H_ */
