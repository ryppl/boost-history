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
  *                specific to the recursive implementation.
  */

#ifndef BOOST_REGEX_V4_PERL_MATCHER_RECURSIVE_HPP
#define BOOST_REGEX_V4_PERL_MATCHER_RECURSIVE_HPP

#ifdef __BORLANDC__
#  pragma option push -a8 -b -Vx -Ve -pc -w-8027
#endif

namespace boost{
namespace re_detail{

template <class BidiIterator>
class backup_subex
{
   int index;
   sub_match<BidiIterator> sub;
public:
   template <class A>
   backup_subex(const match_results<BidiIterator, A>& w, int i)
      : index(i), sub(w[i]) {}
   template <class A>
   void restore(match_results<BidiIterator, A>& w)
   {
      w.set_first(sub.first, index);
      w.set_second(sub.second, index, sub.matched);
   }
};

template <class BidiIterator, class Allocator, class traits, class Allocator2>
bool perl_matcher<BidiIterator, Allocator, traits, Allocator2>::match_all_states()
{
   if(state_count > max_state_count)
      raise_error(traits_inst, REG_ESPACE);
   while(pstate)
   {
      matcher_proc_type proc = s_match_vtable[pstate->type];
      ++state_count;
      if(!(this->*proc)())
      {
         if((m_match_flags & match_partial) && (position == last))
            m_has_partial_match = true;
         return 0;
      }
   }
   return true;
}

template <class BidiIterator, class Allocator, class traits, class Allocator2>
bool perl_matcher<BidiIterator, Allocator, traits, Allocator2>::match_startmark()
{
   int index = static_cast<const re_brace*>(pstate)->index;
   bool r = true;
   switch(index)
   {
   case 0:
      pstate = pstate->next.p;
      break;
   case -1:
   case -2:
      {
         // forward lookahead assert:
         BidiIterator old_position(position);
         const re_syntax_base* next_pstate = static_cast<const re_jump*>(pstate->next.p)->alt.p->next.p;
         pstate = pstate->next.p->next.p;
         r = match_all_states();
         pstate = next_pstate;
         position = old_position;
         if((r && (index != -1)) || (!r && (index != -2)))
            r = false;
         else
            r = true;
         break;
      }
   default:
   {
      assert(index > 0);
      backup_subex<BidiIterator> sub(*m_presult, index);
      m_presult->set_first(position, index);
      pstate = pstate->next.p;
      r = match_all_states();
      if(r == false)
         sub.restore(*m_presult);
      break;
   }
   }
   return r;
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
         BidiIterator oldposition(position);
         const re_syntax_base* old_pstate = jmp->alt.p;
         pstate = pstate->next.p;
         if(!match_all_states())
         {
            pstate = old_pstate;
            position = oldposition;
         }
         return true;
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
   if(next_count->get_id() != rep->id)
   {
      // we're moving to a different repeat from the last
      // one, so set up a counter object and recurse:
      repeater_count<BidiIterator> r(rep->id, &next_count, position);
      return match_rep();
   }
   //
   // If we've had at least one repeat already, and the last one 
   // matched the NULL string then set the repeat count to
   // maximum:
   //
   next_count->check_null_repeat(position, rep->max);

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

   if(next_count->get_count() < rep->min)
   {
      // we must take the repeat:
      if(take_first)
      {
         // increase the counter:
         ++(*next_count);
         pstate = rep->next.p;
         return match_all_states();
      }
      return false;
   }

   if(rep->greedy)
   {
      // try and take the repeat if we can:
      if((next_count->get_count() < rep->max) && take_first)
      {
         // store position in case we fail:
         BidiIterator pos = position;
         // increase the counter:
         ++(*next_count);
         pstate = rep->next.p;
         if(match_all_states())
            return true;
         // failed repeat, reset posistion and fall through for alternative:
         position = pos;
      }
      if(take_second)
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

}
} // namespace boost

#ifdef __BORLANDC__
#  pragma option pop
#endif

#endif
