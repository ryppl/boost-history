
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

#ifndef OBSERVE_HPP_
#define OBSERVE_HPP_

#include <contract.hpp>
#include <list>
#include <algorithm>

/** Observer. */
class observer {
    friend class subject;

    CONTRACT_CLASS( (observer) )

public:
    // Creation //

    /** Create observer. */
    observer(void)
    CONTRACT_CONSTRUCTOR(
            (public) (observer)( (void) )
    (body) ({
    }) )

    /** Destroy observer. */
    virtual ~observer(void)
    CONTRACT_DESTRUCTOR(
            (public) (virtual) (observer)( (void) )
    (body) ({
    }) )

protected:
    // Commands //

    /** If up to date with its subject. */
    virtual bool up_to_date_with_subject(void) const
    CONTRACT_FUNCTION(
            (protected) (virtual) (bool) (up_to_date_with_subject)(
                    (void) ) (const)
    (body) (
        = 0;
    ) )

    virtual void update(void)
    CONTRACT_FUNCTION(
            (protected) (virtual) (void) (update)( (void) )
    (postcondition) ({
        CONTRACT_ASSERT( up_to_date_with_subject() );
    })
    (body) (
        = 0;
    ) )
};

/** Subject for observer design pattern. */
class subject {

    CONTRACT_CLASS( (subject)
    (invariant) ({
        for (std::list<observer*>::const_iterator
                i = observers_.begin(); i != observers_.end(); ++i)
            CONTRACT_ASSERT( *i );
    }) )

public:
    // Creation //

    /** Construct subject. */
    subject(void): observers_()
    CONTRACT_CONSTRUCTOR(
            (public) (subject)( (void) )
    (body) ({
    }) )

    /** Destroy subject. */
    virtual ~subject(void)
    CONTRACT_DESTRUCTOR(
            (public) (virtual) (subject)( (void) )
    (body) ({
    }) )

    // Queries //

    /** If given observer is attached. */
    bool attached(const observer* const ob) const
    CONTRACT_FUNCTION(
            (public) (bool) (attached)( (const observer* const)(ob) ) (const)
    (precondition) ({
        CONTRACT_ASSERT( ob );
    })
    (postcondition) (result) ({
        const std::list<const observer*> obs = observers();
        CONTRACT_ASSERT( result == (std::find(obs.begin(), obs.end(),
                ob) != obs.end()) );
    })
    (body) ({
        return std::find(observers_.begin(), observers_.end(),
                ob) != observers_.end();
    }) )

    // Commands //

    /** Remember given objects as on of the subject observers. */
    void attach(observer* const ob)
    CONTRACT_FUNCTION(
            (public) (void) (attach)( (observer* const)(ob) ) (copyable)
    (precondition) ({
        CONTRACT_ASSERT( ob );
        CONTRACT_ASSERT( !attached(ob) );
    })
    (postcondition) ({
        CONTRACT_ASSERT( attached(ob) );

        // Frame rule.
        const std::list<const observer*>& old =
                CONTRACT_OLDOF(this)->observers();
        std::list<const observer*> now = observers();
        std::remove(now.begin(), now.end(), ob);
        std::list<const observer*>::const_iterator nowi = now.begin();
        std::list<const observer*>::const_iterator oldi = old.begin();
        while (now.end() != nowi && old.end() != oldi) {
            CONTRACT_ASSERT_MSG( *nowi == *oldi,
                    "all other observers unchanged" );
            ++nowi;
            ++oldi;
        }
    })
    (body) ({
        observers_.push_back(ob);
    }) )

protected:
    // Queries //

    /** All observers attached to this subject.
     * This is protected because it is intended to support contract
     * specification only (including contracts of derived classes).
     * See related discussion in [Mitchell2002] Section 9.9. */
    std::list<const observer*> observers(void) const
    CONTRACT_FUNCTION(
            (protected) (std::list<const observer*>) (observers)(
                    (void) ) (const)
    (body) ({
        // Create list of pointers to const observers.
        std::list<const observer*> obs;
        for (std::list<observer*>::const_iterator
                i = observers_.begin(); i != observers_.end(); ++i)
            obs.push_back(*i);
        return obs;
    }) )

    // Commands //

    /** Update all attached observers. */
    void notify(void)
    CONTRACT_FUNCTION(
            (protected) (void) (notify)( (void) )
    (postcondition) ({
        const std::list<const observer*> obs = observers();
        for (std::list<const observer*>::const_iterator
                i = obs.begin(); i != obs.end(); ++i) {
            CONTRACT_ASSERT( *i );
            CONTRACT_ASSERT( (*i)->up_to_date_with_subject() );
        }
    })
    (body) ({
        for (std::list<observer*>::iterator
                i = observers_.begin(); i != observers_.end(); ++i)
            // Invariant ensures no null pointers in observers.
            (*i)->update();
    }) )

private:
    std::list<observer*> observers_;
};

#endif // #include guard

