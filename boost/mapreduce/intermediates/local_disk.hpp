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

#include <iomanip>                  // setw
#include <fstream>		// linux
#include <boost/unordered_map.hpp>
#include <boost/mapreduce/hash_partitioner.hpp>

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

template<
    typename MapTask,
    typename SortFn=mapreduce::detail::file_sorter,
    typename MergeFn=mapreduce::detail::file_merger,
    typename PartitionFn=mapreduce::hash_partitioner>
class local_disk : boost::noncopyable
{
  private:
    typedef
    boost::unordered_map<
        size_t,                                     // hash value of intermediate key (R)
        std::pair<
            std::string,                            // filename
            boost::shared_ptr<std::ofstream> > >    // file stream
    intermediates_t;

  public:
    typedef MapTask map_task_type;
    typedef typename intermediates_t::const_iterator const_iterator;

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
            for (intermediates_t::iterator it=intermediates_.begin(); it!=intermediates_.end(); ++it)
                boost::filesystem::remove(it->second.first);
        }
        catch (std::exception const &/*e*/)
        {
//            std::cerr << "\nError: " << e.what() << "\n";
        }
    }

    bool const insert(typename map_task_type::intermediate_key_type   const &key,
                      typename map_task_type::intermediate_value_type const &value)
    {
        unsigned const partition = partitioner_(key, num_partitions_);
        intermediates_t::iterator it = intermediates_.insert(make_pair(partition, intermediates_t::mapped_type())).first;
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

    bool const get_partition_filename(size_t const partition, std::string &filename) const
    {
        intermediates_t::const_iterator it=completed_intermediates_.find(partition);
        if (it == completed_intermediates_.end())
            return false;
        filename = it->second.first;
        return true;
    }

    template<typename FnObj>
    void combine(FnObj &fn_obj)
    {
        this->close_files();
        for (intermediates_t::iterator it=intermediates_.begin(); it!=intermediates_.end(); ++it)
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

        assert(completed_intermediates_.size() == 0);
        std::swap(completed_intermediates_, intermediates_);
    }

    void combine(mapreduce::null_combiner &/*fn_obj*/)
    {
        this->close_files();
        assert(completed_intermediates_.size() == 0);
        std::swap(completed_intermediates_, intermediates_);
    }

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
        for (intermediates_t::iterator it=intermediates_.begin(); it!=intermediates_.end(); ++it)
            if (it->second.second  &&  it->second.second->is_open())
                it->second.second->close();
    }

  private:
    unsigned const  num_partitions_;
    intermediates_t intermediates_;
    intermediates_t completed_intermediates_;
    PartitionFn     partitioner_;
};

}   // namespace intermediates

}   // namespace mapreduce

}   // namespace boost

#endif  // BOOST_MAPREDUCE_LOCAL_DISK_INTERMEDIATES_HPP
