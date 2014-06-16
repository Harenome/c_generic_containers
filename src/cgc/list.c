/**
 * \file list.c
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
#include "cgc/list.h"

cgc_list * cgc_list_new (cgc_alloc_function alloc_fun, cgc_free_function free_fun, cgc_copy_function copy_fun)
{
    cgc_list * list = malloc (sizeof * list);
    if (list != NULL)
    {
        * list = (cgc_list) {
            ._first = NULL,
            ._last = NULL,
            ._alloc_fun = alloc_fun,
            ._free_fun = free_fun,
            ._copy_fun = copy_fun,
        };
    }

    return list;
}

void cgc_list_free (cgc_list * list)
{
    if (list != NULL)
    {
        cgc_list_element * current = list->_first;
        while (current != NULL)
        {
            cgc_list_element * next = current->_next;
            list->_free_fun (current->_content);
            free (current);
            current = next;
        }
        free (list);
    }
}

bool cgc_list_is_empty (const cgc_list * list)
{
    return list->_first == NULL;
}

const void * cgc_list_front (const cgc_list * list)
{
    return list->_first->_content;
}

const void * cgc_list_back (const cgc_list * list)
{
    return list->_last->_content;
}

size_t cgc_list_size (const cgc_list * list)
{
    size_t size = 0;
    if (list != NULL)
        for (cgc_list_element * e = list->_first; e != NULL; e = e->_next)
            ++size;
    return size;
}

void * cgc_list_pop_front (cgc_list * list)
{
    void * content = list->_first->_content;

    cgc_list_element * new_first = list->_first->_next;
    if (new_first != NULL)
        new_first->_previous = NULL;
    free (list->_first);
    list->_first = new_first;

    return content;
}

void * cgc_list_pop_back (cgc_list * list)
{
    void * content = list->_last->_content;

    cgc_list_element * new_last = list->_last->_previous;
    if (new_last != NULL)
        new_last->_next = NULL;
    free (list->_last);
    list->_last = new_last;

    return content;
}

static cgc_list_element * _cgc_list_element_alloc (void)
{
    cgc_list_element * e = malloc (sizeof * e);
    if (e != NULL)
        * e = (cgc_list_element) { ._content = NULL, ._next = NULL, ._previous = NULL };

    return e;
}

static int _cgc_list_copy_content (cgc_list * list,  cgc_list_element * e, void * content)
{
    int error = 0;
    e->_content = list->_alloc_fun ();
    if (e->_content == NULL)
        error = -3;
    else
        list->_copy_fun (content, e->_content);

    return error;
}

static int _cgc_list_push_prelude (cgc_list * list, void * content, cgc_list_element ** e)
{
    int error = cgc_check_pointer (list);
    if (! error)
        error = cgc_check_pointer (content);

    if (! error)
        * e = _cgc_list_element_alloc ();

    if (! error)
        error = e != NULL ? _cgc_list_copy_content (list, * e, content) : -2;

    return error;
}

int cgc_list_push_front (cgc_list * list, void * content)
{
    cgc_list_element * e = NULL;
    int error = _cgc_list_push_prelude (list, content, &e);

    if (! error)
    {
        e->_next = list->_first;
        if (list->_first != NULL)
            list->_first->_previous = e;
        list->_first = e;
        if (list->_last == NULL)
            list->_last = e;
    }

    return error;
}

int cgc_list_push_back (cgc_list * list, void * content)
{
    cgc_list_element * e = NULL;
    int error = _cgc_list_push_prelude (list, content, &e);

    if (! error)
    {
        e->_previous = list->_last;
        if (list->_last != NULL)
            list->_last->_next = e;
        list->_last = e;
        if (list->_first == NULL)
            list->_first = e;
    }

    return error;
}

cgc_list * cgc_list_copy (const cgc_list * list)
{
    cgc_list * copy = cgc_list_new (list->_alloc_fun, list->_free_fun, list->_copy_fun);
    if (! cgc_list_is_empty (list))
        for (cgc_list_element * e = list->_first; e != NULL; e = e->_next)
            cgc_list_push_back (copy, e->_content);
    return copy;
}

void cgc_list_map (cgc_list * list, cgc_unary_op_function op_fun)
{
    if (! cgc_list_is_empty (list))
        for (cgc_list_element * e = list->_first; e != NULL; e = e->_next)
            op_fun (e->_content);
}

void cgc_list_fold_left (const cgc_list * list, cgc_binary_op_left_function op_fun, void * base_result)
{
    if (! cgc_list_is_empty (list))
    {
        op_fun (base_result, list->_first->_content);
        for (cgc_list_element * e = list->_first->_next; e != NULL; e = e->_next)
            op_fun (base_result, e->_content);
    }
}

void cgc_list_fold_right (const cgc_list * list, cgc_binary_op_right_function op_fun, void * base_result)
{
    if (! cgc_list_is_empty (list))
    {
        op_fun (list->_first->_content, base_result);
        for (cgc_list_element * e = list->_last; e != NULL; e = e->_previous)
            op_fun (e->_content, base_result);
    }
}
