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

////////////////////////////////////////////////////////////////////////////////
// Static utilities.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Actually initialize a list.
 * \param list A pointer to a CGC List.
 * \param element_size The size in bytes of the elements.
 * \param copy_fun The copy function.
 * \param clean_fun The cleaning function.
 * \pre list != NULL
 */
static inline void _cgc_list_init_actual_init (cgc_list * const list, size_t element_size, cgc_copy_function copy_fun, cgc_clean_function clean_fun)
{
    list->_first = NULL;
    list->_last = NULL;
    list->_clean_fun = clean_fun;
    list->_copy_fun = copy_fun;
    list->_size = 0;
    list->_element_size = element_size;
}

/**
 * \brief Allocate a list element.
 * \return A pointer to a list element.
 * \retval \c NULL in case of failure.
 * \note This function may change the value of \c errno.
 * \note In case of failure, \c errno may be set to \c ENOMEM.
 */
static cgc_list_element * _cgc_list_element_alloc (void)
{
    cgc_list_element * const e = malloc (sizeof * e);
    if (e != NULL)
        * e = (cgc_list_element) { ._content = NULL, ._next = NULL, ._previous = NULL };

    return e;
}

/**
 * \brief Copy a content into a list element.
 * \param list A pointer to a CGC list.
 * \param e A pointer to a list element.
 * \param content A pointer to the content of interest.
 * \retval 0 in case of success.
 * \retval -3 in case of failure because of malloc.
 */
static int _cgc_list_copy_content (cgc_list * const list,  cgc_list_element * const e, const void * const content)
{
    int error = 0;
    e->_content = malloc (list->_element_size);
    if (e->_content == NULL)
        error = -3;
    else if (list->_copy_fun != NULL)
        list->_copy_fun (content, e->_content);
    else
        memcpy (e->_content, content, list->_element_size);

    return error;
}

/**
 * \brief Prelude for the push functions.
 * \param list A pointer to a CGC list.
 * \param content A pointer to the content that will be pushed.
 * \param e A pointer to a pointer to a list element.
 * \retval 0 in case of success.
 * \retval -1 if at least one of the pointers points to \c NULL.
 * \retval  -2 in case of failure due to malloc.
 * Check the pointers, allocate the list element.
 */
static int _cgc_list_push_prelude (cgc_list * const list, const void * const content, cgc_list_element ** e)
{
    int error = cgc_check_pointer (list);
    if (! error)
        error = cgc_check_pointer (content);

    if (! error)
    {
        * e = _cgc_list_element_alloc ();
        error = e != NULL ? _cgc_list_copy_content (list, * e, content) : -2;
    }

    return error;
}

/**
 * \brief Get the list element at the specified index if possible, or the last
 * element otherwise.
 * \param list A pointer to a CGC list.
 * \param i Target index.
 * \return A pointer to the targetted element.
 */
static inline cgc_list_element * _cgc_list_element_at (cgc_list * const list, size_t i)
{
    size_t j = 0;
    cgc_list_element * e;
    for (e = list->_first; j != i && e->_next != NULL; ++j)
        e = e->_next;

    return e;
}

/**
 * \brief Erase elements until \c i elements are erased.
 * \param list A pointer to a CGC list.
 * \param e The first element to erase.
 * \param i Number of elements to erase.
 * \return A pointer the successor of the last erased element.
 */
static inline cgc_list_element * _cgc_list_erase_until (cgc_list * const list, cgc_list_element * e, size_t i)
{
    size_t j = 0;
    for (e = e; j != i && e != NULL; j++)
    {
        cgc_list_element * next = e->_next;
        if (list->_clean_fun != NULL)
            list->_clean_fun (e->_content);
        free (e->_content);
        free (e);
        e = next;
    }

    return e;
}

////////////////////////////////////////////////////////////////////////////////
// New, free, copy.
////////////////////////////////////////////////////////////////////////////////

cgc_list * cgc_list_create (size_t element_size, cgc_copy_function copy_fun, cgc_clean_function clean_fun)
{
    cgc_list * list = malloc (sizeof * list);
    if (list != NULL)
        cgc_list_init (list, element_size, copy_fun, clean_fun);

    return list;
}

void cgc_list_destroy (cgc_list * const list)
{
    if (list != NULL)
    {
        cgc_list_clear (list);
        free (list);
    }
}

cgc_list * cgc_list_copy (const cgc_list * const list)
{
    cgc_list * copy = NULL;
    if (list != NULL)
    {
        copy = cgc_list_create (list->_element_size, list->_copy_fun, list->_clean_fun);
        if (! cgc_list_is_empty (list))
            for (cgc_list_element * e = list->_first; e != NULL; e = e->_next)
                cgc_list_push_back (copy, e->_content);
    }

    return copy;
}

int cgc_list_init (cgc_list * const list, size_t element_size, cgc_copy_function copy_fun, cgc_clean_function clean_fun)
{
    int error = cgc_check_pointer (list);

    if (! error)
        _cgc_list_init_actual_init (list, element_size, copy_fun, clean_fun);

    return error;
}

int cgc_list_clean (cgc_list * const list)
{
    return cgc_list_clear (list);
}

