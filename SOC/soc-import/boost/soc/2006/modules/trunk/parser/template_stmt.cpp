#include "template_stmt.h"
#include "base_operations.h"
#include "lexpolicies.h"
#include "xformctx.h"
#include <boost/wave/token_ids.hpp>
#include <boost/format.hpp>

#include <iostream>

using namespace std;
using namespace boost::wave;
using boost::format;

TemplateStmtXForm::
TemplateStmtXForm (const context_iter_t& s,
                   const context_iter_t& e) 
  : TransformStage (s,e) {}


//
// Templates should be passed verbatim to both the 
// header and source files.
OperationPair 
TemplateStmtXForm::
process_token (const token_t& tok, TransformContext *) {
    OperationPair result;
    result.header = Operation_p(new TokenOp(tok));
    result.source = result.header;
    
    return result;
}

OperationPair 
TemplateStmtXForm::
process_upstream (OperationPair p, TransformContext *) {
    p.header = p.source;
    return p;
}

TemplateStmtXForm::
~TemplateStmtXForm () {}

const std::string& 
TemplateStmtXForm::
get_identifier (){
    static std::string s_id("template_stmt");
    return s_id;
}

const std::string& 
TemplateStmtXForm::
identifier () const {
    return get_identifier ();
}
