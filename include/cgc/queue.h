/**
 * \file queue.h
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
#ifndef _CGC_QUEUE_H_
#define _CGC_QUEUE_H_

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
 * \defgroup queues_group Queues
 */

////////////////////////////////////////////////////////////////////////////////
// Typedefs.
////////////////////////////////////////////////////////////////////////////////

/**
 * \class cgc_queue
 * \ingroup queues_group
 * \brief CGC Queue.
 *
 * CGC Queues are akin to CGC Lists, but allow less operations.
 *
 * CGC Queue function        | CGC List equivalent
 * --------------------------|-----------------------
 * cgc_queue_create()        | cgc_list_create()
 * cgc_queue_destroy()       | cgc_list_destroy()
 * cgc_queue_init()          | cgc_list_init()
 * cgc_queue_clean()         | cgc_list_clean()
 * cgc_queue_copy()          | cgc_list_copy()
 * cgc_queue_copy_into()     | cgc_list_copy_into()
 * cgc_queue_swap()          | cgc_list_swap()
 * cgc_queue_front()         | cgc_list_front()
 * cgc_queue_back()          | cgc_list_back()
 * cgc_queue_is_empty()      | cgc_list_is_empty()
 * cgc_queue_size()          | cgc_list_size()
 * cgc_queue_push()          | cgc_list_push_back()
 * cgc_queue_pop()           | cgc_list_pop_front()
 * cgc_queue_clear()         | cgc_list_clear()
 *
 * \sa cgc_list
 */

/**
 * \brief cgc_queue
 */
typedef cgc_list cgc_queue;

////////////////////////////////////////////////////////////////////////////////
// Dynamic creation and destruction.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Create a new cgc_queue.
 * \param element_size Element size.
 * \param copy_fun Copy function.
 * \param clean_fun Cleaning function.
 * \relatesalso cgc_queue
 * \return The pointer to the new cgc_queue in case of success. \c NULL in case
 * of failure.
 * \retval NULL if the queue could not be allocated.
 * \note Queues obtained this way must be freed using cgc_queue_destroy().
 * \note In case of failure, \c errno may be set to \c ENOMEM.
 * \note A call to this function may change the value of \c errno.
 */
cgc_queue * cgc_queue_create (size_t element_size, cgc_copy_function copy_fun, cgc_clean_function clean_fun);

/**
 * \brief Free a dynamically allocated cgc_queue.
 * \param queue Queue.
 * \relatesalso cgc_queue
 * \note A call to this function may change the value of \c errno.
 * \note It is safe to pass a \c NULL to this function.
 * \warning It is strongly advised to use this function only of queues obtained
 * via cgc_queue_create().
 */
void cgc_queue_destroy (cgc_queue * queue);

////////////////////////////////////////////////////////////////////////////////
// Initialization and cleaning.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Initialize a cgc_queue.
 * \param[in,out] queue Queue.
 * \param[in] element_size Element size
 * \param[in] copy_fun Copy function.
 * \param[in] clean_fun Cleaning function.
 * \relatesalso cgc_queue
 * \return This function shall return 0 in case of success, a negative integer
 * in case of failure.
 * \retval 0 in case of success.
 * \retval -1 if one of the arguments is \c NULL. \c errno shall be set to
 * \c EINVAL.
 */
int cgc_queue_init (cgc_queue * queue, size_t element_size, cgc_copy_function copy_fun, cgc_clean_function clean_fun);

/**
 * \brief Clean a cgc_queue.
 * \param[in,out] queue Queue.
 * \return This function shall return 0 in case of success, a negative integer
 * in case of failure.
 * \retval 0 in case of success.
 * \retval -1 if one of the arguments is \c NULL. \c errno shall be set to
 * \c EINVAL.
 * \relatesalso cgc_queue
 */
int cgc_queue_clean (cgc_queue * queue);

////////////////////////////////////////////////////////////////////////////////
// Copy.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Copy a queue.
 * \param queue Queue.
 * \return A pointer to the copy in case of success. \c NULL in case of failure.
 * \relatesalso cgc_queue
 * \note It is safe to pass a \c NULL to this function.
 * \retval NULL if the queue could not be copied or \c queue was \c NULL.
 * \note Queues obtained this way must be freed using cgc_queue_destroy().
 * \note A call to this function may change the value of \c errno.
 */
