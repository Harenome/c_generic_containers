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
 *
 * List elements are used internally to store the elements of lists. An element
 * contains a content and is linked to its predecessor (if any) in the list and
 * its successor (if any) in the list.
 */
typedef struct cgc_list_element
{
    void * _content;    /**<- Content. */
    void * _next;       /**<- Next element. */
    void * _previous;   /**<- Previous element. */
} cgc_list_element;

/**
 * \brief CGC List.
 *
 * CGC Lists are generic lists. These lists are implemented as doubly linked 
 * lists.
 *
 * # Basics
 * To achieve genericity, CGC Lists rely on the use of 3 functions:
 * - a memory allocation function
 * - a free function
 * - a copy function
 * It is up to the user to properly write these 3 functions in order to create
 * a valid CGC List.
 *
 * ## Allocation function
 * A proper allocation function is required to allocate the memory which will be
 * used to store the elements of the list.
 *
 * ### Prototype
 * The prototype of this function must be:
 *
 *      void * alloc_function (void);
 *
 * It shall return a pointer to memory wide enough to hold whatever the user
 * wishes to store as an element of the list.
 *
 * ## Free function
 * A dedicated free function is required since the CGC List aims to be generic:
 * users should be able to store any type of element in the list. Elements could
 * themselves point to dynamically allocated memory. Thus, this function shall,
 * if needed, free any memory an element of the list could supposedly hold, and
 * then free the element itself.
 * ### Prototype
 * The prototype of this function must be:
 *
 *      void free_function (void *);
 *
 * ## Copy function
 * Since the CGC List could possibly hold any type of element, the user must
 * supply a way to properly copy elements into the list.
 * ### Prototype
 * The prototype of this function must be:
 *
 *      int copy_function (const void *, void *);
 *
 * The first argument is the original element, while the second argument is the
 * destination (the copy).
 * This function shall assume the memory for the copy has already been allocated.
 *
 * # CGC List creation and destruction
 * ## Dynamic allocation
 * CGC Lists can be dynamically created using cgc_list_new(). Such lists must be
 * destroyed using cgc_list_free().
 * For instance:
 *
 *      cgc_list * list = cgc_list_new (alloc_fun, free_fun, copy_fun);
 *      // Use the list.
 *      cgc_list_free (list);
 *
 * ## Static allocation
 * CGC Lists can also be statically declared. In such cases, one must initialize
 * it with cgc_list_init(), and clean it with cgc_list_clean().
 * For instance:
 *
 *      cgc_list list;
 *      cgc_list_init (& list, alloc_fun, free_fun, copy_fun);
 *      // Use the list.
 *      cgc_list_clean (& list);
 *
 * ## Note on dynamic lists and cleaning
 * Note that cgc_list_free() actually is equivalent to calling cgc_list_clean()
 * and then manually freeing the dynamically allocated list using the standard
 * library's \c free.
 *
 * ## Copying a list
 * Lists can be copied using cgc_list_copy().
 *
 * # List properties
 * One can test whether a list is empty with cgc_list_is_empty() and get the
 * size of a list with cgc_list_size().
 *
 * # Element access
 * ## Without changing the list
 * There are three functions that return pointers to elements of the list.
 * Please be aware that the pointers directly point to the content of the list:
 * modifying the accessed element is fine, but freeing it will most likely lead
 * to serious injuries (because at the time of cleaning or freeing, the list
 * will try to free all of its remaining elements).
 *
 * Access function  | Pointer to
 -------------------|--------------
 * cgc_list_at()    | Nth element
 * cgc_list_front() | First element
 * cgc_list_back()  | Last element
 *
 * Note that these functions do not:
 * - check whether the supplied list is valid
 * - check whether the requested element exists
 *
 * Thus, it is necessery to supply valid lists, and to check the list's size
 * before trying to access an element.
 *
 * ## While modifying the list
 * It is also possible to get an element while modifying the list: the first
 * or the last element can be accessed while removing it from the list. Elements
 * accessed in this fashion shall be freed by the user.
 *
 * Access function      | Element
 * ---------------------|---------------
 * cgc_list_pop_front() | First element
 * cgc_list_pop_back()  | Last element
 *
 * # Modification
 * ## Insertions
 *
 * Insertion function       | Insertion
 * -------------------------|------------------------
 * cgc_list_push_front()    | As the first element
 * cgc_list_push_back()     | As the last element
 * cgc_list_insert()        | Before the Nth element
 *
 * ## Deletions
 *
 * Deletion function    | Deleted elements
 * ---------------------|-----------------------------
 * cgc_list_pop_front() | First element
 * cgc_list_pop_back()  | Last element
 * cgc_list_erase()     | Several contiguous elements
 * cgc_list_clear()     | All elements
 *
 * As stated before, cgc_list_pop_front() and cgc_list_pop_back() remove an
 * element from the list, but do not free it.
 * On the other hand, cgc_list_erase() and cgc_list_clear() do free the
 * deleted elements.
 *
 * # Operations on lists
 * Those familiar with functionnal programming will probably be happy to know
 * that the map and fold functions are available. However, do not expect perfect
 * reproduction of their functionnal programming counterparts.
 *
 * ## Map
 * The map function for CGC Lists is cgc_list_map(). It expects a list, and an
 * operation function.
 * The prototype of the operation function must be:
 *
 *      int operation (void *)
 *
 * This operation directly modifies the supplied element. Thus, the list is
 * directly modified. If the original list must be kept, one should first copy
 * it with cgc_list_copy().
 *
 * It is strongly recommended that the “type” of the element is not changed by
 * the \c operation function. Indeed, the cgc_list is initialized with specific
 * allocation, free and copy functions. These functions may not work well on
 * different types of elements.
 *
 * ## Folds
 * The fold functions for CGC Lists are cgc_list_fold_left() and
 * cgc_list_fold_right(). These functions both expect a list, a binary
 * operation and an initial value. The prototypes of the binary operations are:
 *
 *      int fold_right_operation (const void *, void *);
 *      int fold_left_operation (const void *, void *);
 *
 * These operations directly modify one of their argument. Thus the initial
 * value supplied to the fold function is likely to be modified and will hold
 * the result of the fold.
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
// New, free, copy, initialization, cleaning.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Create a new cgc_list
 * \param alloc_fun Memory allocation function.
 * \param free_fun Free function.
 * \param copy_fun Copy function.
 * \relatesalso cgc_list
 * \return pointer to a cgc_list.
 * \retval NULL if the list could not be allocated.
 * \note Lists obtained this way must be freed using cgc_list_free().
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
 * \relatesalso cgc_list
 * \retval NULL if the list could not be copied.
 * \note Lists obtained this way must be freed using cgc_list_free().
 */
