//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACTIVITY_DETAIL_ACTIVITY_DATA_HPP
#define BOOST_ACTIVITY_DETAIL_ACTIVITY_DATA_HPP

#include <windows.h> // ToDo: Remove, change HANDLE to void*

#include <boost/optional.hpp>

namespace boost
{
  namespace detail
  {
    class activity_data_base
      : ::boost::noncopyable
    {
    public:
      activity_data_base()
        : event_handle_m( CreateEvent( NULL, TRUE, FALSE, NULL ) )
      {
        // ToDo: Check return value
      }

      ~activity_data_base()
      {
        // ToDo: Possibly handle return value
        CloseHandle( event_handle_m );
      }
    public:
      HANDLE event_handle() const
      {
        return event_handle_m;
      }
    private:
      HANDLE event_handle_m;
    };

    template< typename ContainedType >
    class activity_data
      : activity_data_base
    {
    public:
      using activity_data_base::event_handle;
      ::boost::optional< ContainedType > result;
      // ToDo: Create queue
    };

    template<>
    class activity_data< void >
      : activity_data_base
    {
    public:
      using activity_data_base::event_handle;
    };
  }
}

#endif
