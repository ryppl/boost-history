
#ifndef _DATE_TIME_DATE_PARSING_HPP___
#define _DATE_TIME_DATE_PARSING_HPP___
/* Copyright (c) 2002, 2003 CrystalClear Software, Inc.
 * Disclaimer & Full Copyright at end of file
 * Author: Jeff Garland 
 */

#include "boost/tokenizer.hpp"
#include "boost/lexical_cast.hpp"
#include <iterator>

namespace boost {

namespace date_time {

  //! Generic function to parse a delimited date (eg: 2002-02-10)
  /*! Accepted formats are: "2003-02-10" or " 2003-Feb-10" or
   * "2003-Feburary-10" */
  template<class date_type>
  date_type
  parse_date(const std::string& s)
  {
    typedef typename date_type::year_type year_type;
    int pos = 0;
    typename date_type::ymd_type ymd(year_type::min(),1,1);
    boost::tokenizer<boost::char_delimiters_separator<char> > tok(s);
    for(boost::tokenizer<>::iterator beg=tok.begin(); beg!=tok.end();++beg)
    {
      unsigned short i =0; 
      switch(pos) {
        case 0:
	{
          i = boost::lexical_cast<unsigned short>(*beg);
	  ymd.year = i; 
	  break;
	}
        case 1:
	{
	  std::string s = *beg;
	  if((s.at(0) >= '0') && (s.at(0) <= '9'))
          {
            i = boost::lexical_cast<unsigned short>(s);
	  }
	  else
          {
	    for(int j = 1; j <= 12; ++j)
            {
	      typedef gregorian::greg_month::month_enum month_enum;
	      gregorian::greg_month m(static_cast<month_enum>(j));
	      if(s == m.as_long_string() || s == m.as_short_string()) 
              {
		i = static_cast<unsigned short>(j);
		break;
	      }
	    }
	  }
	  ymd.month = i; 
	  break;
	}
        case 2:
        {
          i = boost::lexical_cast<unsigned short>(*beg);
          ymd.day = i; 
          break;
        }
      }; //switch
      pos++;
    }
    return date_type(ymd);
  }

  //! Generic function to parse undelimited date (eg: 20020201)
  template<class date_type>
  date_type
  parse_undelimited_date(const std::string& s)
  {
    int offsets[] = {4,2,2};
    int pos = 0;
    typedef typename date_type::year_type year_type;
    typename date_type::ymd_type ymd(year_type::min(),1,1);
    boost::offset_separator osf(offsets, offsets+3); 
    boost::tokenizer<boost::offset_separator> tok(s, osf);
    for(boost::tokenizer<boost::offset_separator>::iterator ti=tok.begin(); ti!=tok.end();++ti) {
      unsigned short i = boost::lexical_cast<unsigned short>(*ti);
      //      std::cout << i << std::endl;
      switch(pos) {
      case 0: ymd.year = i; break;
      case 1: ymd.month = i; break;
      case 2: ymd.day = i; break;
      };
      pos++;
    } 
    return date_type(ymd);
  }
  
  
  template<class date_type>
  date_type
  parse_date2(const std::string& s)
  {
    //using namespace boost;
    int pos = 0;
    typedef typename date_type::year_type year_type;
    typename date_type::ymd_type ymd(year_type::min(),1,1);
    boost::char_delimiters_separator<char> delim("DT");
    boost::tokenizer<boost::char_delimiters_separator<char> > tok(s);
    for(boost::tokenizer<>::iterator beg=tok.begin(); beg!=tok.end();++beg){
      int i = boost::lexical_cast<int>(*beg);
      switch(pos) {
      case 0: ymd.year = i; break;
      case 1: ymd.month = i; break;
      case 2: ymd.day = i; break;
      };
      pos++;
    }
    return date_type(ymd);
  }


} } //namespace date_time



/* Copyright (c) 2003
 * CrystalClear Software, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  CrystalClear Software makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 */

#endif

