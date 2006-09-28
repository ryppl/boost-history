/** @file method_body.h
    \brief TransformStages and Operations for supporting method body transformation.
    
    Pass-through for source code, but replaces the body with a semicolon
    for the header.  Doesn't touch anything for process_upstream.
*/
#ifndef INCLUDE_METHOD_BODY_H
#define INCLUDE_METHOD_BODY_H

#include "operations.h"

/// For the header, puts out only a declaration, full definition for the source.
/// used for either free functions, inline methods, or whatever.
class MethodBodyXForm : public TransformStage {
    int m_depth;
    bool m_started;
public:
    MethodBodyXForm (const context_iter_t& s,
                     const context_iter_t& e);
//  virtual void at_start ();
//  virtual void at_end ();
	static const std::string& get_identifier ();
	virtual const std::string& identifier () const;
    virtual OperationPair process_token (const token_t& tok, TransformContext *);
    virtual OperationPair process_upstream (OperationPair p, TransformContext *);
    virtual ~MethodBodyXForm ();
};

#endif