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

static inline char * _cgc_strdup (const char * string)
{
    char * copy = malloc ((strlen (string) + 1) * sizeof (char));
    if (copy != NULL)
        strcpy (copy, string);
    return copy;
}

static void _cgc_str_clean (void * content)
{
    char ** element = content;
    free (* element);
}

////////////////////////////////////////////////////////////////////////////////
// New, free.
////////////////////////////////////////////////////////////////////////////////

cgc_string_vector * cgc_string_vector_new (size_t size)
{
    return cgc_vector_new (sizeof (char **), NULL, _cgc_str_clean, size);
}

void cgc_string_vector_free (cgc_string_vector * vector)
{
    cgc_string_vector_clear (vector);
    cgc_vector_free (vector);
}

////////////////////////////////////////////////////////////////////////////////
// Properties getters.
////////////////////////////////////////////////////////////////////////////////

bool cgc_string_vector_is_empty (const cgc_string_vector * vector)
{
    return cgc_vector_is_empty (vector);
}

size_t cgc_string_vector_size (const cgc_string_vector * vector)
{
    return cgc_vector_size (vector);
}

size_t cgc_string_vector_max_size (const cgc_string_vector * vector)
{
    return cgc_vector_max_size (vector);
}

////////////////////////////////////////////////////////////////////////////////
// Access.
////////////////////////////////////////////////////////////////////////////////

char * cgc_string_vector_at (cgc_string_vector * vector, size_t i)
{
    char ** string = cgc_vector_at (vector, i);
    return * string;
}

char * cgc_string_vector_front (cgc_string_vector * vector)
{
    char ** string = cgc_vector_front (vector);
    return * string;
}

char * cgc_string_vector_back (cgc_string_vector * vector)
{
    char ** string = cgc_vector_back (vector);
    return * string;
}

////////////////////////////////////////////////////////////////////////////////
// Modifiers.
////////////////////////////////////////////////////////////////////////////////

int cgc_string_vector_push_front (cgc_string_vector * vector, const char * string)
{
    char * copy = _cgc_strdup (string);
    return cgc_vector_push_front (vector, & copy);
}

int cgc_string_vector_push_back (cgc_string_vector * vector, const char * string)
{
    char * copy = _cgc_strdup (string);
    return cgc_vector_push_back (vector, & copy);
}

char * cgc_string_vector_pop_front (cgc_string_vector * vector)
{
    char ** front_copy = cgc_vector_pop_front (vector);
    char * front = * front_copy;
    free (front_copy);
    return front;
}

char * cgc_string_vector_pop_back (cgc_string_vector * vector)
{
    char ** back_copy = cgc_vector_pop_back (vector);
    char * back = * back_copy;
    free (back_copy);
    return back;
}

int cgc_string_vector_insert (cgc_string_vector * vector, size_t i, const char * string)
{
    char * copy = _cgc_strdup (string);
    return cgc_vector_insert (vector, i, & copy);
}

int cgc_string_vector_clear (cgc_string_vector * vector)
{
    return cgc_vector_clear (vector);
}

int cgc_string_vector_erase (cgc_string_vector * vector, size_t start, size_t end)
{
    return cgc_vector_erase (vector, start, end);
}
