// import_stmt.cpp
#include "import_stmt.h"
#include "export_stmt.h"
#include "base_operations.h"
#include "lexpolicies.h"
#include "xformctx.h"
#include <boost/wave/token_ids.hpp>
#include <boost/format.hpp>

#include <iostream>

using namespace std;
using namespace boost::wave;
using boost::format;

struct IncludeOp : public Operation {
    ModuleName m_str;
    IncludeOp (const ModuleName& s) : m_str(s) {}
    virtual void operator () (OutputDelegate * op) {
        op->include(m_str.canonical());
    }
    virtual ~IncludeOp () {}
};

ImportStmtXForm::
ImportStmtXForm (const context_iter_t& s,
                 const context_iter_t& e)
  : TransformStage (s,e), m_mode(mImport) {}
 
 
 
OperationPair 
ImportStmtXForm::
process_token (const token_t& tok, TransformContext * ctx) {
    OperationPair result;
    switch (m_mode) {
        case mImport:
            if (tok == T_IMPORT)
                m_mode = mModule;
            break;
        case mNamespace:
//             if (tok == T_NAMESPACE)
//                 m_mode = mModule;
            break;
        case mModule:
            if (tok == T_SEMICOLON) {
                result.header = Operation_p(new IncludeOp(m_mod_name));
                result.source = result.header;
                // let's see if we're in an enclosing export statment.
                TransformStage_w eq = ctx->upward_stage(
                        ExportStmtXForm::get_identifier (),
                        this);
                if (TransformStage_p enclosing = eq.lock ()) {
                	ExportStmtXForm * p = dynamic_cast<ExportStmtXForm*>(enclosing.get ());
                	// let's see if the enclosing module name's in the
                	// same partition.
                	if (p->modname().same_module(m_mod_name)) {
                		// put out a using namespace XX decl for it.
                		Operation_p ns(new StringOp ((format ("\nusing namespace %s;\n")
                		   % p->modname().as_identifier ()).str ()));
                		ctx->add_header(ns);
                		ctx->add_source(ns);
                	}
                }
                m_mode = mFound;
            } else {
                if (tok == T_IDENTIFIER) 
                    m_mod_name.add_segment (tok);
                if (tok == T_STRINGLIT)
                	m_mod_name.set_partition (tok);
            }
            break;
        case mFound:
            break;
    }
    return result;
}

OperationPair 
ImportStmtXForm::
process_upstream (OperationPair p, TransformContext *) {
    return p;
}

const std::string& 
ImportStmtXForm::
get_identifier (){
	static std::string s_id("import_stmt");
	return s_id;
}

const std::string& 
ImportStmtXForm::
identifier () const {
	return get_identifier ();
}

ImportStmtXForm::
~ImportStmtXForm () {
}
