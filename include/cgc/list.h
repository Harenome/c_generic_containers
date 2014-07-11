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
#include <string.h>

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
 * To achieve genericity, CGC Lists rely on 3 things:
 * - the size in bytes of the elements
 * - a copy function
 * - a cleaning function
 *
 * It is up to the user to properly write these 2 functions, if needed,
 * in order to create a valid CGC List.
 *
 * ## Element allocation
 * CGC Lists will dynamically allocate memory for its elements. This allocation
 * depends on the size provided at the list creation.
 *
 * ## Copy function
 * Since the CGC List could possibly hold any type of element, it might be
 * necessary to supply a way to properly copy elements into the list.
 *
 * If no copy function was provided at the list creation, a mere copy (using
 * \c memcpy) of the provided element will be done.
 *
 * For further information, see #cgc_copy_function.
 *
 * ## Cleaning function
 * A cleaning function may be required since the CGC List aims to be generic:
 * users should be able to store any type of element in the list. Elements could
 * themselves point to dynamically allocated memory. Thus, this function shall,
 * if needed, free any memory an element of the list could supposedly hold.
 *
 * Note that the function should free any dynamically allocated memory held by
 * the list element but should not free the aforementioned element.
 *
 * For further information, set #cgc_clean_function.
 *
 * # CGC List creation and destruction
 * ## Dynamic allocation
 * CGC Lists can be dynamically created using cgc_list_create(). Such lists must be
 * destroyed using cgc_list_destroy().
 *
 * ### Simple examples
 * #### Integer list
 * One can create a list of integers as follows:
 *
 *     // Create a list: give the size of an int, no copy or cleaning function.
 *     cgc_list * list = cgc_list_create (sizeof (int), NULL, NULL);
 *     // ...
 *     // Use the list...
 *     // ...
 *     // Free the list once unneeded.
 *     cgc_list_destroy (list);
 *
 * #### Struct list
 * Simple structs can also be stored in a list:
 *
 *     // Define a struct.
 *     struct my_struct
 *     {
 *         int _first_field;
 *         int _second_field;
 *     };
 *
 *     // Create a list.
 *     cgc_list * list = cgc_list_create (sizeof (struct my_struct), NULL, NULL);
 *     // ...
 *     // Use the list...
 *     // ...
 *     // Free the list once unneeded.
 *     cgc_list_destroy (list);
 *
 * ### Complex example
 * Sometimes, the element holds dynamically allocated memory.
 *
 *     // Define a struct.
 *     struct my_struct
 *     {
 *         int _static;         // An integer.
 *         int * _dynamic;      // A pointer to an integer.
 *     };
 *
 *     // Define a copy function.
 *     // It shall assume the memory is already allocated.
 *     int my_struct_copy (const void * source, void * destination)
 *     {
 *         // Cast to know the actual size.
 *         const my_struct * const s = source;
 *         my_struct * const d = destination;
 *
 *         // Copy the contents.
 *         * d = * s;
 *
 *         // Now, d->_dynamic == s->_dynamic is true.
 *         // But it is a pointer...
 *         d->_dynamic = malloc (sizeof (int));
 *         * d->_dynamic = * s->_dynamic;

 *         return 0;
 *     }
 *
 *     // Define a free function.
 *     void my_struct_clean (void * pointer)
 *     {
 *         // The ._dynamic field points to a malloc'd memory.
 *         struct my_struct * my_s = pointer;
 *         free (my_s->_dynamic);
 *     }
 *
 *     // Create a list.
 *     cgc_list * list = cgc_list_create (sizeof (struct my_struct), my_struct_copy, my_struct_clean);
 *     // ...
 *     // Use the list...
 *     // ...
 *     // Free the list.
 *     cgc_list_destroy (list);
 *
 * ## Static allocation
 * CGC Lists can also be statically declared. In such cases, one must initialize
 * it with cgc_list_init(), and clean it with cgc_list_clean().
 * For instance:
 *
 *      cgc_list list;
 *      cgc_list_init (& list, sizeof (int), copy_fun, clean_fun);
 *      // Use the list.
 *      cgc_list_clean (& list);
 *
 * ## Note on dynamic lists and cleaning
 * Note that cgc_list_destroy() actually is equivalent to calling cgc_list_clean()
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
 * -----------------|--------------
 * cgc_list_at()    | Nth element
 * cgc_list_front() | First element
 * cgc_list_back()  | Last element
 *
 * Note that these functions do not:
 * - check whether the supplied list is valid
 * - check whether the requested element exists
 *
 * Thus, it is necessary to supply valid lists, and to check the list's size
 * before trying to access an element.
 *
 * ## While modifying the list
 * It is also possible to get an element while modifying the list: the first
 * or the last element can be accessed while removing it from the list.
 *
 * \b Important: Elements accessed in this fashion shall be freed by the user.
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
 * that the \c map and \c fold functions are available. However, do not expect
 * perfect reproduction of their functionnal programming counterparts.
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
 *      int fold_left_operation (void *, const void *);
 *
 * These operations directly modify one of their argument. Thus the initial
 * value supplied to the fold function is likely to be modified and will hold
 * the result of the fold.
 */
