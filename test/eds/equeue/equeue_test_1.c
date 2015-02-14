/*
 * This file is part of Neon.
 *
 * Copyright (C) 2010 - 2015 Nenad Radulovic
 *
 * Neon is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Neon is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Neon.  If not, see <http://www.gnu.org/licenses/>.
 *
 * web site:    http://github.com/nradulovic
 * e-mail  :    nenad.b.radulovic@gmail.com
 */

/*=========================================================  INCLUDE FILES  ==*/

#include <stdbool.h>

#include "shared/debug.h"
#include "eds/equeue.h"
#include "eds/event.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

#define nequeu_free(equeue)				nqueue_free(&(equeue)->queue)

int main(void)
{
	volatile size_t				o_free;
	volatile const struct nevent *    o_event;
	volatile bool				o_is_empty;
	volatile bool 				o_is_full;

	struct nevent *				equeue_storage[4];
	struct nequeue				equeue;
	const struct nevent			event[4] =
	{
		{
			.id = 1
		}, {
			.id = 2
		}, {
			.id = 3
		}, {
			.id = 4
		}
	};
	const struct nequeue_define equeue_define =
	{
		equeue_storage,
		sizeof(equeue_storage)
	};
	nequeue_init(&equeue, &equeue_define);

	/*
	 * 1: 	put fifo(1),
	 * 		o_free = 3,
	 * 		o_is_empty = false,
	 * 		o_is_full = false,
	 * 		get() = 1,
	 */
	nequeue_put_fifo(&equeue, &event[0]);
	o_free  	= nequeu_free(&equeue);
	o_is_empty	= nequeue_is_empty(&equeue);
	o_is_full   = nequeue_is_full(&equeue);
	o_event 	= nequeue_get(&equeue);

	/*
	 * 2: 	put lifo(2),
	 * 		o_free = 3,
	 * 		o_is_empty = false,
	 * 		o_is_full = false,
	 * 		get() = 2
	 */
	nequeue_put_lifo(&equeue, &event[1]);
	o_free  	= nequeu_free(&equeue);
	o_is_empty	= nequeue_is_empty(&equeue);
	o_is_full   = nequeue_is_full(&equeue);
	o_event 	= nequeue_get(&equeue);

	/*
	 * 3: 	o_free = 4,
	 * 		o_is_empty = true,
	 * 		o_is_full = false,
	 */
	o_free  	= nequeu_free(&equeue);
	o_is_empty	= nequeue_is_empty(&equeue);
	o_is_full   = nequeue_is_full(&equeue);

	/*
	 * 4:	put_fifo(1), put_fifo(2), put_fifo(3), put_fifo(4),
	 * 		o_free = 0,
	 * 		o_is_empty = false,
	 * 		o_is_full = true,
	 */
	nequeue_put_fifo(&equeue, &event[0]);
	nequeue_put_fifo(&equeue, &event[1]);
	nequeue_put_fifo(&equeue, &event[2]);
	nequeue_put_fifo(&equeue, &event[3]);
	o_free  	= nequeu_free(&equeue);
	o_is_empty	= nequeue_is_empty(&equeue);
	o_is_full   = nequeue_is_full(&equeue);

	/*
	 * 5:   get() = 1, get() = 2, get() = 3, get() = 4
	 * 		o_free = 4,
	 * 		o_is_empty = true,
	 * 		o_is_full = false,
	 */
	o_event 	= nequeue_get(&equeue);
	o_event 	= nequeue_get(&equeue);
	o_event 	= nequeue_get(&equeue);
	o_event 	= nequeue_get(&equeue);
	o_free  	= nequeu_free(&equeue);
	o_is_empty	= nequeue_is_empty(&equeue);
	o_is_full   = nequeue_is_full(&equeue);

	/*
	 * 6:   put_fifo(1), put_lifo(4),
	 * 		get() = 4, get() = 1,
	 * 		o_free = 4,
	 * 		o_is_empty = true,
	 * 		o_is_full = false,
	 */
	nequeue_put_fifo(&equeue, &event[0]);
	nequeue_put_lifo(&equeue, &event[3]);
	o_event 	= nequeue_get(&equeue);
	o_event 	= nequeue_get(&equeue);
	o_free  	= nequeu_free(&equeue);
	o_is_empty	= nequeue_is_empty(&equeue);
	o_is_full   = nequeue_is_full(&equeue);

	/*
	 * 7:   put_fifo(1), put_fifo(2), put_fifo(3), put_lifo(4),
	 * 		get() = 4, get() = 1, get() = 2, get() = 3,
	 * 		o_free = 4,
	 * 		o_is_empty = true,
	 * 		o_is_full = false,
	 */
	nequeue_put_fifo(&equeue, &event[0]);
	nequeue_put_fifo(&equeue, &event[1]);
	nequeue_put_fifo(&equeue, &event[2]);
	nequeue_put_lifo(&equeue, &event[3]);
	o_event 	= nequeue_get(&equeue);
	o_event 	= nequeue_get(&equeue);
	o_event 	= nequeue_get(&equeue);
	o_event 	= nequeue_get(&equeue);
	o_free  	= nequeu_free(&equeue);
	o_is_empty	= nequeue_is_empty(&equeue);
	o_is_full   = nequeue_is_full(&equeue);

	/*
	 * 8:   put_lifo(1), put_lifo(2), put_lifo(3), put_lifo(4),
	 * 		get() = 4, get() = 3, get() = 2, get() = 1,
	 * 		o_free = 4,
	 * 		o_is_empty = true,
	 * 		o_is_full = false,
	 */
	nequeue_put_lifo(&equeue, &event[0]);
	nequeue_put_lifo(&equeue, &event[1]);
	nequeue_put_lifo(&equeue, &event[2]);
	nequeue_put_lifo(&equeue, &event[3]);
	o_event 	= nequeue_get(&equeue);
	o_event 	= nequeue_get(&equeue);
	o_event 	= nequeue_get(&equeue);
	o_event 	= nequeue_get(&equeue);
	o_free  	= nequeu_free(&equeue);
	o_is_empty	= nequeue_is_empty(&equeue);
	o_is_full   = nequeue_is_full(&equeue);

	(void)o_free;
	(void)o_event;
	(void)o_is_empty;
	(void)o_is_full;

	return (0);
}

PORT_C_NORETURN void hook_at_assert(
    const PORT_C_ROM struct ncomponent_info * component_info,
    const PORT_C_ROM char *     fn,
    uint32_t                    line,
    const PORT_C_ROM char *     expr,
    const PORT_C_ROM char *     msg)
{
	for (;;);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//***************************************************************
 * END of main.c
 ******************************************************************************/
