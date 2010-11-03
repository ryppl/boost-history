/*==============================================================================
    Copyright (c) 2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <iostream>
#include <fstream>
#include <map>
#include <string>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/time_facet.hpp>

int
main(int argc, char *argv[])
{
    if(argc<=1)
    {
        std::cout << "Pass at least one argument..." << std::endl;
        return -1;
    }

    typedef
        std::map<
            std::string
          , std::multimap<boost::posix_time::time_period, std::string>
        >
    timings_type;
    timings_type timings;

    for(int i=1;i!=argc;++i)
    {

        std::ifstream timing(argv[i]);
        timing.imbue(std::locale(
            timing.getloc(),
            new boost::posix_time::time_input_facet("%Y-%m-%d %H:%M:%S")));

        boost::posix_time::ptime start;
        boost::posix_time::ptime end;

        {
            std::string line;

            timing >> start;
            std::getline(timing, line);

            timing >> end;
            std::getline(timing, line);
        }

        std::string category;
        std::getline(timing, category);

        std::string desc;
        std::getline(timing, desc);
        if(!timing)
        {
            std::cout << "Failed parsing " << argv[i] << std::endl;
            return -1;
        }

        timings[category].insert(std::make_pair(
            boost::posix_time::time_period(start, end),desc));
    }

    for(timings_type::const_iterator p=timings.begin();p!=timings.end();++p)
    {
        if(p!=timings.begin())
        {
            std::cout << std::endl;
        }

        std::cout << p->first << "..." << std::endl;
        for(timings_type::mapped_type::const_iterator q=p->second.begin();
            q!=p->second.end();
            ++q)
        {
            std::cout << "\t" << q->first.length() <<
                "  ..." << q->second << std::endl;
        }
    }
}
