/**
 * \file common.h
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
#ifndef _CGC_COMMON_H_
#define _CGC_COMMON_H_

#include <stdlib.h>
#include <stdio.h>
#include <sysexits.h>
#include <stdbool.h>
#include <errno.h>

/**
 * \brief Check pointers.
 * \param pointer Pointer to check.
 * \retval 0 if the pointer does not point to \c NULL.
 * \retval -1 otherwise.
 * \note \c errno will be set to \c EINVAL if \c pointer points to \c NULL.
 */
static inline int cgc_check_pointer (const void * const pointer)
{
    int error = 0;
    if (pointer == NULL)
    {
        error = -1;
        errno = EINVAL;
    }
    return error;
}

#endif /* _CGC_COMMON_H_ */
