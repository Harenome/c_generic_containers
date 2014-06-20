/**
 * \file list.h
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
#ifndef _CGC_LIST_H_
#define _CGC_LIST_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <sysexits.h>

#include "cgc/common.h"
#include "cgc/types.h"

////////////////////////////////////////////////////////////////////////////////
// Typedefs.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief CGC List element.
 */
typedef struct cgc_list_element
{
    void * _content;    /**<- Content. */
    void * _next;       /**<- Next element. */
    void * _previous;   /**<- Previous element. */
} cgc_list_element;

/**
 * \brief CGC List.
 */
typedef struct cgc_list
{
    cgc_list_element * _first;          /**<- First element. */
    cgc_list_element * _last;           /**<- Last element. */
    cgc_alloc_function _alloc_fun;      /**<- Allocation function. */
    cgc_free_function _free_fun;        /**<- Free function. */
    cgc_copy_function _copy_fun;        /**<- Copy function. */
    size_t _size;                       /**<- Size. */
} cgc_list;

////////////////////////////////////////////////////////////////////////////////
// New, free, copy.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Create a new cgc_list
 * \param alloc_fun Memory allocation function.
 * \param free_fun Free function.
 * \param copy_fun Copy function.
 * \relatesalso cgc_list
 * \return pointer to a cgc_list.
 */
cgc_list * cgc_list_new (cgc_alloc_function alloc_fun, cgc_free_function free_fun, cgc_copy_function copy_fun);

/**
 * \brief Free a cgc_list.
 * \param list List.
 * \relatesalso cgc_list
 */
void cgc_list_free (cgc_list * list);

/**
 * \brief Copy a list.
 * \param list List.
 * \return copy.
 * \relatesalsoo cgc_list
 */
cgc_list * cgc_list_copy (const cgc_list * list);

int cgc_list_init (cgc_list * list, cgc_alloc_function alloc_fun, cgc_free_function free_fun, cgc_copy_function copy_fun);

////////////////////////////////////////////////////////////////////////////////
// Properties getters.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Check whether a CGC list is empty.
 * \param list List.
 * \retval true if the list is empty.
 * \retval false otherwise.
 * \relatesalso cgc_list
 */
bool cgc_list_is_empty (const cgc_list * list);

/**
 * \brief Get the size of the list.
 * \param list List.
 * \return size.
 * \relatesalso cgc_list
 */
size_t cgc_list_size (const cgc_list * list);

////////////////////////////////////////////////////////////////////////////////
// Access.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Get the element at index \c i.
 * \param list List.
 * \param i Index.
 * \return element.
 * \relatesalso cgc_list
 */
void * cgc_list_at (const cgc_list * list, size_t i);

/**
 * \brief Get the front of the list.
 * \param list List.
 * \return Front.
 * \relatesalso cgc_list
 */
void * cgc_list_front (const cgc_list * list);

/**
 * \brief Get the back of the list.
 * \param list List.
 * \return Back.
 * \relatesalso cgc_list
 */
void * cgc_list_back (const cgc_list * list);

////////////////////////////////////////////////////////////////////////////////
// Modifiers.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Push to the front.
 * \param list List
 * \param element Element.
 * \retval 0 in case of success
 * \relatesalso cgc_list
 */
int cgc_list_push_front (cgc_list * list, void * element);

/**
 * \brief Push to the back.
 * \param list List
 * \param element Element.
 * \retval 0 in case of success
 * \relatesalso cgc_list
 */
int cgc_list_push_back (cgc_list * list, void * element);

/**
 * \brief Pop the front.
 * \param list List.
 * \return size.
 * \relatesalso cgc_list
 */
void * cgc_list_pop_front (cgc_list * list);

/**
 * \brief Pop the back.
 * \param list List.
 * \return size.
 * \relatesalso cgc_list
 */
void * cgc_list_pop_back (cgc_list * list);

int cgc_list_insert (cgc_list * list, size_t i, void * element);

int cgc_list_clear (cgc_list * list);

int cgc_list_erase (cgc_list * list, size_t start, size_t end);

////////////////////////////////////////////////////////////////////////////////
// Functions on lists.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Map.
 * \param list List
 * \param op_fun Operation.
 * \relatesalso cgc_list
 */
void cgc_list_map (cgc_list * list, cgc_unary_op_function op_fun);

/**
 * \brief Fold left.
 * \param list List.
 * \param op_fun Binary operation.
 * \param base Base.
 * \param result Result.
 * \relatesalso cgc_list
 */
void cgc_list_fold_left (const cgc_list * list, cgc_binary_op_left_function op_fun, void * base_result);

/**
 * \brief Fold right.
 * \param list List.
 * \param op_fun Binary operation.
 * \param base Base.
 * \param result Result.
 * \relatesalso cgc_list
 */
void cgc_list_fold_right (const cgc_list * list, cgc_binary_op_right_function op_fun, void * base_result);

#endif /* _CGC_LIST_H_ */
