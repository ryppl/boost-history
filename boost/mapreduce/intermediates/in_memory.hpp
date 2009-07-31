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
 
#ifndef BOOST_MAPREDUCE_IN_MEMORY_INTERMEDIATES_HPP
#define BOOST_MAPREDUCE_IN_MEMORY_INTERMEDIATES_HPP

namespace boost {

namespace mapreduce {

namespace intermediates {

template<typename MapTask, typename ReduceTask>
class reduce_null_output
{
  public:
    reduce_null_output(std::string const &/*output_filespec*/,
                       unsigned    const  /*partition*/,
                       unsigned    const  /*num_partitions*/)
    {
    }

    void operator()(typename MapTask::intermediate_key_type const &/*key*/,
                    typename ReduceTask::value_type         const &/*value*/)
    {
    }
};


template<
    typename MapTask,
    typename ReduceTask,
    typename PartitionFn=mapreduce::hash_partitioner>
class in_memory
{
  private:
    typedef
    std::vector<
        std::map<
            typename MapTask::intermediate_key_type,
            std::list<typename MapTask::intermediate_value_type> > >
    intermediates_t;

  public:
    typedef MapTask map_task_type;
    typedef reduce_null_output<MapTask, ReduceTask> store_result_type;

    in_memory(unsigned const num_partitions)
      : num_partitions_(num_partitions)
    {
        intermediates_.resize(num_partitions_);
    }

    template<typename Callback>
    void reduce(unsigned const partition, Callback &callback, results &result)
    {
        intermediates_t::value_type &map = intermediates_[partition];
        for (typename intermediates_t::value_type::const_iterator it1=map.begin(); it1!=map.end(); ++it1)
        {
            callback(it1->first, it1->second.begin(), it1->second.end());
            ++result.counters.reduce_keys_executed;
        }
    }

    void merge_from(unsigned partition, in_memory &other)
    {
        BOOST_ASSERT(intermediates_.find(partition) != intermediates_.end());
        BOOST_ASSERT(other.intermediates_.find(partition) != other.intermediates_.end());

        typedef intermediates_t::value_type map_type;

        map_type &map       = intermediates_[partition];
        map_type &other_map = other.intermediates_[partition];

        if (map.size() == 0)
        {
            std::swap(map, other_map);
            return;
        }

        for (map_type::iterator it=other_map.begin(); it!=other_map.end(); ++it)
        {
            map_type::iterator iti = map.insert(make_pair(it->first, map_type::mapped_type())).first;
            std::copy(it->second.begin(), it->second.end(), std::back_inserter(iti->second));
        }
    }

    void merge_from(in_memory &other)
    {
        for (unsigned partition=0; partition<num_partitions_; ++partition)
            merge_from(partition, other);
        other.intermediates_.clear();
    }


    bool const insert(typename map_task_type::intermediate_key_type   const &key,
                      typename map_task_type::intermediate_value_type const &value)
    {
        intermediates_t::value_type &map = intermediates_[partitioner_(key, num_partitions_)];

        map.insert(
            make_pair(
                key,
                intermediates_t::value_type::mapped_type())).first->second.push_back(value);

        return true;
    }

//!!!
    //bool const replace(typename map_task_type::intermediate_key_type   const &key,
    //                   typename map_task_type::intermediate_value_type const &value)
    //{
    //    intermediates_t::iterator it = intermediates_.find(key);
    //    if (it == intermediates_.end())
    //        return false;
    //    it->second.clear();
    //    it->second.push_back(value);
    //    return true;
    //}

    template<typename FnObj>
    void combine(FnObj &fn_obj)
    {
        intermediates_t intermediates;
        std::swap(intermediates_, intermediates);

        for (intermediates_t::const_iterator it=intermediates.begin(); it!=intermediates.end(); ++it)
        {
            for (intermediates_t::value_type::const_iterator it1=it->begin(); it1!=it->end(); ++it1)
            {
                fn_obj.start(it1->first);
                std::for_each<
                    intermediates_t::value_type::mapped_type::const_iterator, FnObj &>(
                        it1->second.begin(),
                        it1->second.end(),
                        fn_obj);
                fn_obj.finish(it1->first, *this);
            }
        }

        if (intermediates_.size() == 0)
            std::swap(intermediates_, intermediates);
    }

  private:
    unsigned const  num_partitions_;
    intermediates_t intermediates_;
    PartitionFn     partitioner_;
};


}   // namespace intermediates

}   // namespace mapreduce

}   // namespace boost

#endif  // BOOST_MAPREDUCE_IN_MEMORY_INTERMEDIATES_HPP
