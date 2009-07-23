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

#ifndef BOOST_MAPREDUCE_JOB_HPP
#define BOOST_MAPREDUCE_JOB_HPP
 
namespace boost {

namespace mapreduce {

template<typename T> size_t length(T const &str);

template<typename MapTask,
         typename ReduceTask,
         typename Combiner=null_combiner,
         typename Datasource=datasource::directory_iterator<MapTask>,
         typename IntermediateStore=intermediates::local_disk<MapTask> >
class job : private boost::noncopyable
{
  public:
    typedef MapTask                 map_task_type;
    typedef ReduceTask              reduce_task_type;
    typedef Datasource              datasource_type;
    typedef IntermediateStore       intermediate_store_type;
    typedef Combiner                combiner_type;
    typedef std::list<std::string>  filenames_t;

  public:
    class map_task_runner : boost::noncopyable
    {
      public:
        map_task_runner(job &j)
          : job_(j),
            intermediate_store_(job_.number_of_partitions())
        {
        }

        // 'value' parameter is not a reference to const to enable streams to be passed
        map_task_runner &operator()(typename map_task_type::key_type const &key, typename map_task_type::value_type &value)
        {
            map_task_type(*this)(key, value);

            // consolidating map intermediate results can save network time by
            // aggregating the mapped valued at mapper
            combiner_type combiner;
            intermediate_store_.combine(combiner);

            // notify the job of the final partition files
            notify_runtime_partition_files();

            return *this;
        }

        bool const emit_intermediate(typename map_task_type::intermediate_key_type   const &key,
                                     typename map_task_type::intermediate_value_type const &value)
        {
            return intermediate_store_.insert(key, value);
        }

    protected:
        void notify_runtime_partition_files(void) const
        {
            for (unsigned partition=0; partition<job_.number_of_partitions(); ++partition)
            {
                std::string filename;
                if (intermediate_store_.get_partition_filename(partition, filename))
                {
                    job_.new_partition_file(partition, filename);
                }
            }
        }

      private:
        job                     &job_;
        intermediate_store_type  intermediate_store_;
    };

    class reduce_task_runner : boost::noncopyable
    {
      public:
        reduce_task_runner(std::string const &output_filespec, size_t const partition, size_t const num_partitions)
        {
            std::ostringstream filename;
            filename << output_filespec << partition+1 << "_of_" << num_partitions;
            filename_ = filename.str();
            output_file_.open(filename_.c_str());
        }

        void emit(typename map_task_type::intermediate_key_type const &key,
                  typename reduce_task_type::value_type         const &value)
        {
            output_file_ << key << "\t" << value << "\n";
        }

        template<typename It>
        void operator()(typename map_task_type::intermediate_key_type const &key, It it, It ite)
        {
            reduce_task_type reduce_task(*this);
            reduce_task(key, it, ite);
        }

        std::string const &filename(void) const
        {
            return filename_;
        }

      private:
        std::string   filename_;
        std::ofstream output_file_;
    };

    job(datasource_type &datasource)
      : num_partitions_(0),
        datasource_(datasource),
        output_filespec_(".\\mapreduce_")
     {
     }

    ~job()
    {
        try
        {
            for (partition_files_t::iterator itp=partition_files_.begin(); itp!=partition_files_.end(); ++itp)
                std::for_each(itp->second.begin(), itp->second.end(), boost::bind(detail::delete_file, _1));
        }
        catch (std::exception const &e)
        {
            std::cerr << "\nError: " << e.what() << "\n";
        }
        partition_files_.clear();
    }

    void set_output_filespec(std::string const &output_filespec)
    {
        output_filespec_ = output_filespec;
    }

    bool const get_next_map_key(void *&key)
    {
        std::auto_ptr<typename map_task_type::key_type> next_key(new typename map_task_type::key_type);
        if (!datasource_.setup_key(*next_key))
            return false;
        key = next_key.release();
        return true;
    }

