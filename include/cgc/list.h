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

typedef struct cgc_list_element
{
    void * _content;
    void * _next;
    void * _previous;
} cgc_list_element;

typedef struct cgc_list
{
    cgc_list_element * _first;
    cgc_list_element * _last;
    cgc_alloc_function _alloc_fun;
    cgc_free_function _free_fun;
    cgc_copy_function _copy_fun;
} cgc_list;

cgc_list * cgc_list_new (cgc_alloc_function alloc_fun, cgc_free_function free_fun, cgc_copy_function copy_fun);
void cgc_list_free (cgc_list * list);

bool cgc_list_is_empty (const cgc_list * list);
const void * cgc_list_front (const cgc_list * list);
const void * cgc_list_back (const cgc_list * list);
size_t cgc_list_size (const cgc_list * list);

void * cgc_list_pop_front (cgc_list * list);
void * cgc_list_pop_back (cgc_list * list);
int cgc_list_push_front (cgc_list * list, void * element);
int cgc_list_push_back (cgc_list * list, void * element);

cgc_list * cgc_list_copy (const cgc_list * list);

void cgc_list_map (cgc_list * list, cgc_unary_op_function op_fun);
void cgc_list_fold_left (const cgc_list * list, cgc_binary_op_function op_fun, void * base, void * result);
void cgc_list_fold_right (const cgc_list * list, cgc_binary_op_function op_fun, void * base, void * result);

#endif /* _CGC_LIST_H_ */
