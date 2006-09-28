/** @file template_stmt.h
    \brief Processing for templates
    
    Essentially we un-collapse template methods and functions.
*/
#ifndef INCLUDE_TEMPLATE_STMT_H
#define INCLUDE_TEMPLATE_STMT_H

#include "operations.h"


class TemplateStmtXForm : public TransformStage {
public:
    TemplateStmtXForm (const context_iter_t& s,
                       const context_iter_t& e);
//  virtual void at_start (TransformContext *);
//  virtual void at_end (TransformContext *);
	static const std::string& get_identifier ();
	virtual const std::string& identifier () const;
    virtual OperationPair process_token (const token_t& tok, 
                                         TransformContext *);
    virtual OperationPair process_upstream (OperationPair p, 
                                            TransformContext *);
    
    virtual ~TemplateStmtXForm ();
};

#endif
