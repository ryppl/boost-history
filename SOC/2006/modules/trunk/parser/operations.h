/** @file operations.h
    \brief Operations and Transformation Contexts
    
    The transformation engine works by wrapping up tokens
    into an intermediate representation (IR) called Operations.
    These are functors that emit tokens into an OutputDelegate,
    which saves them for eventual emission into a generated file.
    
    TransformStages wrap up tokens into Operations in their process_token
    methods (possibly removing or replacing them as they go).  Two
    Operations are emitted: one for the header file, and another for
    the source.  Any other TransformStages on the stack below the top
    will receive the created Operations and can copy, delete, or
    replace them at will.
    
    Finally the resulting Operations are executed upon the OutputDelegate
    by the TransformContext
*/


#ifndef OPERATIONS_H
#define OPERATIONS_H
#include "lexpolicies.h"
#include <boost/shared_ptr.hpp>

class OutputDelegate;

struct Operation;
typedef boost::shared_ptr<Operation> Operation_p;

/// Individual outputters.  These are run against either the header or 
/// source, causing text to be added (possibly out of order).
/// TODO: we should add a combining API to let these get aggregated
/// when they're enqueued for the OutputDelegate.  That'll keep the
/// memory hit less ridiculous.
struct Operation {
    /// Can call out () to output strings as needed,
    /// and add any #include paths needed.
    virtual void operator ()(OutputDelegate *) {}
    virtual ~Operation () {}
    /// a no-op
    static Operation_p   null_op;
};

/// Two Operation_ps, communicated down the stack, down to the
/// actual OutputDelegates.
struct OperationPair {
    Operation_p  header;
    Operation_p  source;
    OperationPair () : header(Operation::null_op), 
                       source (Operation::null_op) {}
};

class TransformContext;

struct TransformStage {
    context_iter_t start, end;
    TransformStage (const context_iter_t& s, const context_iter_t& e)
      : start(s), end(e) {}
//  MarkupFunctor *at_start, *at_end;

    // identifiers the type of stage, for inter-stage communication.
    virtual const std::string& identifier () const = 0;

    virtual void at_start (TransformContext *) {}
    virtual void at_end (TransformContext *) {}
    /// when this entry's at the top of the stack, it 
    virtual OperationPair  process_token (const token_t& tok, 
                                          TransformContext *) = 0;
    virtual OperationPair  process_upstream (OperationPair p,
                                             TransformContext *) = 0;
    virtual ~TransformStage () {}
};
typedef boost::shared_ptr<TransformStage>  TransformStage_p;
typedef boost::weak_ptr<TransformStage>    TransformStage_w;

bool operator< (const TransformStage& left, const TransformStage& right);
bool operator< (const TransformStage_p left, const TransformStage& right);


#endif
