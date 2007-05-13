/*
 *  signal_junction_template.h
 *  Utility
 *
 *  Created by Stjepan Rajko on 3/9/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SIGNAL_NETWORK_TEMPLATE_BODY

junction_impl() : enabled(true) {}
void enable() {enabled = true;}
void disable() {enabled = false;}
SIGNAL_NETWORK_FUNCTION(SIGNAL_NETWORK_DEFAULT_SLOT,Signature,SIGNAL_NETWORK_TEMPLATE_ARITY)
{
	if (enabled)
		SIGNAL_NETWORK_RETURN(_return) SIGNAL_NETWORK_TEMPLATE_BASE::out(SIGNAL_NETWORK_ARG_NAMES(SIGNAL_NETWORK_TEMPLATE_ARITY));
}
private:
volatile bool enabled;

#else

#endif