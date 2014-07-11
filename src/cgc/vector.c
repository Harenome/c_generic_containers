/**
 * \file vector.c
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
#include "cgc/vector.h"

////////////////////////////////////////////////////////////////////////////////
// Constants.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief An arbitrary step value for vectors.
 */
static const size_t _DEFAULT_SIZE_STEP = 128;

////////////////////////////////////////////////////////////////////////////////
// Static utilities.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Get the address of the element at index \c i.
 * \param vector A pointer to a CGC Vector.
 * \param i Index of the target.
 * \return A pointer to the element.
 * \pre \c vector != \c NULL.
 */
static inline void * _cgc_vector_address (const cgc_vector * const vector, size_t i)
{
    char * v = vector->_content;
    return v + i * vector->_element_size;
}

/**
 * \brief Resize a vector.
 * \pparam vector A pointer to a CGC vector.
 * \param new_size The new size of the vector.
 * \retval 0.
 * \pre vetor != NULL.
 */
static inline int _cgc_vector_grow (cgc_vector * const vector, size_t new_size)
{
    void * new_content = realloc (vector->_content, new_size * vector->_element_size);
    if (new_content != NULL)
    {
        vector->_content = new_content;
        vector->_max_size = new_size;
    }
    return 0;
}

/**
 * \brief Copy an element into a vector.
 * \param vector A pointer to a CGC Vector.
 * \param i Target index.
 * \param element A pointer to the element of interest.
 * \retval 0 in case of success.
 * \pre vetor != NULL.
 */
static inline int _cgc_vector_copy_element (cgc_vector * const vector, size_t i, const void * const element)
{
    int error = 0;
    void * v = _cgc_vector_address (vector, i);
    if (vector->_copy_fun != NULL)
        error = vector->_copy_fun (element, v);
    else
        memcpy (v, element, vector->_element_size);
    return error;
}

/**
 * \brief Shift some elements of a vector.
 * \param vector A pointer to a CGC Vector.
 * \param source Index of the source.
 * \param destination Target index.
 * \pre vetor != NULL.
 */
static inline void _cgc_vector_shift_elements (cgc_vector * const vector, size_t source, size_t destination)
{
    void * start = _cgc_vector_address (vector, source);
    void * new_start = _cgc_vector_address (vector, destination);
    size_t shift_start = source > destination ? destination : source;
    memmove (new_start, start, vector->_element_size * (vector->_size - shift_start));
}

/**
 * \brief Reset some elements of a vector.
 * \param vector A pointer to a CGC Vector.
 * \param start Start index.
 * \param end End index.
 * \pre \c start <= \c end.
 * \pre vetor != NULL.
 */
static inline void _cgc_vector_reset_elements (cgc_vector * const vector, size_t start, size_t end)
{
    size_t total_size = end - start;
    void * beginning = _cgc_vector_address (vector, start);
    memset (beginning, 0, total_size * vector->_element_size);
}

////////////////////////////////////////////////////////////////////////////////
// New, free.
////////////////////////////////////////////////////////////////////////////////

cgc_vector * cgc_vector_create (size_t element_size, cgc_copy_function copy_fun, cgc_clean_function clean_fun, size_t size)
{
    size_t size_step = size != 0 ? size : _DEFAULT_SIZE_STEP;

    cgc_vector * vector = malloc (sizeof * vector);
    if (vector != NULL)
    {
        vector->_content = malloc (size_step * element_size);
        if (vector->_content != NULL)
        {
            vector->_size = 0;
            vector->_max_size = size_step;
            vector->_size_step = size_step;
            vector->_element_size = element_size;
            vector->_copy_fun = copy_fun;
            vector->_clean_fun = clean_fun;
        }
        else
        {
            free (vector);
            vector = NULL;
        }
    }

    return vector;
}

void cgc_vector_destroy (cgc_vector * const vector)
{
    if (vector != NULL)
    {
        cgc_vector_clear (vector);
        free (vector->_content);
    }
    free (vector);
}

////////////////////////////////////////////////////////////////////////////////
// Properties getters.
////////////////////////////////////////////////////////////////////////////////

bool cgc_vector_is_empty (const cgc_vector * const vector)
{
    return vector->_size == 0;
}

