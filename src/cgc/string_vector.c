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

cgc_string_vector * cgc_string_vector_new (size_t size)
{
    return cgc_vector_new (sizeof (char **), size);
}

void cgc_string_vector_free (cgc_string_vector * vector)
{
    cgc_string_vector_clear (vector);
    cgc_vector_free (vector);
}

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

int cgc_string_vector_push_back (cgc_string_vector * vector, const char * string)
{
    char * copy = malloc ((strlen (string) + 1) * sizeof (char));
    strcpy (copy, string);
    return cgc_vector_push_back (vector, & copy);
}

char * cgc_string_vector_pop_back (cgc_string_vector * vector)
{
    char ** back_copy = cgc_vector_pop_back (vector);
    char * back = * back_copy;
    free (back_copy);
    return back;
}

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

void cgc_string_vector_clear (cgc_string_vector * vector)
{
    size_t size = cgc_string_vector_size (vector);
    for (size_t i = 0; i < size; ++i)
    {
        char * element = cgc_string_vector_at (vector, i);
        free (element);
    }
    cgc_vector_clear (vector);
}

