/**
 * \file queue.c
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
#include "cgc/queue.h"

////////////////////////////////////////////////////////////////////////////////
// Dynamic creation and destruction.
////////////////////////////////////////////////////////////////////////////////

cgc_queue * cgc_queue_create (size_t element_size, cgc_copy_function copy_fun, cgc_clean_function clean_fun)
{
    return cgc_list_create (element_size, copy_fun, clean_fun);
}

void cgc_queue_destroy (cgc_queue * queue)
{
    cgc_list_destroy (queue);
}

////////////////////////////////////////////////////////////////////////////////
// Initialization and cleaning.
////////////////////////////////////////////////////////////////////////////////

int cgc_queue_init (cgc_queue * queue, size_t element_size, cgc_copy_function copy_fun, cgc_clean_function clean_fun)
{
    return cgc_list_init (queue, element_size, copy_fun, clean_fun);
}

int cgc_queue_clean (cgc_queue * queue)
{
    return cgc_list_clean (queue);
}

////////////////////////////////////////////////////////////////////////////////
// Copy.
////////////////////////////////////////////////////////////////////////////////

cgc_queue * cgc_queue_copy (const cgc_queue * queue)
{
    return cgc_list_copy (queue);
}

int cgc_queue_copy_into (const cgc_queue * original, cgc_queue * destination)
{
    return cgc_list_copy_into (original, destination);
}

////////////////////////////////////////////////////////////////////////////////
// Swap.
////////////////////////////////////////////////////////////////////////////////

int cgc_queue_swap (cgc_queue * a, cgc_queue * b)
{
    return cgc_list_swap (a, b);
}

////////////////////////////////////////////////////////////////////////////////
// Properties getters.
////////////////////////////////////////////////////////////////////////////////

bool cgc_queue_is_empty (const cgc_queue * queue)
{
    return cgc_list_is_empty (queue);
}

size_t cgc_queue_size (const cgc_queue * queue)
{
    return cgc_list_size (queue);
}

////////////////////////////////////////////////////////////////////////////////
// Access.
////////////////////////////////////////////////////////////////////////////////

void * cgc_queue_front (const cgc_queue * queue)
{
    return cgc_list_front (queue);
}

void * cgc_queue_back (const cgc_queue * queue)
{
    return cgc_list_back (queue);
}

////////////////////////////////////////////////////////////////////////////////
// Modifiers.
////////////////////////////////////////////////////////////////////////////////

int cgc_queue_push (cgc_queue * queue, const void * element)
{
    return cgc_list_push_back (queue, element);
}

void * cgc_queue_pop (cgc_queue * queue)
{
    return cgc_list_pop_front (queue);
}

int cgc_queue_clear (cgc_queue * queue)
{
    return cgc_list_clear (queue);
}
