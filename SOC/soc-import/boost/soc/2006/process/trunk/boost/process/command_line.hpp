//
// Boost.Process
// Command line representation.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//
 
#if !defined(BOOST_PROCESS_COMMAND_LINE_HPP)
#define BOOST_PROCESS_COMMAND_LINE_HPP

#include <string>
#include <vector>

namespace boost {
    namespace process {

        class command_line {
        public:
            typedef std::vector< std::string > arguments_vector;

        private:
            std::string m_executable;
            arguments_vector m_arguments;

        public:
            explicit
            command_line(const std::string& executable) :
                m_executable(executable)
            {
            }

            command_line&
            argument(const std::string& argument)
            {
                m_arguments.push_back(argument);
                return *this;
            }

            const arguments_vector&
            get_arguments(void)
                const
            {
                return m_arguments;
            }

            const std::string&
            get_executable(void)
                const
            {
                return m_executable;
            }
        };

    } // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_COMMAND_LINE_HPP)
