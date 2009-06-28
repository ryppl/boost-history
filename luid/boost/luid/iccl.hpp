//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/luid for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_LUID_ICCL_HPP_
#define BOOST_LUID_ICCL_HPP_

#include "boost/luid/iccl/base.hpp"
#include "boost/luid/iccl/storage.hpp"
//#include "boost/luid/iccl/counter.hpp"
#include "boost/luid/iccl/upper_bound.hpp"
#include "boost/luid/iccl/static_lower_bound.hpp"
#include "boost/luid/iccl/dynamic_lower_bound.hpp"
#include "boost/luid/iccl/counter2.hpp"
#include "boost/luid/iccl/discard.hpp"
#include "boost/luid/iccl/fifo.hpp"
#include "boost/luid/iccl/rec_fifo_delay.hpp"
#include "boost/luid/iccl/on_overflow.hpp"
#include "boost/luid/iccl/coherency_bitset.hpp"
#include "boost/luid/iccl/set.hpp"
#include "boost/luid/iccl/internally_locked.hpp"
#include "boost/luid/iccl/externally_locked.hpp"
#include "boost/luid/iccl/facade.hpp"
#include "boost/luid/iccl/constructor.hpp"

/* 

Functional 
    make a luid
    recover a luid
    resize
    
Feature model
    + The unsigned integer type
        + numeric_type
        - The lower bound value : the first value
            + max
        - The upper bound value : the fact the generator bounds until a given value
            + max
            
    - An error policy used when there are no more free uid 
        - throw_exception
        - return_invalid_value
        
    + A releasing policy which control whether identifiers are discarded or 
        recovered when they    are released
        - discard: the released luid are not reusable
        - recover: the generator can recover identifiers that are released. 
                             This has two features
            + when the released values become reusable
                  - immediate: the released luid is immediatelly reusable
                - delay: the luid is reusable after given a time. 
                    This needs the following parameters
                    + time_type: the time type
                    + duration_type: the duration type
                    + now: how to get the current time
                    + duration: the delay after which the luid is reusable.
                - freeze: the luid is reusable once some minimal number of new luid 
                                    have been taken
                    + distance: the 
            + how the reusable values are reused
                - fifo:  reusable values are reused on fifo order
                - undefined: no constraint is added 
            
    + the scope of the luid generator
        - mono_threaded: only one single thread access the luid generator
        - multi_threaded: several threads in a single process share the luid generator
            + A locking policy
        - inter_process: several process share the luid generator
            + A locking policy
        
    + A locking policy determining the synchronization mechanisms for internal 
        access to shared resources.
        - internally: the generator ensure that the make and release operations 
            are syncronized on its scope
        - externally: the user needs to provide the needed synchronization.
        
    + The lifetime of the information identified by this luids
        - process: the information is lost once the process exits or shuts down
        - node: the information is lost once the node process exits or shuts down. 
                If a process on the scope restarts it will recover the information.
        - persistent: the information is preserved using some persistent mechanism.
        
    + An optimization policy 
        - speed: the used algorithms are efficient even if a lot of space is used
        - space: the needed space is minimazed even if this has as consequance a 
                        lost of efficiency.
    + extendability
    

    
Which feature must to be in th DCL
==================================
. Should the lower, upper bound be static or given at construction time? 
    I think that these values must be given at construction time.
    
. Should the low value be 0?
???

. Can the upper bound be changed? And how? 
    The upper bound should be changed following the same algorithm as a vector. 
    
. Should the duration be static, given at construction time or when releasing 
    the uid?
    I think that this must be given at construction time. 
    This simplify the algorithm.

. Configurability versus extendability
In order to manage with all this variations this library propose the use of a 
luidg generator with a high degree of configurablity using a domain specific 
language (DSL). The DSL expressions are transformed to a Genvoca stack of 
Implemntation Components Configuration Language(ICCL) by the generator.

Each IC is a mixin and all of them have same form

template <typename INNER>
class IC : public/protected/private INNER {
    ...
};

This DSL can not be extended without modifying the library. In order to make 
extensions we need to modify how these implementation components are 
arranged. An Extending DSL language could be provided to this concern. 
The language could consider replacing an IC by a user IC, removing an IC or 
inserting over a IC a user IC. This has as consequence that the implementation 
components must be defined as concepts. 
Another posibility is to let the user extend the DSL, and provide a another 
generator.
In any case the IC must be provided and the concep behind this concept must be 
explicit.

. Do we need to use Boost.Parameters
As the number of parameters is quite big the parameter should be given by name. 
The use of the Boost.Parameter seams to help to achieve that.

DSL
 luidg( [uint_type], 
                 on_overflow(throw_exception|return_invalid_value),
                 on_release(
                         discard
                    | recover(
                        when (
                             immediate
                        | frozen(uint_type)
                        | delayed(time_type, duration_type, time_type now())
                    ), 
                    how (
                            fifo 
                        | undefined
                    )
                ), 
                 scope(
                         mono_threaded
                    |    multi_threaded(locking(internally|externally))
                    |    inter_process(locking(internally|externally))
                ),
                 lifetime(
                         process
                    | node
                    |    persistent
                ),
                optimization(speed|space),
                extension (
                    | insert_over_synch(NewImpl)
                    | replace_synch(NewImpl)
                    | insert_over_error(NewImpl)
                    | replace_error(NewImpl)
                    | insert_over_recover(NewImpl)
                    | replace_recover(NewImpl)                
                    | insert_over_counter(NewImpl)
                    | replace_counter(NewImpl)
                    | insert_over_holder(NewImpl)
                    | replace_holder(NewImpl)
                    | insert_over_base(NewImpl)
                )*
                Level(
            )


ICCL: GenVoca Grammar for the Locally Unique Identifier Generator LUIDG
    _________________________________________________________________________
  LUIDG = luidg[OptLocking]
    _________________________________________________________________________
  OptLocking = 
          | internally_locked[InternallyLockingContext]
          | AltOnOverflow
     InternallyLockingContext: 
                multi_threaded[AltOnOverflow]
            | multi_process[AltOnOverflow]
    _________________________________________________________________________
  AltOnOverflow: 
        return_error_value[AltReleasing] 
          | throw_exception[AltReleasing]
    _________________________________________________________________________
  AltReleasing: 
        delay[Recoverable] 
      |    freeze[Recoverable] 
          | Counter
  Recoverable: recoverable[QueueContext]
    QueueContext: 
                raw_queue[Counter] 
            | shm_queue[Counter] 
            | mmfile_queue[Counter]
    _________________________________________________________________________
  Counter: counter[CounterContext]
     CounterContext: 
                raw_counter[Holder] 
             | shm_counter[Holder] 
            | mmfile_counter[Holder]
    _________________________________________________________________________
    Holder : 
                raw[Base] 
            | shm[Base] 
            | mmfile[Base]
    _________________________________________________________________________
  Base: base[Config]
  _________________________________________________________________________
  Config: [numeric_type] LUIDG
                  
  luidg[throw_exception[counter[raw_counter[raw[base[Config]]]]]]
  luidg[throw_exception[counter[shm_counter[shm[base[Config]]]]]]
  luidg[throw_exception[counter[mmfile_counter[mmfile[base[Config]]]]]]

  luidg[throw_exception[recover[raw_queue[
        counter[raw_counter[raw[base[Config]]]]]]]]
  luidg[throw_exception[recover[shm_queue[
        counter[shm_counter[shm[base[Config]]]]]]]]
  luidg[throw_exception[recover[mmfile_queue[
        counter[mmfile_counter[mmfile[base[Config]]]]]]]]
  
  luidg[throw_exception[delay[recover[raw_queue[
        counter[raw_counter[raw[base[Config]]]]]]]]]
  luidg[throw_exception[delay[recover[shm_queue[
        counter[shm_counter[shm[base[Config]]]]]]]]]
  luidg[throw_exception[delay[recover[mmfile_queue[
        counter[mmfile_counter[mmfile[base[Config]]]]]]]]]

  luidg[throw_exception[freeze[recover[raw_queue[
        counter[raw_counter[raw[base[Config]]]]]]]]]
  luidg[throw_exception[freeze[recover[shm_queue[
        counter[shm_counter[shm[base[Config]]]]]]]]]
  luidg[throw_exception[freeze[recover[mmfile_queue[
        counter[mmfile_counter[mmfile[base[Config]]]]]]]]]

  luidg[internally_locked[multi_treaded[
        throw_exception[freeze[recover[raw_queue[
        counter[raw[counter[raw[base[Config]]]]]]]]]]]
  luidg[internally_locked[multi_treaded[
        throw_exception[freeze[recover[shm_queue[
        counter[shm_counter[shm[base[Config]]]]]]]]]]]
  luidg[internally_locked[multi_treaded[
        throw_exception[freeze[recover[mmfile_queue[
        counter[mmfile_counter[mmfile[base[Config]]]]]]]]]]]

  luidg[internally_locked[interprocess[
        throw_exception[freeze[recover[shm_queue[
        counter[shm_counter[shm[base[Config]]]]]]]]]]]
  luidg[internally_locked[interprocess[
        throw_exception[freeze[recover[mmfile_queue[
        counter[mmfile_counter[mmfile[base[Config]]]]]]]]]]]
  
24+24+16
 * */
 
#endif /*ICCL_HPP_*/
