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
 
#ifndef BOOST_MAPREDUCE_LOCAL_DISK_INTERMEDIATES_HPP
#define BOOST_MAPREDUCE_LOCAL_DISK_INTERMEDIATES_HPP

#include <iomanip>      // setw
#ifdef __GNUC__
#include <iostream>     // ubuntu linux
#include <fstream>      // ubuntu linux
#endif

namespace boost {

namespace mapreduce {

struct null_combiner;

namespace detail {

struct file_merger
{
    template<typename List>
    void operator()(List const &filenames, char const *dest)
    {
        buffer_.reset(new char[buffer_size_]);

        std::ofstream outfile(dest, std::ios_base::in | std::ios_base::binary);
        for (typename List::const_iterator it=filenames.begin(); it!=filenames.end(); ++it)
            copy_file(*it, outfile);
    }

  private:
    void copy_file(std::string const &filename, std::ofstream &outfile)
    {
        boost::uint64_t remaining = boost::filesystem::file_size(filename);

        std::ifstream infile(filename.c_str(), std::ios_base::in | std::ios_base::binary);
        if (!infile.is_open())
        {
            std::ostringstream err;
            err << "Failed to open file: " << filename;
            BOOST_THROW_EXCEPTION(std::runtime_error(err.str()));
        }

        std::streamsize read_write_size = buffer_size_;
        while (remaining > 0)
        {
            if (remaining < (boost::uint64_t)read_write_size)
                read_write_size = (std::streamsize)remaining;

            infile.read(buffer_.get(), read_write_size);
            if (infile.bad()  ||  infile.fail())
            {
                std::ostringstream err;
                err << "Error reading file " << filename;
                BOOST_THROW_EXCEPTION(std::runtime_error(err.str()));
            }

            outfile.write(buffer_.get(), read_write_size);
            if (outfile.bad()  ||  outfile.fail())
            {
                std::ostringstream err;
                err << "Error writing to file";
                BOOST_THROW_EXCEPTION(std::runtime_error(err.str()));
            }

            remaining -= read_write_size;
        }
    }

    std::auto_ptr<char> buffer_;
    static unsigned const buffer_size_ = 1048576;
};

struct file_sorter
{
    bool const operator()(char const *in, char const *out, unsigned const offset) const
    {
        return mapreduce::merge_sort(in, out, offset);
    }
};

}   // namespace detail

namespace intermediates {

template<typename MapTask, typename ReduceTask>
class reduce_file_output
{
  public:
    reduce_file_output(std::string const &output_filespec,
                       unsigned    const  partition,
                       unsigned    const  num_partitions)
    {
        std::ostringstream filename;
        filename << output_filespec << partition+1 << "_of_" << num_partitions;
        filename_ = filename.str();
        output_file_.open(filename_.c_str());
    }

    void operator()(typename MapTask::intermediate_key_type const &key,
                    typename ReduceTask::value_type         const &value)
    {
        output_file_ << key << "\t" << value << "\n";
    }

  private:
    std::string   filename_;
    std::ofstream output_file_;
};


template<
    typename MapTask,
    typename ReduceTask,
    typename SortFn=mapreduce::detail::file_sorter,
    typename MergeFn=mapreduce::detail::file_merger,
    typename PartitionFn=mapreduce::hash_partitioner>
class local_disk : boost::noncopyable
{
  private:
    typedef
    std::map<
        size_t,                                     // hash value of intermediate key (R)
        std::pair<
            std::string,                            // filename
            boost::shared_ptr<std::ofstream> > >    // file stream
    intermediates_t;

  public:
    typedef MapTask map_task_type;
    typedef reduce_file_output<MapTask, ReduceTask> store_result_type;

    local_disk(unsigned const num_partitions)
      : num_partitions_(num_partitions)
    {
    }

    ~local_disk()
    {
        try
        {
            this->close_files();

            // delete the temporary files
            for (intermediates_t::iterator it=intermediate_files_.begin(); it!=intermediate_files_.end(); ++it)
                boost::filesystem::remove(it->second.first);
        }
        catch (std::exception const &e)
        {
            std::cerr << "\nError: " << e.what() << "\n";
        }
    }

    bool const insert(typename map_task_type::intermediate_key_type   const &key,
                      typename map_task_type::intermediate_value_type const &value)
    {
        unsigned const partition = partitioner_(key, num_partitions_);

        intermediates_t::iterator it =
            intermediate_files_.insert(
                make_pair(
                    partition,
                    intermediates_t::mapped_type())).first;

        if (it->second.first.empty())
        {
            it->second.first = platform::get_temporary_filename();
            it->second.second.reset(new std::ofstream);
        }

        if (!it->second.second->is_open())
            it->second.second->open(it->second.first.c_str(), std::ios_base::out | std::ios_base::app);
        assert(it->second.second->is_open());

        std::ostringstream key_text_stream;
        key_text_stream << key;
        std::string key_text(key_text_stream.str());
        *it->second.second << std::setw(10) << key_text.length() << "\t" << key << "\t" << value << "\n";
        return !(it->second.second->bad()  ||  it->second.second->fail());
    }

