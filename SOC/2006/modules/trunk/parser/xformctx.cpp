// xformctx.cpp
#include "xformctx.h"
#include "base_operations.h"
#include <assert.h>
using namespace std;

bool 
operator< (const TransformStage& left, const TransformStage& right) {
    return left.start < right.start;
}

bool 
operator< (const TransformStage_p left, const TransformStage_p right) {
    return *(left.get()) < *(right.get());
}


bool 
operator == (const context_iter_t& a, const context_iter_t& b) {
    return (!(a<b)) && (!(b<a));
}

bool 
operator >= (const context_iter_t& a, const context_iter_t& b) {
    return (a == b) || (b < a);
}

bool 
operator > (const context_iter_t& a, const context_iter_t& b) {
    return (b < a);
}

// the default definition works as a nice null operation.
Operation_p Operation::null_op = boost::shared_ptr<Operation>(new Operation);

TransformContext::
TransformContext () : m_moving (false), m_header_emit(false) {}

TransformContext::
~TransformContext () {}

void
TransformContext::
add (const TransformStage_p m) {
    assert (!m_moving);
    // do our own sorted insert.
    if (m_annotations.empty ()) {
        m_annotations.push_back (m);
    } else {
        list<TransformStage_p>::iterator it = m_annotations.begin ();
        while (   (it != m_annotations.end ()) 
               && (*it->get() < *m.get())) {
             ++it;
        }       
        m_annotations.insert (it, m);
    }
}


void
TransformContext::
set_position (const context_iter_t& pos) {
    // start our traversal.
    // our annotations list is locked now.
    if (!m_moving) {
        m_ann_pos = m_annotations.begin ();
        m_moving = true;
    }
    
//  assert (pos >= m_ann_pos->start);
    
    // first, scan the current state for any TransformStages we
    // have to leave.  Remove them in stack order, and
    // call their at_end()s.
    while (m_curstate.size () && m_curstate.back()->end <= pos) {
        m_curstate.back()->at_end(this);
        m_curstate.pop_back ();
    }
    
    // next, add any new items to the stack, and invoke
    // their at_begin()s.
    while (   (m_ann_pos != m_annotations.end ())
           && ((*m_ann_pos)->start <= pos)) {
        m_curstate.push_back (*m_ann_pos);
        (*m_ann_pos)->at_start(this);
        m_ann_pos++;
    }
}

TransformStage_w 
TransformContext::
upward_stage (const std::string& pattern, TransformStage *start) {
    TransformStage_w result;
    
    if (m_curstate.empty ()) 
        return result;
        
    // first, figure out where 'start' is in the chain.
    list<TransformStage_p>::reverse_iterator ct = m_curstate.rbegin ();
    while (ct != m_curstate.rend() && ct->get() != start)
        ++ct;

    // now, scan from here for the proper identifier.
    while (ct != m_curstate.rend() && (*ct)->identifier() != pattern)
        ++ct;

    if (ct != m_curstate.rend())
        result = TransformStage_w(*ct);

    return result;
}

/// TODO: this single-token iteration results in several
/// mallocs per character.  UGLY.  Well, that's straightforward
/// enough to solve (in a bit) with some forward-scanning of the
/// context stack.
void
TransformContext::
output (const context_iter_t& start,
        const context_iter_t& end,
        OutputDelegate* header,
        OutputDelegate* source) {
    
    // go through the text again, setting up the stack for
    // each token, and process the token through the entire
    // stack, until we append the properly transformed Operation_s
    // to the proper vectors for final emission.
    for (context_iter_t it = start;
         it != end;
         ++it) {
        set_position(it);
        
        list<TransformStage_p>::reverse_iterator ct = m_curstate.rbegin ();
        if (ct != m_curstate.rend ()) {
            OperationPair result = (*ct)->process_token (*it,this);
            while (++ct != m_curstate.rend ()) {
                result = (*ct)->process_upstream(result,this);
            }
            if (m_header_emit) {
                add_header(result.header);
            }
            add_source(result.source);
        } else {
            // no active transforms on this at all.
            // push it to the source, no modification.
            add_source(Operation_p(new TokenOp(*it)));
        }
    }
    
    // now execute the Operations on the source and header.
    vector<Operation_p>::iterator op_it;
    for (op_it = m_for_header.begin ();
         op_it != m_for_header.end ();
         ++op_it) {
        (*(op_it->get()))(header);
    }
    for (op_it = m_for_source.begin ();
         op_it != m_for_source.end ();
         ++op_it) {
        (*(op_it->get()))(source);
    }
}
