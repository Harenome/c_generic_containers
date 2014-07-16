/**
 * \file string_vector.c
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
#include "cgc/string_vector.h"

////////////////////////////////////////////////////////////////////////////////
// Static utilities.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief CGC implementation of strdup.
 * \param string String of interest.
 * \return A pointer to a copy of the string.
 * strdup is not available in C99.
 */
static inline char * _cgc_strdup (const char * const string)
{
    char * copy = malloc ((strlen (string) + 1) * sizeof (char));
    if (copy != NULL)
        strcpy (copy, string);
    return copy;
}

/**
 * \brief Cleaning function for CGC String Vectors.
 * \param content.
 * cgc_string_vector's store pointers to strings... C strings are pointers to
 * characters...
 */
static void _cgc_str_clean (void * const content)
{
    char ** element = content;
    free (* element);
}

////////////////////////////////////////////////////////////////////////////////
// New, free.
////////////////////////////////////////////////////////////////////////////////

/* C strings are a bit special: they are pointers to arrays of characters.
 * We can not directly copy them into the vector, since their size may vary.
 *
 * The workaround is to store pointers to strings inside the vector: the size
 * of a pointer never changes. This requires one more step in the push and
 * insertion functions (a manual copy of the string).
 *
 * No copy function is needed for pointers.
 * A cleaning function is needed: the pointed string must be freed.
 */
cgc_string_vector * cgc_string_vector_create (size_t size)
{
    return cgc_vector_create (sizeof (char **), NULL, _cgc_str_clean, size);
}

void cgc_string_vector_destroy (cgc_string_vector * const vector)
{
    cgc_vector_destroy (vector);
}

////////////////////////////////////////////////////////////////////////////////
// Properties getters.
////////////////////////////////////////////////////////////////////////////////

bool cgc_string_vector_is_empty (const cgc_string_vector * const vector)
{
    return cgc_vector_is_empty (vector);
}

size_t cgc_string_vector_size (const cgc_string_vector * const vector)
{
    return cgc_vector_size (vector);
}

size_t cgc_string_vector_max_size (const cgc_string_vector * const vector)
{
    return cgc_vector_max_size (vector);
}

////////////////////////////////////////////////////////////////////////////////
// Access.
////////////////////////////////////////////////////////////////////////////////

char * cgc_string_vector_at (const cgc_string_vector * const vector, size_t i)
{
    char ** string = cgc_vector_at (vector, i);
    return * string;
}

char * cgc_string_vector_front (const cgc_string_vector * const vector)
{
    char ** string = cgc_vector_front (vector);
    return * string;
}

char * cgc_string_vector_back (const cgc_string_vector * const vector)
{
    char ** string = cgc_vector_back (vector);
    return * string;
}

////////////////////////////////////////////////////////////////////////////////
// Modifiers.
////////////////////////////////////////////////////////////////////////////////

/* cgc_string_vector_push_front() and cgc_string_vector_push_back():
 * -----------------------------------------------------------------
 * Copy the string, and store the pointer in the vector.
 */
int cgc_string_vector_push_front (cgc_string_vector * const vector, const char * const string)
{
    const char * const copy = _cgc_strdup (string);
    return cgc_vector_push_front (vector, & copy);
}

int cgc_string_vector_push_back (cgc_string_vector * const vector, const char * const string)
{
    const char * const copy = _cgc_strdup (string);
    return cgc_vector_push_back (vector, & copy);
}

int cgc_string_vector_insert (cgc_string_vector * const vector, size_t i, const char * const string)
{
    const char * const copy = _cgc_strdup (string);
    return cgc_vector_insert (vector, i, & copy);
}

/* cgc_string_vector_pop_front() and cgc_string_vector_pop_back():
 * ---------------------------------------------------------------
 * Usually, the user must free the element obtained via a pop operation.
 * Here, the element is a pointer to a string. However, the function directly
 * returns the string: don't forget to free the aforementioned ``element''
 * (the pointer to the string)!
 */
char * cgc_string_vector_pop_front (cgc_string_vector * const vector)
{

    char ** front_copy = cgc_vector_pop_front (vector);
    char * front = * front_copy;
    free (front_copy);
    return front;
}

char * cgc_string_vector_pop_back (cgc_string_vector * const vector)
{
    char ** back_copy = cgc_vector_pop_back (vector);
    char * back = * back_copy;
    free (back_copy);
    return back;
}

/* cgc_string_vector_clear() and cgc_string_vector_erase():
 * --------------------------------------------------------
 * No special action required, a mere call to the generic version is enough.
 */
int cgc_string_vector_clear (cgc_string_vector * const vector)
{
    return cgc_vector_clear (vector);
}

int cgc_string_vector_erase (cgc_string_vector * const vector, size_t start, size_t end)
{
    return cgc_vector_erase (vector, start, end);
}