////////////////////////////////////////////////////////////////////////////////
// Properties getters.
////////////////////////////////////////////////////////////////////////////////

bool cgc_list_is_empty (const cgc_list * const list)
{
    return list->_first == NULL;
}

size_t cgc_list_size (const cgc_list * const list)
{
    return list->_size;
}

////////////////////////////////////////////////////////////////////////////////
// Access.
////////////////////////////////////////////////////////////////////////////////

void * cgc_list_at (const cgc_list * const list, size_t i)
{
    size_t j = 0;
    const cgc_list_element * e;
    for (e = list->_first; j < i && e != NULL; e = e->_next)
        j++;
    return e == NULL ? NULL : e->_content;
}

void * cgc_list_front (const cgc_list * const list)
{
    return list->_first->_content;
}

void * cgc_list_back (const cgc_list * const list)
{
    return list->_last->_content;
}

////////////////////////////////////////////////////////////////////////////////
// Modifiers.
////////////////////////////////////////////////////////////////////////////////

int cgc_list_push_front (cgc_list * const list, const void * const content)
{
    cgc_list_element * e = NULL;
    int error = _cgc_list_push_prelude (list, content, & e);

    if (! error)
    {
        e->_next = list->_first;
        if (list->_first != NULL)
            list->_first->_previous = e;
        list->_first = e;
        if (list->_last == NULL)
            list->_last = e;
        list->_size++;
    }

    return error;
}

int cgc_list_push_back (cgc_list * const list, const void * const content)
{
    cgc_list_element * e = NULL;
    int error = _cgc_list_push_prelude (list, content, & e);

    if (! error)
    {
        e->_previous = list->_last;
        if (list->_last != NULL)
            list->_last->_next = e;
        list->_last = e;
        if (list->_first == NULL)
            list->_first = e;
        list->_size++;
    }

    return error;
}

void * cgc_list_pop_front (cgc_list * const list)
{
    void * content = list->_first->_content;

    cgc_list_element * new_first = list->_first->_next;
    if (new_first != NULL)
        new_first->_previous = NULL;
    free (list->_first);
    list->_first = new_first;
    list->_size--;

    return content;
}

void * cgc_list_pop_back (cgc_list * const list)
{
    void * content = list->_last->_content;

    cgc_list_element * new_last = list->_last->_previous;
    if (new_last != NULL)
        new_last->_next = NULL;
    free (list->_last);
    list->_last = new_last;
    list->_size--;

    return content;
}

int cgc_list_insert (cgc_list * const list, size_t i, const void * const element)
{
    int error = 0;
    if (i == 0 || cgc_list_is_empty (list))
        error = cgc_list_push_front (list, element);
    else if (i >= cgc_list_size (list))
        error = cgc_list_push_back (list, element);
    else
    {
        cgc_list_element * new_e = _cgc_list_element_alloc ();
        error = new_e != NULL ? _cgc_list_copy_content (list, new_e, element) : -2;

        if (! error)
        {
            cgc_list_element * target = _cgc_list_element_at (list, i);
            cgc_list_element * previous = target->_previous;
            new_e->_previous = previous;
            previous->_next = new_e;
            new_e->_next = target;
            target->_previous = new_e;
            list->_size++;
        }
    }

    return error;
}

int cgc_list_clear (cgc_list * const list)
{
    return cgc_list_erase (list, 0, cgc_list_size (list));
}

int cgc_list_erase (cgc_list * const list, size_t start, size_t end)
{
    int error = cgc_check_pointer (list);
    if (! error)
    {
        cgc_list_element * current = _cgc_list_element_at (list, start);

        cgc_list_element * before_start = NULL;
        if (current != NULL)
            before_start = current->_previous;

        current = _cgc_list_erase_until (list, current, end - start);

        if (before_start == NULL)
            list->_first = before_start;
        else
            before_start->_next = current;

        if (current == NULL)
            list->_last = NULL;

        size_t deleted = end >= list->_size ? start : end - start;
        list->_size -= list->_size - deleted;
    }

    return error;
}

////////////////////////////////////////////////////////////////////////////////
// Functions on lists.
////////////////////////////////////////////////////////////////////////////////

void cgc_list_map (cgc_list * const list, cgc_unary_op_function op_fun)
{
    if (! cgc_list_is_empty (list))
        for (cgc_list_element * e = list->_first; e != NULL; e = e->_next)
            op_fun (e->_content);
}

void cgc_list_fold_left (const cgc_list * const list, cgc_binary_op_left_function op_fun, void * const base_result)
{
    if (! cgc_list_is_empty (list))
    {
        op_fun (base_result, list->_first->_content);
        for (cgc_list_element * e = list->_first->_next; e != NULL; e = e->_next)
            op_fun (base_result, e->_content);
    }
}

void cgc_list_fold_right (const cgc_list * const list, cgc_binary_op_right_function op_fun, void * const base_result)
{
    if (! cgc_list_is_empty (list))
    {
        op_fun (list->_first->_content, base_result);
        for (cgc_list_element * e = list->_last; e != NULL; e = e->_previous)
            op_fun (e->_content, base_result);
    }
}
