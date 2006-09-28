// method_body.cpp
#include "method_body.h"
#include "base_operations.h"
#include "lexpolicies.h"
#include <boost/wave/token_ids.hpp>

#include <iostream>
using namespace boost::wave;
using namespace std;

MethodBodyXForm::
MethodBodyXForm (const context_iter_t& s,
                 const context_iter_t& e) 
 : TransformStage(s,e),m_depth(0), m_started(false) {
}

OperationPair
MethodBodyXForm::
process_token (const token_t& tok, TransformContext *) {
    OperationPair result;
    if (!m_started) {
        result.source = Operation_p(new TokenOp(tok));
        if (tok == T_LEFTBRACE) {
            m_started = true;
            m_depth++;
            result.header = Operation_p(new StringOp(";"));
        } else {
            result.header = result.source;
        }
    } else {
        if (tok == T_LEFTBRACE) {
            m_depth++;
        } else if (tok == T_RIGHTBRACE) {
            m_depth--;
        }
        
        result.header = Operation::null_op;
        result.source = Operation_p(new TokenOp(tok));
        
        if (m_depth == 0) {
            m_started = false;
        }
    }
    return result;
}

OperationPair
MethodBodyXForm::
process_upstream (OperationPair p, TransformContext *) {
    return p;
}

MethodBodyXForm::
~MethodBodyXForm () {}

const std::string& 
MethodBodyXForm::
get_identifier (){
	static std::string s_id("method_body");
	return s_id;
}

const std::string& 
MethodBodyXForm::
identifier () const {
	return get_identifier ();
}