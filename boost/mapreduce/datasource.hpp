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

#include <boost/iostreams/device/mapped_file.hpp>

namespace boost {

namespace mapreduce {

namespace datasource {

namespace detail {

template<typename Key, typename Value>
class file_handler
{
  public:
    file_handler(boost::mapreduce::specification const &spec)
      : specification_(spec), data_(new data)
    {
    }

    bool const get_data(Key const &key, Value &value)   const;
    bool const setup_key(Key &/*key*/)                  const { return false; }

  private:
    boost::mapreduce::specification const &specification_;

    struct data;
    boost::shared_ptr<data> data_;
};

template<>
struct file_handler<
    std::string,
    std::ifstream>::data
{
};

template<>
bool const
file_handler<
    std::string,
    std::ifstream>::get_data(
        std::string const &key,
        std::ifstream &value) const
{
    value.open(key.c_str());
    return value.is_open();
}


template<>
struct file_handler<
    std::string,
    std::pair<
        char const *,
        char const *> >::data
{
    struct detail
    {
        boost::iostreams::mapped_file   mmf;    // memory mapped file
        boost::uintmax_t                size;   // size of the file
        boost::uintmax_t                offset; // offset to map next time
    };

    typedef
    std::map<std::string, boost::shared_ptr<detail> >
    maps_t;

    maps_t       maps;
    boost::mutex mutex;
    std::string  current_file;
};

template<>
bool const
file_handler<
    std::string,
    std::pair<
        char const *,
        char const *> >::get_data(
            std::string const &key,
            std::pair<char const *, char const *> &value) const
{
    // we need to hold the lock for the duration of this function
    boost::mutex::scoped_lock l(data_->mutex);
    data::maps_t::iterator it;
    if (data_->current_file.empty())
    {
        data_->current_file = key;
        it = data_->maps.insert(std::make_pair(key, boost::shared_ptr<data::detail>(new data::detail))).first;
        it->second->mmf.open(key, BOOST_IOS::in);
        if (!it->second->mmf.is_open())
        {
            std::cout << "\nFailed to map file into memory: " << key;
            return false;
        }

        it->second->size   = boost::filesystem::file_size(key);
        it->second->offset = std::min(specification_.max_file_segment_size, it->second->size);
        value.first        = it->second->mmf.const_data();
        value.second       = value.first + it->second->offset;
    }
    else
    {
        BOOST_ASSERT(key == data_->current_file);
        it = data_->maps.find(key);
        BOOST_ASSERT(it != data_->maps.end());

        value.first        = it->second->mmf.const_data() + it->second->offset;
        it->second->offset = std::min(it->second->offset+specification_.max_file_segment_size, it->second->size);
        value.second       = it->second->mmf.const_data() + it->second->offset;
    }

    if (it->second->offset == it->second->size)
        data_->current_file.clear();

    // break on a line boundary
    while (*value.second != '\n'  &&  *value.second != '\r'  &&  it->second->offset != it->second->size)
    {
        ++value.second;
        ++it->second->offset;
    }

    return true;
}

template<>
bool const
file_handler<
    std::string,
    std::pair<
        char const *,
        char const *> >::setup_key(std::string &key) const
{
    boost::mutex::scoped_lock l(data_->mutex);
    if (data_->current_file.empty())
        return false;
    key = data_->current_file;
    return true;
}

}   // namespace detail

template<
    typename MapTask,
    typename FileHandler = detail::file_handler<
        typename MapTask::key_type,
        typename MapTask::value_type> >
class directory_iterator : boost::noncopyable
{
  public:
    directory_iterator(boost::mapreduce::specification const &spec)
      : specification_(spec),
        file_handler_(spec)
    {
        it_dir_ = boost::filesystem::basic_directory_iterator<path_t>(specification_.input_directory);
    }

    bool const setup_key(typename MapTask::key_type &key) const
    {
        if (!file_handler_.setup_key(key))
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
        }
        return true;
    }

    bool const get_data(typename MapTask::key_type &key, typename MapTask::value_type &value) const
    {
        return file_handler_.get_data(key, value);
    }

  private:
    typedef
    boost::filesystem::basic_path<std::string, boost::filesystem::path_traits>
    path_t;

    mutable boost::filesystem::basic_directory_iterator<path_t> it_dir_;
    std::string                            directory_;
    boost::mapreduce::specification const &specification_;
    FileHandler                            file_handler_;
};

}   // namespace datasource

}   // namespace mapreduce 

}   // namespace boost

#endif  // BOOST_DATASOURCE_SCHEDULE_POLICY_HPP
