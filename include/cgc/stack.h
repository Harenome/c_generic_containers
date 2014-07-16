/**
 * \file stack.h
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
#ifndef _CGC_STACK_H_
#define _CGC_STACK_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <sysexits.h>
#include <string.h>

#include "cgc/common.h"
#include "cgc/types.h"
#include "cgc/list.h"

/**
 * \defgroup stacks_group Stacks
 */

////////////////////////////////////////////////////////////////////////////////
// Typedefs.
////////////////////////////////////////////////////////////////////////////////

/**
 * \class cgc_stack
 * \ingroup stacks_group
 * \brief CGC Stack.
 *
 * CGC Stacks are akin to CGC Lists, but allow less operations.
 *
 * CGC Stack function        | CGC List equivalent
 * --------------------------|-----------------------
 * cgc_stack_create()        | cgc_list_create()
 * cgc_stack_destroy()       | cgc_list_destroy()
 * cgc_stack_init()          | cgc_list_init()
 * cgc_stack_clean()         | cgc_list_clean()
 * cgc_stack_copy()          | cgc_list_copy()
 * cgc_stack_copy_into()     | cgc_list_copy_into()
 * cgc_stack_swap()          | cgc_list_swap()
 * cgc_stack_front()         | cgc_list_front()
 * cgc_stack_back()          | cgc_list_back()
 * cgc_stack_is_empty()      | cgc_list_is_empty()
 * cgc_stack_size()          | cgc_list_size()
 * cgc_stack_push()          | cgc_list_push_back()
 * cgc_stack_pop()           | cgc_list_pop_front()
 * cgc_stack_clear()         | cgc_list_clear()
 *
 * \sa cgc_list
 */

/**
 * \brief cgc_stack
 */
typedef cgc_list cgc_stack;

////////////////////////////////////////////////////////////////////////////////
// Dynamic creation and destruction.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Create a new cgc_stack.
 * \param element_size Element size.
 * \param copy_fun Copy function.
 * \param clean_fun Cleaning function.
 * \relatesalso cgc_stack
 * \return The pointer to the new cgc_stack in case of success. \c NULL in case
 * of failure.
 * \retval NULL if the stack could not be allocated.
 * \note Stacks obtained this way must be freed using cgc_stack_destroy().
 * \note In case of failure, \c errno may be set to \c ENOMEM.
 * \note A call to this function may change the value of \c errno.
 */
cgc_stack * cgc_stack_create (size_t element_size, cgc_copy_function copy_fun, cgc_clean_function clean_fun);

/**
 * \brief Free a dynamically allocated cgc_stack.
 * \param stack Stack.
 * \relatesalso cgc_stack
 * \note A call to this function may change the value of \c errno.
 * \note It is safe to pass a \c NULL to this function.
 * \warning It is strongly advised to use this function only of stacks obtained
 * via cgc_stack_create().
 */
void cgc_stack_destroy (cgc_stack * stack);

////////////////////////////////////////////////////////////////////////////////
// Initialization and cleaning.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Initialize a cgc_stack.
 * \param[in,out] stack Stack.
 * \param[in] element_size Element size
 * \param[in] copy_fun Copy function.
 * \param[in] clean_fun Cleaning function.
 * \relatesalso cgc_stack
 * \return This function shall return 0 in case of success, a negative integer
 * in case of failure.
 * \retval 0 in case of success.
 * \retval -1 if one of the arguments is \c NULL. \c errno shall be set to
 * \c EINVAL.
 */
int cgc_stack_init (cgc_stack * stack, size_t element_size, cgc_copy_function copy_fun, cgc_clean_function clean_fun);

/**
 * \brief Clean a cgc_stack.
 * \param[in,out] stack Stack.
 * \return This function shall return 0 in case of success, a negative integer
 * in case of failure.
 * \retval 0 in case of success.
 * \retval -1 if one of the arguments is \c NULL. \c errno shall be set to
 * \c EINVAL.
 * \relatesalso cgc_stack
 */
int cgc_stack_clean (cgc_stack * stack);

