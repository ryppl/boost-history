/** @file access_ctl.h
    \brief Module Access Control Systems.
    
    TransformStages for access control in modules.
*/
#ifndef INCLUDE_ACCESS_CTL_H
#define INCLUDE_ACCESS_CTL_H
#include "operations.h"

class AccessCtlXForm : public TransformStage {
    /// the access mode we're employing here.
    enum access_mode { mPublic, mPrivate } m_access;
    
    /// Whether we've hit an access control token yet.
    enum parse_mode { mNotFound, mColon, mFound, mNormal } m_parse;
    std::string m_ns_name;
    void insert_private (TransformContext * ctx);
public:
    AccessCtlXForm (const context_iter_t& s,
                    const context_iter_t& e);
    virtual void at_start (TransformContext *);
    virtual void at_end (TransformContext *);
    virtual OperationPair process_token (const token_t& tok, 
                                         TransformContext *);
    virtual OperationPair process_upstream (OperationPair p, 
                                            TransformContext *);
    virtual ~AccessCtlXForm ();
};

#endif