typedef struct cgc_list
{
    cgc_list_element * _first;          /**<- First element. */
    cgc_list_element * _last;           /**<- Last element. */
    cgc_clean_function _clean_fun;      /**<- Clean function. */
    cgc_copy_function _copy_fun;        /**<- Copy function. */
    size_t _size;                       /**<- Size. */
    size_t _element_size;               /**<- Element size. */
} cgc_list;

////////////////////////////////////////////////////////////////////////////////
// New, free, copy, initialization, cleaning.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Create a new cgc_list.
 * \param element_size Element size.
 * \param copy_fun Copy function.
 * \param clean_fun Cleaning function.
 * \relatesalso cgc_list
 * \return The pointer to the new cgc_list in case of success. \c NULL in case
 * of failure.
 * \retval NULL if the list could not be allocated.
 * \note Lists obtained this way must be freed using cgc_list_destroy().
 * \note In case of failure, \c errno may be set to \c ENOMEM.
 * \note A call to this function may change the value of \c errno.
 */
cgc_list * cgc_list_create (size_t element_size, cgc_copy_function copy_fun, cgc_clean_function clean_fun);

/**
 * \brief Free a dynamically allocated cgc_list.
 * \param list List.
 * \relatesalso cgc_list
 * \note A call to this function may change the value of \c errno.
 * \note It is safe to pass a \c NULL to this function.
 * \warning It is strongly advised to use this function only of lists obtained
 * via cgc_list_create().
 */
void cgc_list_destroy (cgc_list * list);

/**
 * \brief Copy a list.
 * \param list List.
 * \return A pointer to the copy in case of success. \c NULL in case of failure.
 * \relatesalso cgc_list
 * \note It is safe to pass a \c NULL to this function.
 * \retval NULL if the list could not be copied or \c list was \c NULL.
 * \note Lists obtained this way must be freed using cgc_list_destroy().
 * \note A call to this function may change the value of \c errno.
 */
cgc_list * cgc_list_copy (const cgc_list * list);

/**
 * \brief Initialize a cgc_list.
 * \param[in,out] list List.
 * \param[in] element_size Element size
 * \param[in] copy_fun Copy function.
 * \param[in] clean_fun Cleaning function.
 * \relatesalso cgc_list
 * \return This function shall return 0 in case of success, a negative integer
 * in case of failure.
 * \retval 0 in case of success.
 * \retval -1 if one of the arguments is \c NULL. \c errno shall be set to
 * \c EINVAL.
 */
int cgc_list_init (cgc_list * list, size_t element_size, cgc_copy_function copy_fun, cgc_clean_function clean_fun);

/**
 * \brief Clean a cgc_list.
 * \param list List.
 * \return This function shall return 0 in case of success, a negative integer
 * in case of failure.
 * \retval 0 in case of success.
 * \retval -1 if one of the arguments is \c NULL. \c errno shall be set to
 * \c EINVAL.
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
 * \pre cgc_list_size(list) > \c i
 * \pre list != NULL
 * \warning This function does not check whether the supplied list is valid!
 * \warning This function does not check whether the supplied list is long enough!
 */
void * cgc_list_at (const cgc_list * list, size_t i);

/**
 * \brief Get the front of the list.
 * \param list List.
 * \return Front.
 * \relatesalso cgc_list
 * \pre cgc_list_is_empty(list) == \c false
 * \pre list != NULL
 * \warning This function does not check whether the supplied list is valid!
 * \warning This function does not check whether the supplied list is long enough!
 */
void * cgc_list_front (const cgc_list * list);

/**
 * \brief Get the back of the list.
 * \param list List.
 * \return Back.
 * \relatesalso cgc_list
 * \pre cgc_list_is_empty(list) == \c false
 * \pre list != NULL
 * \warning This function does not check whether the supplied list is valid!
 * \warning This function does not check whether the supplied list is long enough!
 */
void * cgc_list_back (const cgc_list * list);

