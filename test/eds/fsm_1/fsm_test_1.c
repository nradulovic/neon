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

#include "shared/debug.h"
#include "eds/event.h"
#include "eds/smp.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/

static naction state_init(struct nsm * sm, const nevent * event);
static naction state_actv(struct nsm * sm, const nevent * event);
static naction state_inct(struct nsm * sm, const nevent * event);

/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static naction state_init(struct nsm * sm, const nevent * event)
{
    switch (event->id) {
        case NSMP_ENTRY: {

            return (NACTION_HANDLED);
        }
        case NSMP_EXIT: {

            return (NACTION_HANDLED);
        }
        case NSMP_INIT: {

            return (naction_transit_to(sm, &state_actv));
        }
        default : {
            return (NACTION_IGNORED);
        }
    }
}

static naction state_actv(struct nsm * sm, const nevent * event)
{
    switch (event->id) {
        case NSMP_ENTRY: {

            return (NACTION_HANDLED);
        }
        case NSMP_EXIT: {

            return (NACTION_HANDLED);
        }
        case NSMP_INIT: {

            return (naction_transit_to(sm, &state_inct));
        }
        default : {
            return (NACTION_IGNORED);
        }
    }
}

static naction state_inct(struct nsm * sm, const nevent * event)
{
    switch (event->id) {
        case NSMP_ENTRY: {

            return (NACTION_HANDLED);
        }
        case NSMP_EXIT: {

            return (NACTION_HANDLED);
        }
        case NSMP_INIT: {

            return (naction_transit_to(sm, &state_actv));
        }
        default : {
            return (NACTION_IGNORED);
        }
    }
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/


int main(void)
{
    struct nsm                  test_fsm;
    const struct nsm_define     test_fsm_define =
    {
        .wspace     = NULL,
        .type       = NSM_TYPE_FSM,
        .init_state = &state_init
    };
    nsm_init(&test_fsm, &test_fsm_define);
    nsm_dispatch(&test_fsm, nsmp_event(NSMP_INIT));

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



void assert_failed(uint8_t* file, uint32_t line)
{
    (void)file;
    (void)line;
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//***************************************************************
 * END of main.c
 ******************************************************************************/
