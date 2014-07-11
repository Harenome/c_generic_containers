/**
 * \file types.h
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
#ifndef _CGC_TYPES_H_
#define _CGC_TYPES_H_

/**
 * \brief Clean functions.
 *
 * # Synopsis
 * Among the necessary tools for CGC containers is the clean function.
 * Before erasing an element, a CGC container must clean it.
 * Since CGC containers aim to be generic, they have no means to determine on
 * their own whether cleaning is required, and how to perform this action when
 * needed.
 *
 * For that matter, the user must, if needed, provide a cleaning function at
 * the creation of the container. If none is provided, the container will assume
 * no special cleaning is required before deleting an element.
 *
 * # Prototype and behaviour
 * A cleaning function must have the following prototype:
 *
 *     void cleaning_function (void *);
 *
 * This function shall clean the underlying contents of an element. It should
 * not, however, free the memory of the element: the container will.
 *
 * # Simple example
 * Imagine a CGC container should store the following struct:
 *
 *     // An example.
 *     struct example_struct
 *     {
 *         int _static;         // An integer.
 *         int * _dynamic;      // A pointer to an integer.
 *     }
 *
 * Let us also suppose that the \c _dynamic field will always point to malloc'd
 * integers. A clean function for this struct could be:
 *
 *     // A clean function for struct_example.
 *     void example_struct_clean (void * v)
 *     {
 *         struct example_struct * ex = v;
 *         free (ex->_dynamic);
 *     }
 */
typedef void (* cgc_clean_function) (void *);

/**
 * \brief Copy functions.
 *
 * # Synopsis
 * Among the necessary tools for CGC containers is the copy function. In order
 * to store elements, a CGC container must copy them. However, complex types
 * can not merely be copied. If an element holds pointers, the contents of the
 * pointed memory may (or may not!) need to be copied as well.
 *
 * For that matter, the user must, if needed, provide a copy function at
 * the creation of the container. If none is provided, the container will assume
 * no special action is required before copying an element.
 *
 * # Prototype and behaviour
 * A cleaning function must have the following prototype:
 *
 *     int copy_function (const void *, void *);
 *
 * The first argument corresponds to the original elemnt, while the second
 * argument is the destination of the copy.
 *
 * This function shall assume the memory for the copy has already been allocated.
 *
 * # Simple example
 * Imagine a CGC container should store the following struct:
 *
 *     // An example.
 *     struct example_struct
 *     {
 *         int _static;         // An integer.
 *         int * _dynamic;      // A pointer to an integer.
 *     }
 *
 * A copy function for this struct could be:
 *
 *     // A copy function for struct_example.
 *     int example_struct_copy (const void * o, void * c)
 *     {
 *         int error = 0;
 *
 *         // Cast the pointers.
 *         const struct example_struct * const original = o;
 *         struct example_struct * const copy = c;
 *
 *         // Initial copy of the contents of the struct.
 *         * copy = * original;
 *
 *         // Take care of the pointer inside the struct.
 *         copy->_dynamic = malloc (sizeof * copy->_dynamic);
 *         if (copy->_dynamic != NULL)
 *             copy->_dynamic = original->_dynamic;
 *         else
 *             error = -1;
 *
 *         return error;
 *     }
 *
 * Note that since this copy function will malloc new memory for the \c _dynamic
 * field, a clean function that would free this memory should be provided along
 * this copy function.
 */
typedef int (* cgc_copy_function) (const void *, void *);

/**
 * \brief Unary operation functions.
 */
typedef int (* cgc_unary_op_function) (void *);

/**
 * \brief Binary operation functions.
 */
typedef int (* cgc_binary_op_right_function) (const void *, void *);

/**
 * \brief Binary operation functions.
 */
typedef int (* cgc_binary_op_left_function) (void *, const void *);


#endif /* _CGC_TYPES_H_ */