cgc_list * cgc_list_copy (const cgc_list * list);

/**
 * \brief Initialize a cgc_list.
 * \param list List.
 * \param alloc_fun Memory allocation function.
 * \param free_fun Free function.
 * \param copy_fun Copy function.
 * \relatesalso cgc_list
 * \return This function shall return 0 in case of success, a negative integer
 * in case of failure.
 * \retval 0 in case of success.
 * \retval -1 if one of the arguments is NULL. \c errno shall be set to \c EINVAL.
 */
int cgc_list_init (cgc_list * list, cgc_alloc_function alloc_fun, cgc_free_function free_fun, cgc_copy_function copy_fun);

/**
 * \brief Clean a cgc_list.
 * \param list List.
 * \return This function shall return 0 in case of success, a negative integer
 * in case of failure.
 * \retval 0 in case of success.
 * \retval -1 if one of the arguments is NULL. \c errno shall be set to \c EINVAL.
 */
int cgc_list_clean (cgc_list * list);

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
 * \warning This function does not check whether the supplied list is valid!
 * \warning This function does not check whether the supplied list is long enough!
 */
void * cgc_list_at (const cgc_list * list, size_t i);

/**
 * \brief Get the front of the list.
 * \param list List.
 * \return Front.
 * \relatesalso cgc_list
 * \warning This function does not check whether the supplied list is valid!
 * \warning This function does not check whether the supplied list is long enough!
 */
void * cgc_list_front (const cgc_list * list);

