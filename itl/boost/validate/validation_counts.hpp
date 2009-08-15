/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef __itl_validation_counts_hpp_JOFA_090713__
#define __itl_validation_counts_hpp_JOFA_090713__


namespace boost{namespace itl
{

class validation_counts
{
public:
    validation_counts():_time(0.0), _count(0){}
    validation_counts(double sum): _time(sum),_count(1){}

    double time()const     {return _time;}
    long   count()const    {return _count;}
    double average()const{ return _count==0 ? 0.0 : _time/_count; }

    validation_counts& operator += (const validation_counts& right)
    { 
        _time += right.time(); 
        _count += right.count(); 
        return *this; 
    }

private:
    double _time;
    long   _count;
};

bool operator == (const validation_counts& left, const validation_counts& right)
{ return left.time()==right.time() && left.count()==right.count(); } 


}} // namespace itl boost

#endif // __itl_validation_counts_hpp_JOFA_090713__
