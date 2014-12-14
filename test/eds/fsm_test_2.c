/*
 * fsm_test_1.c
 *
 *  Created on: 14 Dec 2014
 *      Author: nenad
 */

#include "shared/debug.h"
#include "eds/event.h"
#include "eds/smp.h"

static naction state_init(struct nsm * sm, const nevent * event);
static naction state_actv(struct nsm * sm, const nevent * event);
static naction state_inct(struct nsm * sm, const nevent * event);
static naction state_b1(struct nsm * sm, const nevent * event);
static naction state_b2(struct nsm * sm, const nevent * event);

static const struct nsm_define test_fsm_define = {
	.wspace 	= NULL,
	.type   	= NTYPE_FSM,
	.init_state = &state_init
};

const nevent events[] = {
	{
		.id = 'i'
	}, {
		.id = 'a'
	}, {
		.id = 'b'
	}, {
		.id = 'i'
	}
};

static struct nsm test_fsm;

static naction state_init(struct nsm * sm, const nevent * event)
{
	switch (event->id) {
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
		case 'i': {

			return (naction_transit_to(sm, &state_inct));
		}
		case 'b': {
			return (naction_transit_to(sm, &state_b1));
		}
		default : {
			return (NACTION_IGNORED);
		}
	}
}

static naction state_inct(struct nsm * sm, const nevent * event)
{
	switch (event->id) {
		case 'a': {

			return (naction_transit_to(sm, &state_actv));
		}
		default : {
			return (NACTION_IGNORED);
		}
	}
}

static naction state_b1(struct nsm * sm, const nevent * event)
{
	switch (event->id) {
		case NSMP_INIT: {

			return (naction_transit_to(sm, &state_b2));
		}
		default : {
			return (NACTION_IGNORED);
		}
	}
}

static naction state_b2(struct nsm * sm, const nevent * event)
{
	switch (event->id) {
		case NSMP_INIT: {

			return (naction_transit_to(sm, &state_inct));
		}
		default : {
			return (NACTION_IGNORED);
		}
	}
}

int main(void)
{
	int							counter = 0;
	nsm_init(&test_fsm, &test_fsm_define);
	nsm_dispatch(&test_fsm, nsmp_event(NSMP_INIT));

	for (;;) {
		nsm_dispatch(&test_fsm, &events[counter ++ & 0x03u]);
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
	for (;;);
}
