/**
 * \file stack.c
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
#include "cgc/stack.h"

////////////////////////////////////////////////////////////////////////////////
// Dynamic creation and destruction.
////////////////////////////////////////////////////////////////////////////////

cgc_stack * cgc_stack_create (size_t element_size, cgc_copy_function copy_fun, cgc_clean_function clean_fun)
{
    return cgc_list_create (element_size, copy_fun, clean_fun);
}

void cgc_stack_destroy (cgc_stack * stack)
{
    cgc_list_destroy (stack);
}

////////////////////////////////////////////////////////////////////////////////
// Initialization and cleaning.
////////////////////////////////////////////////////////////////////////////////

int cgc_stack_init (cgc_stack * stack, size_t element_size, cgc_copy_function copy_fun, cgc_clean_function clean_fun)
{
    return cgc_list_init (stack, element_size, copy_fun, clean_fun);
}

int cgc_stack_clean (cgc_stack * stack)
{
    return cgc_list_clean (stack);
}

////////////////////////////////////////////////////////////////////////////////
// Copy.
////////////////////////////////////////////////////////////////////////////////

cgc_stack * cgc_stack_copy (const cgc_stack * stack)
{
    return cgc_list_copy (stack);
}

int cgc_stack_copy_into (const cgc_stack * original, cgc_stack * destination)
{
    return cgc_list_copy_into (original, destination);
}

////////////////////////////////////////////////////////////////////////////////
// Swap.
////////////////////////////////////////////////////////////////////////////////

int cgc_stack_swap (cgc_stack * a, cgc_stack * b)
{
    return cgc_list_swap (a, b);
}

////////////////////////////////////////////////////////////////////////////////
// Properties getters.
////////////////////////////////////////////////////////////////////////////////

bool cgc_stack_is_empty (const cgc_stack * stack)
{
    return cgc_list_is_empty (stack);
}

size_t cgc_stack_size (const cgc_stack * stack)
{
    return cgc_list_size (stack);
}

////////////////////////////////////////////////////////////////////////////////
// Access.
////////////////////////////////////////////////////////////////////////////////

void * cgc_stack_top (const cgc_stack * stack)
{
    return cgc_list_front (stack);
}

////////////////////////////////////////////////////////////////////////////////
// Modifiers.
////////////////////////////////////////////////////////////////////////////////

int cgc_stack_push (cgc_stack * stack, const void * element)
{
    return cgc_list_push_front (stack, element);
}

void * cgc_stack_pop (cgc_stack * stack)
{
    return cgc_list_pop_front (stack);
}

int cgc_stack_clear (cgc_stack * stack)
{
    return cgc_list_clear (stack);
}