    void new_partition_file(unsigned const partition, std::string const &filename)
    {
        partition_files_.insert(std::make_pair(partition, partition_files_t::mapped_type())).first->second.push_back(filename);
    }

    template<typename SchedulePolicy>
    void run(specification const &spec, results &result)
    {
        SchedulePolicy schedule;
        this->run(schedule, spec, result);
    }

    template<typename SchedulePolicy>
    void run(SchedulePolicy &schedule, specification const &spec, results &result)
    {
        time_t const start_time = time(NULL);
        schedule(*this, spec, result);
        result.job_runtime = time(NULL) - start_time;
    }

    bool const run_map_task(void *key, results &result)
    {
        time_t const start_time = time(NULL);

        ++result.counters.map_tasks;

        std::auto_ptr<typename map_task_type::key_type> map_key_ptr(reinterpret_cast<typename map_task_type::key_type *>(key));
        typename map_task_type::key_type &map_key = *map_key_ptr;

        // get the data
        typename map_task_type::value_type value;
        if (!datasource_.get_data(map_key, value))
        {
            ++result.counters.map_tasks_error;
            return false;
        }

        // Map Task
        if (map_key == typename map_task_type::key_type()  ||  value == typename map_task_type::value_type())
        {
            BOOST_ASSERT(map_key != typename map_task_type::key_type());
            BOOST_ASSERT(value != typename map_task_type::value_type());
            ++result.counters.map_tasks_error;
            return false;
        }
        map_task_runner runner(*this);
        runner(map_key, value);

        result.map_times.push_back(time(NULL)-start_time);

        ++result.counters.map_tasks_completed;
        return true;
    }

    unsigned const number_of_partitions(void) const
    {
        return num_partitions_;
    }

    void number_of_partitions(unsigned const partitions)
    {
        num_partitions_ = partitions;
    }

    // the caller must synchronise calls to this function from multiple threads
    bool const get_partition_filenames(unsigned &partition, filenames_t &filenames)
    {
        partition_files_t::iterator itp = partition_files_.find(partition);
        if (itp == partition_files_.end())
            return false;
        else if (itp->second.size() == 0)
            return get_partition_filenames(++partition, filenames);

        std::swap(itp->second, filenames);
        return true;
    }

    bool const run_reduce_task(unsigned const partition, filenames_t const &filenames, results &result)
    {
        time_t const start_time = time(NULL);
        std::string const filename = intermediate_store_type::merge_and_sort(filenames);

        reduce_task_runner runner(output_filespec_, partition, num_partitions_);

        typename map_task_type::intermediate_key_type   key;
        typename map_task_type::intermediate_key_type   last_key;
        typename map_task_type::intermediate_value_type value;
        std::list<typename map_task_type::intermediate_value_type> values;

        std::ifstream infile(filename.c_str());
        while (intermediate_store_type::read_record(infile, key, value))
        {
            if (key != last_key  &&  length(key) > 0)
            {
                if (length(last_key) > 0)
                {
                    runner(last_key, values.begin(), values.end());
                    values.clear();
                }
                if (length(key) > 0)
                    std::swap(key, last_key);
            }

            values.push_back(value);
        }

        if (length(last_key) > 0)
            runner(last_key, values.begin(), values.end());

        infile.close();
        boost::filesystem::remove(filename.c_str());

        result.reduce_times.push_back(time(NULL)-start_time);

        return true;
    }

  private:
    typedef
    boost::unordered_map<
        unsigned,       // partition
        filenames_t>    // file names
    partition_files_t;

    unsigned            num_partitions_;
    datasource_type    &datasource_;
    std::string         output_filespec_;
    partition_files_t   partition_files_;
};

template<>
inline size_t length(std::string const &str)
{
    return str.length();
}

}   // namespace mapreduce

}   // namespace boost

#endif  // BOOST_MAPREDUCE_JOB_HPP