////////////////////////////////////////////////////////////////////////////////
// Copy.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Copy a stack.
 * \param stack Stack.
 * \return A pointer to the copy in case of success. \c NULL in case of failure.
 * \relatesalso cgc_stack
 * \note It is safe to pass a \c NULL to this function.
 * \retval NULL if the stack could not be copied or \c stack was \c NULL.
 * \note Stacks obtained this way must be freed using cgc_stack_destroy().
 * \note A call to this function may change the value of \c errno.
 */
cgc_stack * cgc_stack_copy (const cgc_stack * stack);

/**
 * \brief Copy a cgc_stack into \c destination.
 * \param[in] original The original.
 * \param[in,out] destination The destination of the copy.
 * \retval 0 in case of success.
 * \retval -1 if one of the arguments is \c NULL. \c errno may be set to \c EINVAL.
 * \relatesalso cgc_stack
 * \warning This function will overwrite the destination! It is up to the user to
 * first clean \c destination if needed.
 */
int cgc_stack_copy_into (const cgc_stack * original, cgc_stack * destination);

////////////////////////////////////////////////////////////////////////////////
// Swap.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Swap the contents of two stacks.
 * \param[in,out] a First stack.
 * \param[in,out] b Second stack.
 * \relatesalso cgc_stack
 * \pre a != NULL && b != NULL
 * \return 0 in case of success.
 * \return -1 if at least of the argumes is \c NULL. \c errno may be set to
 * \c EINVAL.
 * \note A cal to this function may change the value of \c errno.
 */
int cgc_stack_swap (cgc_stack * a, cgc_stack * b);

////////////////////////////////////////////////////////////////////////////////
// Properties getters.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Check whether a CGC stack is empty.
 * \param stack Stack.
 * \retval true if the stack is empty.
 * \retval false otherwise.
 * \relatesalso cgc_stack
 */
bool cgc_stack_is_empty (const cgc_stack * stack);

/**
 * \brief Get the size of the stack.
 * \param stack Stack.
 * \return size.
 * \relatesalso cgc_stack
 */
size_t cgc_stack_size (const cgc_stack * stack);

////////////////////////////////////////////////////////////////////////////////
// Access.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Get the top of the stack.
 * \param stack Stack.
 * \return Front.
 * \relatesalso cgc_stack
 * \pre cgc_stack_is_empty(stack) == \c false
 * \pre stack != NULL
 * \warning This function does not check whether the supplied stack is valid!
 * \warning This function does not check whether the supplied stack is long enough!
 */
void * cgc_stack_top (const cgc_stack * stack);

////////////////////////////////////////////////////////////////////////////////
// Modifiers.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Push to the top.
 * \param[in,out] stack Stack
 * \param[in] element Element.
 * \retval 0 in case of success
 * \retval -1 if one of the arguments is \c NULL. \c errno shall be set to
 * \c EINVAL.
 * \retval -2 if the the function failed because memory allocation failed. \c
 * errno may be set to \c ENOMEM.
 * \relatesalso cgc_stack
 * \return This function shall return 0 in case of success, a negative integer
 * in case of failure.
 * \note The supplied element will be copied into the stack. It is safe to free
 * \c element afterwards.
 */
int cgc_stack_push (cgc_stack * stack, const void * element);

/**
 * \brief Pop the top.
 * \param[in,out] stack Stack.
 * \return First element.
 * \relatesalso cgc_stack
 * \warning It is up to the user to free the returned element once unneeded.
 * \pre stack != NULL
 * \pre cgc_stack_is_empty(stack) == \c false
 */
void * cgc_stack_pop (cgc_stack * stack);

/**
 * \brief Clear a stack.
 * \param[in,out] stack Stack.
 * \relatesalso cgc_stack
 * \return This function shall return 0 in case of success, a negative integer
 * in case of failure.
 * \retval 0 in case of success.
 * \retval -1 if one of the arguments is \c NULL. \c errno shall be set to
 * \c EINVAL.
 */
int cgc_stack_clear (cgc_stack * stack);

#endif /* _CGC_STACK_H_ */
