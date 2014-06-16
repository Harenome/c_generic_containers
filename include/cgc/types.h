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

typedef void * (* cgc_alloc_function) (void);
typedef void (* cgc_free_function) (void *);
typedef int (* cgc_copy_function) (const void *, void *);
typedef int (* cgc_unary_op_function) (void *);
typedef int (* cgc_binary_op_function) (const void *, const void *, void *);

#endif /* _CGC_TYPES_H_ */
