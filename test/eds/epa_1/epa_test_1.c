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
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#include "mem/heap.h"
#include "eds/event.h"
#include "eds/epa.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/

struct test_fsm_wspace
{
    bool            is_foo;
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static naction state_init(struct nsm * sm, const nevent * event);
static naction state_s(struct nsm * sm, const nevent * event);
static naction state_s1(struct nsm * sm, const nevent * event);
static naction state_s2(struct nsm * sm, const nevent * event);
static naction state_s11(struct nsm * sm, const nevent * event);
static naction state_s21(struct nsm * sm, const nevent * event);
static naction state_s211(struct nsm * sm, const nevent * event);
static void * runner(void * arg);

/*=======================================================  LOCAL VARIABLES  ==*/

static struct test_fsm_wspace   g_test_fsm_wspace;
static struct nevent *          g_working_fifo_storage[10];
static struct nepa              g_test_epa;
static struct nheap             g_event_mem;
static uint8_t                  g_event_mem_storage[1024];

static const struct nepa_define g_test_epa_define =
{
    .sm.wspace              = &g_test_fsm_wspace,
    .sm.init_state          = state_init,
    .sm.type                = NSM_TYPE_HSM,
    .working_fifo.storage   = g_working_fifo_storage,
    .working_fifo.size      = sizeof(g_working_fifo_storage),
    .thread.name            = "test EPA",
    .thread.priority        = 10
};

/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static naction state_init(struct nsm * sm, const nevent * event)
{
    struct test_fsm_wspace * ws = sm->wspace;

    switch (event->id) {
        case NSMP_INIT: {
            printf("init:init/is_foo=false->s2 ");
            ws->is_foo = false;

            return (naction_transit_to(sm, &state_s2));
        }
        default : {
            return (naction_super(sm, &ntop_state));
        }
    }
}

static naction state_s(struct nsm * sm, const nevent * event)
{
    struct test_fsm_wspace * ws = sm->wspace;

    switch (event->id) {
        case NSMP_ENTRY: {
            printf("s:entry ");

            return (NACTION_HANDLED);
        }
        case NSMP_EXIT: {
            printf("s:exit ");

            return (NACTION_HANDLED);
        }
        case NSMP_INIT: {
            printf("s:init->s11 ");

            return (naction_transit_to(sm, &state_s11));
        }
        case 'e': {
            printf("s:e->s11 ");

            return (naction_transit_to(sm, &state_s11));
        }
        case 'i': {
            if (ws->is_foo) {
                ws->is_foo = false;
                printf("s:i[is_foo] ");
            } else {
                printf("s:i[!is_foo] ");
            }

            return (NACTION_HANDLED);
        }
        default : {
            return (naction_super(sm, &ntop_state));
        }
    }
}

static naction state_s1(struct nsm * sm, const nevent * event)
{
    struct test_fsm_wspace * ws = sm->wspace;

    switch (event->id) {
        case NSMP_ENTRY: {
            printf("s1:entry ");

            return (NACTION_HANDLED);
        }
        case NSMP_EXIT: {
            printf("s1:exit ");

            return (NACTION_HANDLED);
        }
        case NSMP_INIT: {
            printf("s1:init->s11 ");

            return (naction_transit_to(sm, &state_s11));
        }
        case 'a': {
            printf("s1:a->s1 ");
            return (naction_transit_to(sm, &state_s1));
        }
        case 'b': {
            printf("s1:b->s11 ");
            return (naction_transit_to(sm, &state_s11));
        }
        case 'c': {
            printf("s1:c->s2 ");
            return (naction_transit_to(sm, &state_s2));
        }
        case 'd': {
            if (!ws->is_foo) {
                printf("s1:d[!is_foo]/is_foo=true->s ");
                ws->is_foo = true;
                return (naction_transit_to(sm, &state_s));
            } else {
                printf("s1:d[is_foo] ");
                return (NACTION_HANDLED);
            }
        }
        case 'f': {
            printf("s1:f->s211 ");
            return (naction_transit_to(sm, &state_s211));
        }
        case 'i': {
            printf("s1:i ");
            return (NACTION_HANDLED);
        }
        default : {
            return (naction_super(sm, &state_s));
        }
    }
}

static naction state_s11(struct nsm * sm, const nevent * event)
{
    struct test_fsm_wspace * ws = sm->wspace;

    switch (event->id) {
        case NSMP_ENTRY: {
            printf("s11:entry ");
            return (NACTION_HANDLED);
        }
        case NSMP_EXIT: {
            printf("s11:exit ");
            return (NACTION_HANDLED);
        }
        case 'd': {
            if (ws->is_foo) {
                ws->is_foo = false;
                printf("s11:d[is_foo]/is_foo=false->s1 ");
                return (naction_transit_to(sm, &state_s1));
            } else {
                printf("s11:d[!is_foo] ");
                return (NACTION_HANDLED);
            }
        }
        case 'g': {
            printf("s11:g->s211 ");
            return (naction_transit_to(sm, &state_s211));
        }
        case 'h': {
            printf("s11:h->s ");
            return (naction_transit_to(sm, &state_s));
        }
        default : {
            return (naction_super(sm, &state_s1));
        }
    }
}

static naction state_s2(struct nsm * sm, const nevent * event)
{
    struct test_fsm_wspace * ws = sm->wspace;

    switch (event->id) {
        case NSMP_ENTRY: {
            printf("s2:entry ");
            return (NACTION_HANDLED);
        }
        case NSMP_EXIT: {
            printf("s2:exit ");
            return (NACTION_HANDLED);
        }
        case NSMP_INIT: {
            printf("s2:init->s211 ");
            return (naction_transit_to(sm, &state_s211));
        }
        case 'c': {
            printf("s2:c->s1 ");
            return (naction_transit_to(sm, &state_s1));
        }
        case 'f': {
            printf("s2:f->s11 ");
            return (naction_transit_to(sm, &state_s11));
        }
        case 'i': {
            if (!ws->is_foo) {
                ws->is_foo = true;
                printf("s2:i[is_foo]/is_foo=true ");
            } else {
                printf("s2:i[!is_foo] ");
            }
            return (NACTION_HANDLED);
        }
        default : {
            return (naction_super(sm, &state_s));
        }
    }
}

static naction state_s21(struct nsm * sm, const nevent * event)
{
    switch (event->id) {
        case NSMP_ENTRY: {
            printf("s21:entry ");
            return (NACTION_HANDLED);
        }
        case NSMP_EXIT: {
            printf("s21:exit ");
            return (NACTION_HANDLED);
        }
        case NSMP_INIT: {
            printf("s21:init->s211 ");
            return (naction_transit_to(sm, &state_s211));
        }
        case 'a': {
            printf("s21:a->s21 ");
            return (naction_transit_to(sm, &state_s21));
        }
        case 'b': {
            printf("s21:b->s211 ");
            return (naction_transit_to(sm, &state_s211));
        }
        case 'g': {
            printf("s21:g->s11 ");
            return (naction_transit_to(sm, &state_s11));
        }
        default : {
            return (naction_super(sm, &state_s2));
        }
    }
}

static naction state_s211(struct nsm * sm, const nevent * event)
{
    switch (event->id) {
        case NSMP_ENTRY: {
            printf("s211:entry ");
            return (NACTION_HANDLED);
        }
        case NSMP_EXIT: {
            printf("s211:exit ");
            return (NACTION_HANDLED);
        }
        case 'd': {
            printf("s211:d->s21 ");
            return (naction_transit_to(sm, &state_s21));
        }
        case 'h': {
            printf("s211:h->s ");
            return (naction_transit_to(sm, &state_s));
        }
        default : {
            return (naction_super(sm, &state_s21));
        }
    }
}

static void * runner(void * arg)
{
    (void)arg;

    neds_run();

    return (NULL);
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

int main(void)
{
    static pthread_t            g_runner;
    int                         c;
    int                         err;


    nheap_init(&g_event_mem, g_event_mem_storage, sizeof(g_event_mem_storage));
    nevent_register_mem(&g_event_mem.mem_class);

    nsched_init();
    nepa_init(&g_test_epa, &g_test_epa_define);


    printf("\n\n- press 's' to quit:\n");
    err = pthread_create(&g_runner, NULL, &runner, NULL);

    if (err != 0) {
        return (1);
    }

    while ((c = getchar()) != 'q') {
        struct nevent *         event;

        event = nevent_create(sizeof(struct nevent), (uint16_t)c);

        printf("->signal: %c\n", c);

        nepa_send_event(&g_test_epa, event);

        printf("\n");
    }

    return (0);
}



PORT_C_NORETURN void hook_at_assert(
    const PORT_C_ROM struct ncomponent_info * component_info,
    const PORT_C_ROM char *     fn,
    uint32_t                    line,
    const PORT_C_ROM char *     expr,
    const PORT_C_ROM char *     msg)
{
    (void)component_info;
    (void)fn;
    (void)line;
    (void)expr;
    (void)msg;

    for (;;);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//***************************************************************
 * END of main.c
 ******************************************************************************/
