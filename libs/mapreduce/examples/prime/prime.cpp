// Based on code from Christian Henning [chhenning@gmail.com]
#define _SECURE_SCL 0
#define _HAS_ITERATOR_DEBUGGING 0
#include <boost/mapreduce.hpp>

namespace prime_calculator {

bool const is_prime(long const number)
{
    if (number == 0 || number == 1)
        return false;
    else if (number == 2)
        return true;
    else if (number%2 == 0)
        return false;

    long n = std::abs(number);
    long const sqrt_number = static_cast<long>(std::sqrt(static_cast<double>(n)));

    for (long i = 3; i < sqrt_number; i+=2)
    {
        if (n % i == 0)
            return false;
    }

    return true;
}

template<typename MapTask>
class number_source : boost::noncopyable
{
  public:
    number_source(long first, long last, long step)
      : current_(first), last_(last), step_(step)
    {
    }

    const bool setup_key(typename MapTask::key_type &key)
    {
        if (current_ > last_)
            return false;

        key = current_;
        current_ = std::min(current_ + step_, last_+1);
        return true;
    }

    const bool get_data(typename MapTask::key_type &key, typename MapTask::value_type &value) const
    {
        value = std::min(key + step_ - 1, last_);
        return true;
    }

  private:
    long step_;
    long last_;
    long current_;
};

struct map_task : public boost::mapreduce::map_task<long, long>
{
    template<typename Runtime>
    void operator()(Runtime &runtime, key_type const &key, value_type const &value) const
    {
        BOOST_STATIC_ASSERT((boost::is_same<key_type, value_type>::value));
        for (typename key_type loop=key; loop<=value; ++loop)
            runtime.emit_intermediate(is_prime(loop), loop);
    }
};

struct reduce_task : public boost::mapreduce::reduce_task<bool, long>
{
    template<typename Runtime, typename It>
    void operator()(Runtime &runtime, key_type const &key, It it, It ite) const
    {
        if (key)
            for_each(it, ite, boost::bind(&Runtime::emit, &runtime, true, _1));
    }
};

typedef
boost::mapreduce::job<prime_calculator::map_task
                     , prime_calculator::reduce_task
                     , boost::mapreduce::null_combiner
                     , prime_calculator::number_source<prime_calculator::map_task>
> job;

} // namespace prime_calculator

int main(int argc, char* argv[])
{
    boost::mapreduce::specification spec;
    spec.map_tasks    = 0;
    spec.reduce_tasks = std::max(1U, boost::thread::hardware_concurrency());

    int const prime_limit = 100;
    prime_calculator::job::datasource_type datasource(0, prime_limit, prime_limit/spec.reduce_tasks);


    std::cout <<"\nRunning Parallel Prime_Calculator MapReduce..." <<std::endl;
    prime_calculator::job job(datasource, spec);
    boost::mapreduce::results result;
#ifdef _DEBUG
    job.run<boost::mapreduce::schedule_policy::sequential<prime_calculator::job> >(result);
#else
    job.run<boost::mapreduce::schedule_policy::cpu_parallel<prime_calculator::job> >(result);
#endif
    std::cout <<"\nMapReduce Finished." <<std::endl;

    for (prime_calculator::job::const_result_iterator it = job.begin_results();
         it!=job.end_results();
         ++it)
    {
        std::cout <<it->second <<" ";
    }

	return 0;
}