    template<typename FnObj>
    void combine(FnObj &fn_obj)
    {
        this->close_files();
        for (intermediates_t::iterator it=intermediate_files_.begin(); it!=intermediate_files_.end(); ++it)
        {
            std::string infilename  = it->second.first;
            std::string outfilename = platform::get_temporary_filename();

            // sort the input file
            SortFn()(infilename.c_str(), outfilename.c_str(), 11);
            boost::filesystem::remove(infilename);
            std::swap(infilename, outfilename);

            std::string key, last_key;
            std::ifstream infile(infilename.c_str());
            while (!infile.eof())
            {
                typename map_task_type::intermediate_value_type value;
                if (read_record(infile, key, value))
                {
                    if (key != last_key  &&  key.length() > 0)
                    {
                        if (last_key.length() > 0)
                            fn_obj.finish(last_key, *this);
                        if (key.length() > 0)
                        {
                            fn_obj.start(key);
                            std::swap(key, last_key);
                        }
                    }

                    fn_obj(value);
                }
            }

            if (last_key.length() > 0)
                fn_obj.finish(last_key, *this);

            infile.close();

            boost::filesystem::remove(infilename);
        }

        this->close_files();
    }

    void combine(mapreduce::null_combiner &/*fn_obj*/)
    {
        this->close_files();
    }

    void merge_from(local_disk &other)
    {
        BOOST_ASSERT(num_partitions_ == other.num_partitions_);

        for (unsigned partition=0; partition<num_partitions_; ++partition)
        {
            intermediates_t::iterator ito = other.intermediate_files_.find(partition);
            BOOST_ASSERT(ito != other.intermediate_files_.end());

            intermediates_t::iterator it = intermediate_files_.find(partition);
            if (it == intermediate_files_.end())
            {
                intermediate_files_.insert(
                    make_pair(
                        partition,
                        std::make_pair(
                            ito->second.first,
                            boost::shared_ptr<std::ofstream>(new std::ofstream))));
            }
            else
            {
                std::list<std::string> filenames;
                filenames.push_back(it->second.first);
                filenames.push_back(ito->second.first);
                it->second.first = merge_and_sort(filenames);
                it->second.second->close();
            }
            other.intermediate_files_.erase(partition);
        }
    }

    template<typename Callback>
    void reduce(unsigned const partition, Callback &callback, results &result)
    {
        typename map_task_type::intermediate_key_type   key;
        typename map_task_type::intermediate_key_type   last_key;
        typename map_task_type::intermediate_value_type value;
        std::list<typename map_task_type::intermediate_value_type> values;

        std::list<std::string> filenames;
        intermediates_t::const_iterator it = intermediate_files_.find(partition);
        BOOST_ASSERT(it != intermediate_files_.end());

        std::string const &filename = it->second.first;
        std::ifstream infile(filename.c_str());
        while (read_record(infile, key, value))
        {
            if (key != last_key  &&  length(key) > 0)
            {
                if (length(last_key) > 0)
                {
                    ++result.counters.reduce_keys_executed;
                    callback(last_key, values.begin(), values.end());
                    values.clear();
                }
                if (length(key) > 0)
                    std::swap(key, last_key);
            }

            values.push_back(value);
        }

        if (length(last_key) > 0)
        {
            ++result.counters.reduce_keys_executed;
            callback(last_key, values.begin(), values.end());
        }

        infile.close();
        boost::filesystem::remove(filename.c_str());
    }

  protected:
    static bool const read_record(std::ifstream &infile,
                                  typename map_task_type::intermediate_key_type   &key,
                                  typename map_task_type::intermediate_value_type &value)
    {
#if defined(__SGI_STL_PORT)
        size_t keylen;
#else
        std::streamsize keylen;
#endif
        keylen = 0;
        infile >> keylen;
        if (infile.eof()  ||  keylen == 0)
            return false;

        char tab;
        infile.read(&tab, 1);

        key.resize(keylen);
        infile.read(&*key.begin(), keylen);
        infile.read(&tab, 1);
        infile >> value;
        return true;
    }

    template<typename List>
    static std::string merge_and_sort(List const &filenames)
    {
        assert(filenames.size() > 0);

        // first merge
        std::string const temp = platform::get_temporary_filename();
        MergeFn()(filenames, temp.c_str());

        // then sort
        typename List::const_iterator it=filenames.begin();
        SortFn()(temp.c_str(), it->c_str(), 11);

        // delete merge source files
        boost::filesystem::remove(temp);
        std::for_each(++it, filenames.end(), boost::bind(detail::delete_file, _1));

        // return the resulting filename
        return *filenames.begin();
    }

  private:
    void close_files(void)
    {
        for (intermediates_t::iterator it=intermediate_files_.begin(); it!=intermediate_files_.end(); ++it)
            if (it->second.second  &&  it->second.second->is_open())
                it->second.second->close();
    }

  private:
    unsigned const  num_partitions_;
    intermediates_t intermediate_files_;
    PartitionFn     partitioner_;
};

}   // namespace intermediates

}   // namespace mapreduce

}   // namespace boost

#endif  // BOOST_MAPREDUCE_LOCAL_DISK_INTERMEDIATES_HPP
