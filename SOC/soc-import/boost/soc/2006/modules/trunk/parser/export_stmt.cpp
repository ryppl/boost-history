// export_stmt.cpp
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


//  std::string   m_mod_name;
//  enum mode { mExport, mNamespace, mModule, mFound } m_mode;

ExportStmtXForm::
ExportStmtXForm( const context_iter_t& start,
                 const context_iter_t& end )
 : TransformStage(start,end), m_mode (mExport) {
//  cerr << "[export_stmt(ctor)]" << endl; 
}

void 
ExportStmtXForm::
at_start (TransformContext *ctx) {
//  cerr << "[export_stmt: at_start]";
    ctx->start_hdr_emit ();
}

void 
ExportStmtXForm::
at_end (TransformContext * ctx) {
//  cerr << "[export_stmt: at_end]";
    ctx->stop_hdr_emit ();
}

static void
_Match (const token_t& tok, token_id id) {
    if (tok != id) {
        string n (tok.get_value().begin (), tok.get_value().end ());
        cerr << "<Expected " << get_token_value(id) << ", got '" << n << ">'" << endl;
    }
}

OperationPair 
ExportStmtXForm::
process_token (const token_t& tok, TransformContext *ctx) {
//  cerr << "[tok]";
    OperationPair result;
    result.header = Operation::null_op;
    result.source = Operation::null_op;
    switch (m_mode) {
        // we eat the 'export namespace'
        case mExport: if (tok == T_EXPORT) { m_mode = mNamespace; }
            break;
        case mNamespace: if (tok== T_NAMESPACE){ m_mode = mModule; }
            break;
        // transform the module name into a
        // namespace ModuleName { (the rest is passed through, including the
        // closing brace)
        case mModule: {
            if (tok != T_LEFTBRACE) {
                // more module name to go through. just append it to our
                // module name
                if ((tok != T_SPACE) && (tok != T_NEWLINE))
                    m_mod_name.append (tok.get_value().begin (), tok.get_value().end());
                break;
            } else {
                // just hit the opening brace of the module decl.
                // we have the full module name now, so we normalize it and
                // put out a formatted namespace statement.
                
                // first, register the original name, before we mangle it
                // all up.
                ctx->register_module(m_mod_name);
                
                // our normalization's pretty dumb: just replace non-alnums
                // with underscores.
                string s;
                for (string::iterator it = m_mod_name.begin ();
                     it != m_mod_name.end ();
                     ++it) {
                    if (isalnum(*it)) 
                        s.append(1,*it);
                    else
                        s.append(1,'_');
                }
                
                // put out a readable decl.
                result.header = Operation_p (
                                    new StringOp (
                                       (format("\n// module %s\nnamespace %s {\n") 
                                               % m_mod_name 
                                               % s).str()
                                    ));
                result.source = result.header;
                // now we don't care about the non-normalized version.
                m_mod_name.swap(s);
                m_mode = mFound;
                break;              
            }
        }
        default: {
            result.header = Operation_p(new TokenOp(tok));
            result.source = result.header;
        }
    }
    return result;
}

OperationPair 
ExportStmtXForm::
process_upstream (OperationPair p, TransformContext *) {
//  cerr << endl << "[export_stmt: proc_upstream]" << endl;
    return p;
}

ExportStmtXForm::
~ExportStmtXForm () {}

