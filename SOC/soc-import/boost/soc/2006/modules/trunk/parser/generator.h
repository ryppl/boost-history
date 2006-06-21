#ifndef INCLUDE_GENERATOR_H
#define INCLUDE_GENERATOR_H

#include "../driver/output.h"
#include "lexpolicies.h"

class SourceGenerator {
	context_t& c;
	OutputDelegate& d;

public:
	SourceGenerator (context_t& ctx, OutputDelegate& del) : c(ctx), d(del) {}

	// returns a list of the modules defined (in part or whole) int
	// this file.
	std::vector<std::string>  execute ();
};

#endif
