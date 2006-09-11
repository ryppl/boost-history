// import_stmt.cpp
#include "import_stmt.h"
#include "base_operations.h"
#include "lexpolicies.h"
#include "xformctx.h"
#include <boost/wave/token_ids.hpp>
#include <boost/format.hpp>

#include <iostream>

using namespace std;
using namespace boost::wave;
using boost::format;

// class ImportStmtXForm : public TransformStage {
//  std::string   m_mod_name;
//  enum mode { mImport, mNamespace, mModule, mFound } m_mode;
// public:
//  ImportStmtXForm (const context_iter_t& s,
//                   const context_iter_t& e);
//  virtual void at_start (TransformContext *);
//  virtual void at_end (TransformContext *);
//  virtual OperationPair process_token (const token_t& tok, 
//                                       TransformContext *);
//  virtual OperationPair process_upstream (OperationPair p, 
//                                          TransformContext *);
//  virtual ~ImportStmtXForm ();
// };

struct IncludeOp : public Operation {
    std::string m_str;
    IncludeOp (const std::string& s) : m_str(s) {}
    virtual void operator () (OutputDelegate * op) {
        op->include(m_str);
    }
    virtual ~IncludeOp () {}
};

ImportStmtXForm::
ImportStmtXForm (const context_iter_t& s,
                 const context_iter_t& e)
  : TransformStage (s,e), m_mode(mImport) {}
  
OperationPair 
ImportStmtXForm::
process_token (const token_t& tok, 
                         TransformContext * ctx) {
    OperationPair result;
    switch (m_mode) {
        case mImport:
            if (tok == T_IMPORT)
                m_mode = mNamespace;
            break;
        case mNamespace:
            if (tok == T_NAMESPACE)
                m_mode = mModule;
            break;
        case mModule:
            if (tok == T_SEMICOLON) {
                result.header = Operation_p(new IncludeOp(m_mod_name));
                result.source = result.header;
                m_mode = mFound;
            } else {
                if (   (tok == T_IDENTIFIER) || (tok == T_COLON_COLON) 
                    || (tok == T_LEFTBRACKET) || (tok == T_RIGHTBRACKET)
                    || (tok == T_STRINGLIT)) {
                    m_mod_name.append (tok.get_value().begin (),
                                       tok.get_value().end ());
                }
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


ImportStmtXForm::
~ImportStmtXForm () {
}
