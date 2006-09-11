#ifndef INCLUDE_GENERATOR_H
#define INCLUDE_GENERATOR_H

//#include "../driver/output.h"
#include "lexpolicies.h"
#include "xformctx.h"

class SourceGenerator {
    context_t& c;

public:
    SourceGenerator (context_t& ctx) : c(ctx){}

    // returns a list of the modules defined (in part or whole) in
    // this file.
    std::vector<std::string>  execute (OutputDelegate *header, 
                                       OutputDelegate *source);
};

#endif
