#ifndef INCLUDE_XFORMCTX_H
#define INCLUDE_XFORMCTX_H

#include "lexpolicies.h"
#include "../driver/output.h"
#include <vector>
#include <stack>
/** @file xformctx.h
	\brief Transformation Context Management
	
	We go through three passes to get our transformations complete:
	-# An analysis phase, where we store some ContextEntrys around specific
	   ranges of iterators.  As we may have some backtracking during parse,
	   we need this to be a seperate phase from the next, so we enter that
	   phase with a fully-known parse.
	-# A transform phase, where the ContextEntrys are arranged on a stack
	   as their relevant portions of the source text come in and out of
	   scope.  The ContextEntrys transform the text, generating transformed
	   text for both source and header versions.
	-# A generation phase, where the transformed text is streamed to files.
	   This phase is seperate as the prior phase may cause out-of-order
	   text generation, such as adding file #includes.
*/

struct Operation;
typedef boost::shared_ptr<Operation> Operation_p;

/// Individual outputters.  These are run against either the header or 
/// source, causing text to be added (possibly out of order).
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
};

struct ContextEntry {
	context_iter_t start, end;
//	MarkupFunctor *at_start, *at_end;
	virtual void at_start () {}
	virtual void at_end () {}
	/// when this entry's at the top of the stack, it 
	virtual OperationPair  process_token (const token_t& tok) = 0;
	virtual OperationPair  process_upstream (OperationPair p) = 0;
	virtual ~ContextEntry () {}
};
typedef boost::shared_ptr<ContextEntry>  ContextEntry_p;

bool operator< (const ContextEntry& left, const ContextEntry& right);

// Single instance per file transformed
class TransformContext {
	std::list<ContextEntry_p>    m_annotations;
	std::list<ContextEntry_p>    m_curstate;
	context_iter_t               m_position;
	bool                         m_moving;
	std::list<ContextEntry_p>::iterator m_ann_pos;

//	OutputDelegate      *m_outdel;

	/**
	  Move to the specified state in the iteration
	  range.
	*/
	void set_position (const context_iter_t& pos);


public:
	TransformContext (/*OutputDelegate *del*/);
	~TransformContext ();


	/** @name TransformationContext_State
	 * The Context has two phases of operation: a construction
	 * phase, and a playback phase.  Use add() to put in ContextEntry
	 * objects, and set_position() for playback.  Once set_position()
	 * is called, don't call add().  set_position can't be used
	 * for reverse operations.
	 */
	//@{

		/**
		   Add a ContextEntry object to our internal stack.
		   
		   Ordering Note: We know that the ContextEntry objects
		   are going to be added in a semi-stack order, so we
		   sort with higher precedence for later objects.
		   (e.g. for two identical start positions, the object
		   added latest will have its at_start() functor run 
		   first )
		   
		   Copying note: we make one copy of the ContextEntry object
		   we're handed.
		*/
		void add (ContextEntry_p m);
	
		void output (const context_iter_t& start,
		             const context_iter_t& end,
		             OutputDelegate * header,
		             OutputDelegate * source);
	

	//@}
};

#endif
