// Boost.MapReduce library
//
//  Copyright (C) 2009 Craig Henderson.
//  cdm.henderson@googlemail.com
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/mapreduce/
//

#ifndef BOOST_DATASOURCE_SCHEDULE_POLICY_HPP
#define BOOST_DATASOURCE_SCHEDULE_POLICY_HPP

namespace boost {

namespace mapreduce {

namespace datasource {

template<typename MapTask>
class directory_iterator : boost::noncopyable
{
  public:
    bool const setup_key(typename MapTask::key_type &key) const
    {
        while (it_dir_ != boost::filesystem::basic_directory_iterator<path_t>()
            && boost::filesystem::is_directory(*it_dir_))
        {
            ++it_dir_;
        }

        if (it_dir_ == boost::filesystem::basic_directory_iterator<path_t>())
            return false;

        path_t path = *it_dir_++;
        key = path.external_file_string();
        return true;
    }

    bool const get_data(typename MapTask::key_type &key, typename MapTask::value_type &istream) const
    {
        istream.open(key.c_str());
        return istream.is_open();
    }

    void set_directory(char const *directory)
    {
        directory_ = directory;
        it_dir_ = boost::filesystem::basic_directory_iterator<path_t>(directory_);
    }

  private:
    typedef
    boost::filesystem::basic_path<std::string, boost::filesystem::path_traits>
    path_t;

    mutable boost::filesystem::basic_directory_iterator<path_t> it_dir_;
    std::string   directory_;
};

}   // namespace datasource

}   // namespace mapreduce 

}   // namespace boost

#endif  // BOOST_DATASOURCE_SCHEDULE_POLICY_HPP
