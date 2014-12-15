/*
 * fsm_test_1.c
 *
 *  Created on: 14 Dec 2014
 *      Author: nenad
 */

#include <stdbool.h>
#include <stdio.h>

#include "shared/debug.h"
#include "eds/event.h"
#include "eds/smp.h"

static naction state_init(struct nsm * sm, const nevent * event);
static naction state_s(struct nsm * sm, const nevent * event);
static naction state_s1(struct nsm * sm, const nevent * event);
static naction state_s2(struct nsm * sm, const nevent * event);
static naction state_s11(struct nsm * sm, const nevent * event);
static naction state_s21(struct nsm * sm, const nevent * event);
static naction state_s211(struct nsm * sm, const nevent * event);

struct test_fsm_wspace {
	bool			is_foo;
};

static struct nsm test_fsm;
static struct test_fsm_wspace test_fsm_wspace;


static const struct nsm_define test_fsm_define = {
	.wspace 	= &test_fsm_wspace,
	.type   	= NTYPE_HSM,
	.init_state = &state_init
};

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

int main(void)
{
	struct nevent 				event;
	int							c;

	nsm_init(&test_fsm, &test_fsm_define);
	nsm_dispatch(&test_fsm, nsmp_event(NSMP_INIT));

	event.attrib = 0;
	event.ref    = 0;
	event.mem    = NULL;
	event.signature = NEVENT_SIGNATURE;
	printf("\n\n- press 's' to quit:\n");

	while ((c = getchar()) != 'q') {
		event.id = (uint_fast16_t)c;

		printf("->signal: %c\n", c);

		nsm_dispatch(&test_fsm, &event);
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