size_t cgc_vector_size (const cgc_vector * const vector)
{
    return vector->_size;
}

size_t cgc_vector_max_size (const cgc_vector * const vector)
{
    return vector->_max_size;
}

////////////////////////////////////////////////////////////////////////////////
// Access.
////////////////////////////////////////////////////////////////////////////////

void * cgc_vector_at (const cgc_vector * const vector, size_t i)
{
    return _cgc_vector_address (vector, i);
}

void * cgc_vector_front (const cgc_vector * const vector)
{
    return _cgc_vector_address (vector, 0);
}

void * cgc_vector_back (const cgc_vector * const vector)
{
    size_t back_index = vector->_size > 0 ? vector->_size - 1 : 0;
    return _cgc_vector_address (vector, back_index);
}

////////////////////////////////////////////////////////////////////////////////
// Modifiers.
////////////////////////////////////////////////////////////////////////////////

static inline int _cgc_vector_push_prelude (const cgc_vector * const vector, const void * const element)
{
    int error = cgc_check_pointer (vector);
    if (! error)
        error = cgc_check_pointer (element);

    return error;
}

int cgc_vector_push_front (cgc_vector * const vector, const void * const element)
{
    int error = _cgc_vector_push_prelude (vector, element);
    if (! error)
        error = cgc_vector_insert (vector, 0, element);

    return error;
}

int cgc_vector_push_back (cgc_vector * const vector, const void * const element)
{
    int error = _cgc_vector_push_prelude (vector, element);
    if (! error)
    {
        if (vector->_size >= vector->_max_size)
            _cgc_vector_grow (vector, vector->_max_size + vector->_size_step);

        error = _cgc_vector_copy_element (vector, vector->_size, element);
        vector->_size++;
    }
    return error;
}

void * cgc_vector_pop_front (cgc_vector * const vector)
{
    void * element = NULL;
    if (vector != NULL)
    {
        void * front = cgc_vector_front (vector);
        element = malloc (vector->_element_size);
        if (element != NULL)
        {
            memcpy (element, front, vector->_element_size);
            vector->_size--;
            _cgc_vector_shift_elements (vector, 1, 0);
        }
    }
    return element;
}

void * cgc_vector_pop_back (cgc_vector * const vector)
{
    void * element = NULL;
    if (vector != NULL)
    {
        void * back = cgc_vector_back (vector);
        element = malloc (vector->_element_size);
        if (element != NULL)
        {
            memcpy (element, back, vector->_element_size);
            vector->_size--;
        }
    }
    return element;
}

int cgc_vector_insert (cgc_vector * const vector, size_t i, const void * const element)
{
    int error = _cgc_vector_push_prelude (vector, element);
    if (! error)
    {
        if (i >= vector->_max_size)
        {
            size_t new_size = ((i / vector->_size_step) + 1) * vector->_size_step;
            _cgc_vector_grow (vector, new_size);
            vector->_size = i+1;
        }
        else if (i <= vector->_size)
        {
            if (vector->_size + 1 >= vector->_max_size)
                _cgc_vector_grow (vector, vector->_max_size + vector->_size_step);
            _cgc_vector_shift_elements (vector, i, i + 1);
            vector->_size++;
        }
        else
        {
            vector->_size = i+1;
        }
        error = _cgc_vector_copy_element (vector, i, element);
    }

    return error;
}

int cgc_vector_clear (cgc_vector * const vector)
{
    return cgc_vector_erase (vector, 0, cgc_vector_size (vector));
}

int cgc_vector_erase (cgc_vector * const vector, size_t start, size_t end)
{
    int error = 0;
    if (start > end || vector == NULL || start > vector->_size)
    {
        error = -1;
        errno = EINVAL;
    }

    if (! error)
    {
        if (vector->_clean_fun != NULL)
            for (size_t i = start; i < end; ++i)
                vector->_clean_fun (_cgc_vector_address (vector, i));

        size_t actual_end = end > vector->_size ? vector->_size : end;
        size_t erased_count = actual_end - start;
        vector->_size -= erased_count;
        _cgc_vector_shift_elements (vector, actual_end, start);
    }

    return error;
}