/**
 * \brief Get the back of the list.
 * \param list List.
 * \return Back.
 * \relatesalso cgc_list
 * \warning This function does not check whether the supplied list is valid!
 * \warning This function does not check whether the supplied list is long enough!
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
 * \retval -1 if one of the arguments is NULL. \c errno shall be set to \c EINVAL.
 * \retval -2 if the the function failed because memory allocation failed. \c
 * errno may be set to \c ENOMEM.
 * \relatesalso cgc_list
 * \return This function shall return 0 in case of success, a negative integer
 * in case of failure.
 * \note The supplied element will be copied into the list. It is safe to free
 * \c element afterwards.
 */
int cgc_list_push_front (cgc_list * list, void * element);

/**
 * \brief Push to the back.
 * \param list List
 * \param element Element.
 * \retval 0 in case of success
 * \retval -1 if one of the arguments is NULL. \c errno shall be set to \c EINVAL.
 * \retval -2 if the the function failed because memory allocation failed. \c
 * errno may be set to \c ENOMEM.
 * \relatesalso cgc_list
 * \return This function shall return 0 in case of success, a negative integer
 * in case of failure.
 * \note The supplied element will be copied into the list. It is safe to free
 * \c element afterwards.
 */
int cgc_list_push_back (cgc_list * list, void * element);

/**
 * \brief Pop the front.
 * \param list List.
 * \return First element.
 * \relatesalso cgc_list
 * \warning It is up to the user to free the returned element once unneeded.
 */
void * cgc_list_pop_front (cgc_list * list);

/**
 * \brief Pop the back.
 * \param list List.
 * \return Last element.
 * \relatesalso cgc_list
 * \warning It is up to the user to free the returned element once unneeded.
 */
void * cgc_list_pop_back (cgc_list * list);

/**
 * \brief Insert an element a the Ith place in a list.
 * \param list List.
 * \param i Target index.
 * \param element Element.
 * \relatesalso cgc_list
 * \return This function shall return 0 in case of success, a negative integer
 * in case of failure.
 * \retval 0 in case of success.
 * \retval -1 if one of the arguments is NULL. \c errno shall be set to \c EINVAL.
 * \retval -2 if the the function failed because memory allocation failed. \c
 * errno may be set to \c ENOMEM.
 * \note If \c i is greater than the size of the list, the \c element will be
 * inserted at the end of the list.
 * \note The supplied element will be copied into the list. It is safe to free
 * \c element afterwards.
 */
int cgc_list_insert (cgc_list * list, size_t i, void * element);

/**
 * \brief Clear a list.
 * \param list List.
 * \relatesalso cgc_list
 * \return This function shall return 0 in case of success, a negative integer
 * in case of failure.
 * \retval 0 in case of success.
 * \retval -1 if one of the arguments is NULL. \c errno shall be set to \c EINVAL.
 */
int cgc_list_clear (cgc_list * list);

/**
 * \brief Erase elements from start to end excluded from a list.
 * \param list List.
 * \param start First element to remove.
 * \param end Element after the last element to remove.
 * \relatesalso cgc_list
 * \return This function shall return 0 in case of success, a negative integer
 * in case of failure.
 * \retval 0 in case of success.
 * \retval -1 if one of the arguments is NULL. \c errno shall be set to \c EINVAL.
 */
int cgc_list_erase (cgc_list * list, size_t start, size_t end);

////////////////////////////////////////////////////////////////////////////////
// Functions on lists.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Map.
 * \param list List
 * \param op_fun Operation.
 * \relatesalso cgc_list
 * \note The list will be modified. First copy the list if the original must
 * be kept.
 */
void cgc_list_map (cgc_list * list, cgc_unary_op_function op_fun);

/**
 * \brief Fold left.
 * \param list List.
 * \param op_fun Binary operation.
 * \param base_result Initial value and result.
 * \relatesalso cgc_list
 * \note \c base_result must hold the initial value before the call to this
 * function but will be modified to hold the result.
 */
void cgc_list_fold_left (const cgc_list * list, cgc_binary_op_left_function op_fun, void * base_result);

/**
 * \brief Fold right.
 * \param list List.
 * \param op_fun Binary operation.
 * \param base_result Initial value and result.
 * \relatesalso cgc_list
 * \note \c base_result must hold the initial value before the call to this
 * function but will be modified to hold the result.
 */
void cgc_list_fold_right (const cgc_list * list, cgc_binary_op_right_function op_fun, void * base_result);

#endif /* _CGC_LIST_H_ */