////////////////////////////////////////////////////////////////////////////////
// Modifiers.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Push to the front.
 * \param[in,out] list List
 * \param[in] element Element.
 * \retval 0 in case of success
 * \retval -1 if one of the arguments is \c NULL. \c errno shall be set to
 * \c EINVAL.
 * \retval -2 if the the function failed because memory allocation failed. \c
 * errno may be set to \c ENOMEM.
 * \relatesalso cgc_list
 * \return This function shall return 0 in case of success, a negative integer
 * in case of failure.
 * \note The supplied element will be copied into the list. It is safe to free
 * \c element afterwards.
 */
int cgc_list_push_front (cgc_list * list, const void * element);

/**
 * \brief Push to the back.
 * \param[in,out] list List
 * \param[in] element Element.
 * \retval 0 in case of success
 * \retval -1 if one of the arguments is \c NULL. \c errno shall be set to
 * \c EINVAL.
 * \retval -2 if the the function failed because memory allocation failed. \c
 * errno may be set to \c ENOMEM.
 * \relatesalso cgc_list
 * \return This function shall return 0 in case of success, a negative integer
 * in case of failure.
 * \note The supplied element will be copied into the list. It is safe to free
 * \c element afterwards.
 */
int cgc_list_push_back (cgc_list * list, const void * element);

/**
 * \brief Pop the front.
 * \param list List.
 * \return First element.
 * \relatesalso cgc_list
 * \warning It is up to the user to free the returned element once unneeded.
 * \pre list != NULL
 * \pre cgc_list_is_empty(list) == \c false
 */
void * cgc_list_pop_front (cgc_list * list);

/**
 * \brief Pop the back.
 * \param list List.
 * \return Last element.
 * \relatesalso cgc_list
 * \warning It is up to the user to free the returned element once unneeded.
 * \pre list != NULL
 * \pre cgc_list_is_empty(list) == \c false
 */
void * cgc_list_pop_back (cgc_list * list);

/**
 * \brief Insert an element a the Ith place in a list.
 * \param[in,out] list List.
 * \param[in] i Target index.
 * \param[in] element Element.
 * \relatesalso cgc_list
 * \return This function shall return 0 in case of success, a negative integer
 * in case of failure.
 * \retval 0 in case of success.
 * \retval -1 if one of the arguments is \c NULL. \c errno shall be set to
 * \c EINVAL.
 * \retval -2 if the function failed because memory allocation failed. \c errno
 * may be set to \c ENOMEM.
 * \note If \c i is greater than the size of the list, the \c element will be
 * inserted at the end of the list.
 * \note The supplied element will be copied into the list. It is safe to free
 * \c element afterwards, if it was malloc'd.
 */
int cgc_list_insert (cgc_list * list, size_t i, const void * element);

/**
 * \brief Clear a list.
 * \param list List.
 * \relatesalso cgc_list
 * \return This function shall return 0 in case of success, a negative integer
 * in case of failure.
 * \retval 0 in case of success.
 * \retval -1 if one of the arguments is \c NULL. \c errno shall be set to
 * \c EINVAL.
 */
int cgc_list_clear (cgc_list * list);

/**
 * \brief Erase elements from start to end excluded from a list.
 * \param[in,out] list List.
 * \param[in] start First element to remove.
 * \param[in] end Element after the last element to remove.
 * \relatesalso cgc_list
 * \return This function shall return 0 in case of success, a negative integer
 * in case of failure.
 * \retval 0 in case of success.
 * \retval -1 if one of the arguments is \c NULL. \c errno shall be set to
 * \c EINVAL.
 */
int cgc_list_erase (cgc_list * list, size_t start, size_t end);

////////////////////////////////////////////////////////////////////////////////
// Functions on lists.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Map.
 * \param[in,out] list List
 * \param[in] op_fun Operation.
 * \relatesalso cgc_list
 * \note The list will be modified. First copy the list if the original must
 * be kept.
 */
void cgc_list_map (cgc_list * list, cgc_unary_op_function op_fun);

/**
 * \brief Fold left.
 * \param[in] list List.
 * \param[in] op_fun Binary operation.
 * \param[in,out] base_result Initial value and result.
 * \relatesalso cgc_list
 * \note \c base_result must hold the initial value before the call to this
 * function but will be modified to hold the result.
 */
void cgc_list_fold_left (const cgc_list * list, cgc_binary_op_left_function op_fun, void * base_result);

/**
 * \brief Fold right.
 * \param[in] list List.
 * \param[in] op_fun Binary operation.
 * \param[in,out] base_result Initial value and result.
 * \relatesalso cgc_list
 * \note \c base_result must hold the initial value before the call to this
 * function but will be modified to hold the result.
 */
void cgc_list_fold_right (const cgc_list * list, cgc_binary_op_right_function op_fun, void * base_result);

#endif /* _CGC_LIST_H_ */