cgc_queue * cgc_queue_copy (const cgc_queue * queue);

/**
 * \brief Copy a cgc_queue into \c destination.
 * \param[in] original The original.
 * \param[in,out] destination The destination of the copy.
 * \retval 0 in case of success.
 * \retval -1 if one of the arguments is \c NULL. \c errno may be set to \c EINVAL.
 * \relatesalso cgc_queue
 * \warning This function will overwrite the destination! It is up to the user to
 * first clean \c destination if needed.
 */
int cgc_queue_copy_into (const cgc_queue * original, cgc_queue * destination);

////////////////////////////////////////////////////////////////////////////////
// Swap.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Swap the contents of two queues.
 * \param[in,out] a First queue.
 * \param[in,out] b Second queue.
 * \relatesalso cgc_queue
 * \pre a != NULL && b != NULL
 * \return 0 in case of success.
 * \return -1 if at least of the argumes is \c NULL. \c errno may be set to
 * \c EINVAL.
 * \note A cal to this function may change the value of \c errno.
 */
int cgc_queue_swap (cgc_queue * a, cgc_queue * b);

////////////////////////////////////////////////////////////////////////////////
// Properties getters.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Check whether a CGC queue is empty.
 * \param queue Queue.
 * \retval true if the queue is empty.
 * \retval false otherwise.
 * \relatesalso cgc_queue
 */
bool cgc_queue_is_empty (const cgc_queue * queue);

/**
 * \brief Get the size of the queue.
 * \param queue Queue.
 * \return size.
 * \relatesalso cgc_queue
 */
size_t cgc_queue_size (const cgc_queue * queue);

////////////////////////////////////////////////////////////////////////////////
// Access.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Get the front of the queue.
 * \param queue Queue.
 * \return Front.
 * \relatesalso cgc_queue
 * \pre cgc_queue_is_empty(queue) == \c false
 * \pre queue != NULL
 * \warning This function does not check whether the supplied queue is valid!
 * \warning This function does not check whether the supplied queue is long enough!
 */
void * cgc_queue_front (const cgc_queue * queue);

/**
 * \brief Get the back of the queue.
 * \param queue Queue.
 * \return Back.
 * \relatesalso cgc_queue
 * \pre cgc_queue_is_empty(queue) == \c false
 * \pre queue != NULL
 * \warning This function does not check whether the supplied queue is valid!
 * \warning This function does not check whether the supplied queue is long enough!
 */
void * cgc_queue_back (const cgc_queue * queue);

////////////////////////////////////////////////////////////////////////////////
// Modifiers.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Push to the back.
 * \param[in,out] queue Queue
 * \param[in] element Element.
 * \retval 0 in case of success
 * \retval -1 if one of the arguments is \c NULL. \c errno shall be set to
 * \c EINVAL.
 * \retval -2 if the the function failed because memory allocation failed. \c
 * errno may be set to \c ENOMEM.
 * \relatesalso cgc_queue
 * \return This function shall return 0 in case of success, a negative integer
 * in case of failure.
 * \note The supplied element will be copied into the queue. It is safe to free
 * \c element afterwards.
 */
int cgc_queue_push (cgc_queue * queue, const void * element);

/**
 * \brief Pop the front.
 * \param[in,out] queue Queue.
 * \return First element.
 * \relatesalso cgc_queue
 * \warning It is up to the user to free the returned element once unneeded.
 * \pre queue != NULL
 * \pre cgc_queue_is_empty(queue) == \c false
 */
void * cgc_queue_pop (cgc_queue * queue);

/**
 * \brief Clear a queue.
 * \param[in,out] queue Queue.
 * \relatesalso cgc_queue
 * \return This function shall return 0 in case of success, a negative integer
 * in case of failure.
 * \retval 0 in case of success.
 * \retval -1 if one of the arguments is \c NULL. \c errno shall be set to
 * \c EINVAL.
 */
int cgc_queue_clear (cgc_queue * queue);

#endif /* _CGC_QUEUE_H_ */
