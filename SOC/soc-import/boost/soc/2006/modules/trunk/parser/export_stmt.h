/** @file export_stmt.h
    \brief Processing for export statements
    
    We prefix and suffix our text with a namespace declaration, and 
    enable header emission in the TransformContext.
*/
#ifndef INCLUDE_EXPORT_STMT_H
#define INCLUDE_EXPORT_STMT_H

#include "operations.h"
#include "modulename.h"
#include <string>

class ExportStmtXForm : public TransformStage {
    ModuleName   m_mod_name;
    enum mode { mExport, mNamespace, mModule, mFound } m_mode;
    std::string open_block ();	
    std::string close_block ();
public:
    ExportStmtXForm (const context_iter_t& s,
                     const context_iter_t& e);
	ModuleName modname () const { return m_mod_name; }
	static const std::string& get_identifier ();
	virtual const std::string& identifier () const;                     
    virtual void at_start (TransformContext *);
    virtual void at_end (TransformContext *);
    virtual OperationPair process_token (const token_t& tok, 
                                         TransformContext *);
    virtual OperationPair process_upstream (OperationPair p, 
                                            TransformContext *);
    virtual ~ExportStmtXForm ();
};

#endif