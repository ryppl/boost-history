/*
 *
 * Copyright (c) 2002
 * Dr John Maddock
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Dr John Maddock makes no representations
 * about the suitability of this software for any purpose.
 * It is provided "as is" without express or implied warranty.
 *
 */

 /*
  *   LOCATION:    see http://www.boost.org for most recent version.
  *   FILE         perl_matcher_common.cpp
  *   VERSION      see <boost/version.hpp>
  *   DESCRIPTION: Definitions of perl_matcher member functions that are 
  *                specific to the non-recursive implementation.
  */

#ifndef BOOST_REGEX_V4_PERL_MATCHER_NON_RECURSIVE_HPP
#define BOOST_REGEX_V4_PERL_MATCHER_NON_RECURSIVE_HPP

#include <new>

#ifdef __BORLANDC__
#  pragma option push -a8 -b -Vx -Ve -pc -w-8027
#endif

namespace boost{
namespace re_detail{

template <class T>
inline void destroy(T* p)
{
   (void)p;  // warning suppression
   p->~T();
}

enum saved_state_type
{
   saved_type_end = 0,
   saved_type_paren = 1,
   saved_type_recurse = 2,
   saved_type_assertion = 3,
   saved_state_alt = 4,
   saved_state_repeater_count = 5,
   saved_state_extra_block = 6
};

struct saved_state
{
   unsigned int id;
   saved_state(unsigned i) : id(i) {}
};

template <class BidiIterator>
struct saved_matched_paren : public saved_state
{
   int index;
   sub_match<BidiIterator> sub;
   saved_matched_paren(int i, const sub_match<BidiIterator>& s) : saved_state(1), index(i), sub(s){};
};

template <class BidiIterator>
struct saved_position : public saved_state
{
   const re_syntax_base* pstate;
   BidiIterator position;
   saved_position(const re_syntax_base* ps, BidiIterator pos, int i) : saved_state(i), pstate(ps), position(pos){};
};

template <class BidiIterator>
struct saved_assertion : public saved_position<BidiIterator>
{
   bool positive;
   saved_assertion(bool p, const re_syntax_base* ps, BidiIterator pos) 
      : saved_position<BidiIterator>(ps, pos, saved_type_assertion), positive(p){};
};

template <class BidiIterator>
struct saved_repeater : public saved_state
{
   repeater_count<BidiIterator> count;
   saved_repeater(int i, repeater_count<BidiIterator>** s, BidiIterator start) 
      : saved_state(saved_state_repeater_count), count(i,s,start){}
};

struct saved_extra_block : public saved_state
{
   saved_state *base, *end;
   saved_extra_block(saved_state* b, saved_state* e) 
      : saved_state(saved_state_extra_block), base(b), end(e) {}
};

struct save_state_init
{
   saved_state** stack;
   save_state_init(saved_state** base, saved_state** end)
      : stack(base)
   {
      *base = static_cast<saved_state*>(block_cache.get());
      *end = reinterpret_cast<saved_state*>(reinterpret_cast<char*>(*base)+BOOST_REGEX_BLOCKSIZE);
      --(*end);
      new (*end)saved_state(0);
      assert(*end > *base);
   }
   ~save_state_init()
   {
      block_cache.put(*stack);
      *stack = 0;
   }
};

template <class BidiIterator, class Allocator, class traits, class Allocator2>
bool perl_matcher<BidiIterator, Allocator, traits, Allocator2>::match_all_states()
{
   if(state_count > max_state_count)
      raise_error(traits_inst, REG_ESPACE);
   push_recursion_stopper();
   do{
      while(pstate)
      {
         matcher_proc_type proc = s_match_vtable[pstate->type];
         ++state_count;
         if(!(this->*proc)())
         {
            if((m_match_flags & match_partial) && (position == last))
               m_has_partial_match = true;
            if(false == unwind(false))
               return false;
         }
      }
   }while(unwind(true));
   return m_recursive_result;
}

template <class BidiIterator, class Allocator, class traits, class Allocator2>
void perl_matcher<BidiIterator, Allocator, traits, Allocator2>::extend_stack()
{
   if(used_block_count)
   {
      --used_block_count;
      saved_state* stack_base;
      saved_state* backup_state;
      stack_base = static_cast<saved_state*>(block_cache.get());
      backup_state = reinterpret_cast<saved_state*>(reinterpret_cast<char*>(stack_base)+BOOST_REGEX_BLOCKSIZE);
      saved_extra_block* block = static_cast<saved_extra_block*>(backup_state);
      --block;
      new (block) saved_extra_block(m_stack_base, m_backup_state);
      m_stack_base = stack_base;
      m_backup_state = block;
   }
   else
      raise_error(traits_inst, REG_E_MEMORY);
}

template <class BidiIterator, class Allocator, class traits, class Allocator2>
inline void perl_matcher<BidiIterator, Allocator, traits, Allocator2>::push_matched_paren(int index, const sub_match<BidiIterator>& sub)
{
   assert(index);
   saved_matched_paren<BidiIterator>* pmp = static_cast<saved_matched_paren<BidiIterator>*>(m_backup_state);
   --pmp;
   if(pmp < m_stack_base)
   {
      extend_stack();
      pmp = static_cast<saved_matched_paren<BidiIterator>*>(m_backup_state);
      --pmp;
   }
   new (pmp)saved_matched_paren<BidiIterator>(index, sub);
   m_backup_state = pmp;
}

template <class BidiIterator, class Allocator, class traits, class Allocator2>
inline void perl_matcher<BidiIterator, Allocator, traits, Allocator2>::push_recursion_stopper()
{
   saved_state* pmp = m_backup_state;
   --pmp;
   if(pmp < m_stack_base)
   {
      extend_stack();
      pmp = m_backup_state;
      --pmp;
   }
   new (pmp)saved_state(saved_type_recurse);
   m_backup_state = pmp;
}

template <class BidiIterator, class Allocator, class traits, class Allocator2>
inline void perl_matcher<BidiIterator, Allocator, traits, Allocator2>::push_assertion(const re_syntax_base* ps, bool positive)
{
   saved_assertion<BidiIterator>* pmp = static_cast<saved_assertion<BidiIterator>*>(m_backup_state);
   --pmp;
   if(pmp < m_stack_base)
   {
      extend_stack();
      pmp = static_cast<saved_assertion<BidiIterator>*>(m_backup_state);
      --pmp;
   }
   new (pmp)saved_assertion<BidiIterator>(positive, ps, position);
   m_backup_state = pmp;
}

template <class BidiIterator, class Allocator, class traits, class Allocator2>
inline void perl_matcher<BidiIterator, Allocator, traits, Allocator2>::push_alt(const re_syntax_base* ps)
{
   saved_position<BidiIterator>* pmp = static_cast<saved_position<BidiIterator>*>(m_backup_state);
   --pmp;
   if(pmp < m_stack_base)
   {
      extend_stack();
      pmp = static_cast<saved_position<BidiIterator>*>(m_backup_state);
      --pmp;
   }
   new (pmp)saved_position<BidiIterator>(ps, position, saved_state_alt);
   m_backup_state = pmp;
}

template <class BidiIterator, class Allocator, class traits, class Allocator2>
inline void perl_matcher<BidiIterator, Allocator, traits, Allocator2>::push_repeater_count(int i, repeater_count<BidiIterator>** s)
{
   saved_repeater<BidiIterator>* pmp = static_cast<saved_repeater<BidiIterator>*>(m_backup_state);
   --pmp;
   if(pmp < m_stack_base)
   {
      extend_stack();
      pmp = static_cast<saved_repeater<BidiIterator>*>(m_backup_state);
      --pmp;
   }
   new (pmp)saved_repeater<BidiIterator>(i, s, position);
   m_backup_state = pmp;
}

template <class BidiIterator, class Allocator, class traits, class Allocator2>
bool perl_matcher<BidiIterator, Allocator, traits, Allocator2>::match_startmark()
{
   int index = static_cast<const re_brace*>(pstate)->index;
   switch(index)
   {
   case 0:
      pstate = pstate->next.p;
      break;
   case -1:
   case -2:
      {
         // forward lookahead assert:
         const re_syntax_base* next_pstate = static_cast<const re_jump*>(pstate->next.p)->alt.p->next.p;
         pstate = pstate->next.p->next.p;
         push_assertion(next_pstate, index == -1);
         break;
      }
   default:
   {
      assert(index > 0);
      push_matched_paren(index, (*m_presult)[index]);
      m_presult->set_first(position, index);
      pstate = pstate->next.p;
      break;
   }
   }
   return true;
}

template <class BidiIterator, class Allocator, class traits, class Allocator2>
bool perl_matcher<BidiIterator, Allocator, traits, Allocator2>::match_alt()
{
   bool take_first, take_second;
   const re_jump* jmp = static_cast<const re_jump*>(pstate);

   // find out which of these two alternatives we need to take:
   if(position == last)
   {
      take_first = jmp->can_be_null & mask_take;
      take_second = jmp->can_be_null & mask_skip;
   }
   else
   {
      take_first = access::can_start(*position, jmp->_map, (unsigned char)mask_take);
      take_second = access::can_start(*position, jmp->_map, (unsigned char)mask_skip);
  }

   if(take_first)
   {
      // we can take the first alternative,
      // see if we need to push next alternative:
      if(take_second)
      {
         push_alt(jmp->alt.p);
      }
      pstate = pstate->next.p;
      return true;
   }
   if(take_second)
   {
      pstate = jmp->alt.p;
      return true;
   }
   return false;  // neither option is possible
}

template <class BidiIterator, class Allocator, class traits, class Allocator2>
bool perl_matcher<BidiIterator, Allocator, traits, Allocator2>::match_rep()
{
#ifdef BOOST_MSVC
#pragma warning(push)
#pragma warning(disable:4127 4244)
#endif
   const re_repeat* rep = static_cast<const re_repeat*>(pstate);

   // find out which of these two alternatives we need to take:
   bool take_first, take_second;
   if(position == last)
   {
      take_first = rep->can_be_null & mask_take;
      take_second = rep->can_be_null & mask_skip;
   }
   else
   {
      take_first = access::can_start(*position, rep->_map, (unsigned char)mask_take);
      take_second = access::can_start(*position, rep->_map, (unsigned char)mask_skip);
   }

   if(take_first || (next_count->get_id() != rep->id))
   {
      // we're moving to a different repeat from the last
      // one, so set up a counter object:
      push_repeater_count(rep->id, &next_count);
   }
   //
   // If we've had at least one repeat already, and the last one 
   // matched the NULL string then set the repeat count to
   // maximum:
   //
   next_count->check_null_repeat(position, rep->max);

   if(next_count->get_count() < rep->min)
   {
      // we must take the repeat:
      if(take_first)
      {
         // increase the counter:
         ++(*next_count);
         pstate = rep->next.p;
         return true;
      }
      return false;
   }

   if(rep->greedy)
   {
      // try and take the repeat if we can:
      if((next_count->get_count() < rep->max) && take_first)
      {
         if(take_second)
         {
            // store position in case we fail:
            push_alt(rep->alt.p);
         }
         // increase the counter:
         ++(*next_count);
         pstate = rep->next.p;
         return true;
      }
      else if(take_second)
      {
         pstate = rep->alt.p;
         return true;
      }
      return false; // can't take anything, fail...
   }
   else // non-greedy
   {
      // try and skip the repeat if we can:
      if(take_second)
      {
         // store position in case we fail:
         BidiIterator pos = position;
         pstate = rep->alt.p;
         if(match_all_states())
            return true;
         // failed alternative, reset posistion and fall through for repeat:
         position = pos;
      }
      if((next_count->get_count() < rep->max) && take_first)
      {
         // increase the counter:
         ++(*next_count);
         pstate = rep->next.p;
         return match_all_states();
      }
   }
   return false;
#ifdef BOOST_MSVC
#pragma warning(pop)
#endif
}

/****************************************************************************

Unwind and associated proceedures follow, these perform what normal stack
unwinding does in the recursive implementation.

****************************************************************************/

template <class BidiIterator, class Allocator, class traits, class Allocator2>
bool perl_matcher<BidiIterator, Allocator, traits, Allocator2>::unwind(bool have_match)
{
   m_recursive_result = have_match;
   unwind_proc_type unwinder;
   bool cont;
   //
   // keep unwinding our stack until we have something to do:
   //
   do
   {
      unwinder = s_unwind_table[m_backup_state->id];
      cont = (this->*unwinder)(m_recursive_result);
   }while(cont);
   //
   // return true if we have more states to try:
   //
   return pstate ? true : false;
}

template <class BidiIterator, class Allocator, class traits, class Allocator2>
bool perl_matcher<BidiIterator, Allocator, traits, Allocator2>::unwind_end(bool)
{
   pstate = 0;   // nothing left to search
   return false; // end of stack nothing more to search
}

template <class BidiIterator, class Allocator, class traits, class Allocator2>
bool perl_matcher<BidiIterator, Allocator, traits, Allocator2>::unwind_paren(bool have_match)
{
   saved_matched_paren<BidiIterator>* pmp = static_cast<saved_matched_paren<BidiIterator>*>(m_backup_state);
   // restore previous values if no match was found:
   if(have_match == false)
   {
      m_presult->set_first(pmp->sub.first, pmp->index);
      m_presult->set_second(pmp->sub.second, pmp->index, pmp->sub.matched);
   }
   // unwind stack:
   m_backup_state = pmp+1;
   destroy(pmp);
   return true; // keep looking
}

template <class BidiIterator, class Allocator, class traits, class Allocator2>
bool perl_matcher<BidiIterator, Allocator, traits, Allocator2>::unwind_recursion_stopper(bool)
{
   destroy(m_backup_state++);
   pstate = 0;   // nothing left to search
   return false; // end of stack nothing more to search
}

template <class BidiIterator, class Allocator, class traits, class Allocator2>
bool perl_matcher<BidiIterator, Allocator, traits, Allocator2>::unwind_assertion(bool r)
{
   saved_assertion<BidiIterator>* pmp = static_cast<saved_assertion<BidiIterator>*>(m_backup_state);
   pstate = pmp->pstate;
   position = pmp->position;
   bool result = (r == pmp->positive);
   m_recursive_result = pmp->positive ? r : !r;
   destroy(pmp++);
   m_backup_state = pmp;
   return !result; // return false if the assertion was matched to stop search.
}

template <class BidiIterator, class Allocator, class traits, class Allocator2>
bool perl_matcher<BidiIterator, Allocator, traits, Allocator2>::unwind_alt(bool r)
{
   saved_position<BidiIterator>* pmp = static_cast<saved_position<BidiIterator>*>(m_backup_state);
   if(!r)
   {
      pstate = pmp->pstate;
      position = pmp->position;
   }
   destroy(pmp++);
   m_backup_state = pmp;
   return r; 
}

template <class BidiIterator, class Allocator, class traits, class Allocator2>
bool perl_matcher<BidiIterator, Allocator, traits, Allocator2>::unwind_repeater_counter(bool)
{
   saved_repeater<BidiIterator>* pmp = static_cast<saved_repeater<BidiIterator>*>(m_backup_state);
   destroy(pmp++);
   m_backup_state = pmp;
   return true; // keep looking
}

template <class BidiIterator, class Allocator, class traits, class Allocator2>
bool perl_matcher<BidiIterator, Allocator, traits, Allocator2>::unwind_extra_block(bool)
{
   saved_extra_block* pmp = static_cast<saved_extra_block*>(m_backup_state);
   void* condemmed = m_stack_base;
   m_stack_base = pmp->base;
   m_backup_state = pmp->end;
   destroy(pmp);
   block_cache.put(condemmed);
   return true; // keep looking
}

template <class BidiIterator, class Allocator, class traits, class Allocator2>
typename perl_matcher<BidiIterator, Allocator, traits, Allocator2>::unwind_proc_type const
perl_matcher<BidiIterator, Allocator, traits, Allocator2>::s_unwind_table[] = 
{
   &perl_matcher<BidiIterator, Allocator, traits, Allocator2>::unwind_end,
   &perl_matcher<BidiIterator, Allocator, traits, Allocator2>::unwind_paren,
   &perl_matcher<BidiIterator, Allocator, traits, Allocator2>::unwind_recursion_stopper,
   &perl_matcher<BidiIterator, Allocator, traits, Allocator2>::unwind_assertion,
   &perl_matcher<BidiIterator, Allocator, traits, Allocator2>::unwind_alt,
   &perl_matcher<BidiIterator, Allocator, traits, Allocator2>::unwind_repeater_counter,
   &perl_matcher<BidiIterator, Allocator, traits, Allocator2>::unwind_extra_block,
};

}
} // namespace boost

#ifdef __BORLANDC__
#  pragma option pop
#endif

#endif
