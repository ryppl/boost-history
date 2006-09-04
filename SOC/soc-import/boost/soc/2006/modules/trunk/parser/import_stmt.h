/** @file import_stmt.h
	\brief Implementation of the import namespace XX statment.
	
	We eat our tokens up altogether, and pop up an #include
*/
#ifndef INCLUDE_IMPORT_STMT_H
#define INCLUDE_IMPORT_STMT_H

#include "operations.h"
#include <string>

class ImportStmtXForm : public TransformStage {
	std::string   m_mod_name;
	enum mode { mImport, mNamespace, mModule, mFound } m_mode;
public:
	ImportStmtXForm (const context_iter_t& s,
	                 const context_iter_t& e);
	virtual OperationPair process_token (const token_t& tok, 
	                                     TransformContext *);
	virtual OperationPair process_upstream (OperationPair p, 
	                                        TransformContext *);
	virtual ~ImportStmtXForm ();
};


#endif