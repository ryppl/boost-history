#ifndef SIGNAL_NETWORK_TIMED_GENERATOR_HPP
#define SIGNAL_NETWORK_TIMED_GENERATOR_HPP

#include <boost/dataflow/signal/component/storage.hpp>

#include <boost/thread/thread.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/xtime.hpp>

namespace boost { namespace signals {

/** \brief Creates its own thread and periodically sends a signal with the stored value.
	\param Signature signature of the sent signal.
*/
template<typename Signature,
    typename OutSignal=SIGNAL_NETWORK_DEFAULT_OUT,
    typename Combiner = boost::last_value<typename boost::function_traits<Signature>::result_type>,
    typename Group = int,
    typename GroupCompare = std::less<Group>
    >
class timed_generator : public storage<Signature, OutSignal, Combiner, Group, GroupCompare>
{
public:    
	/// Default constructor.  Starts the thread, but signals won't be sent until the enable() function is called.
	timed_generator() : terminating(false), enabled(false)
	{
		thread_object = new boost::thread(boost::bind(&timed_generator::thread_function, boost::ref(*this)));
	}
	/// Sets the object to send the stored value at specified time intervals.
	/** \param[in] interval Sets the time interval (in seconds) at which the signal is sent.
		\param[in] signal_count The signal will be sent signal_count times, or indefinitelly if signal_count==0.
	*/
	void enable(double interval, unsigned signal_count=0)
	{
		xinterval.sec = (boost::xtime::xtime_sec_t)interval;
		xinterval.nsec = boost::xtime::xtime_nsec_t((interval - xinterval.sec) / 0.000000001);

		boost::mutex::scoped_lock lock(mutex_);
		boost::xtime_get(&xt, boost::TIME_UTC);
		count = signal_count;
		enabled = true;
		cond.notify_all();
	};
	///	Stops the sending of signals and suspends the thread.
	void disable()
	{
		enabled = false;
	}
	///	Forces the thread to terminate.
	void join()
	{
		boost::mutex::scoped_lock lock(mutex_);
		cond.notify_all();
		terminating = true;
		cond.wait(lock);
		thread_object->join();
	}
	///	The thread should be joined by the time the destructor is called.
	~timed_generator()
	{
		terminating = true;
		cond.notify_all();
		delete thread_object;
	}
private:
	void thread_function()
	{
		while (!terminating)
		{
			{
				boost::mutex::scoped_lock lock(mutex_);
				if (!enabled)
					cond.wait(lock);
			}
			if (terminating) break;
				
			xt.sec += xinterval.sec;
			xt.nsec += xinterval.nsec;
			if (xt.nsec >= 1000000000)
			{
				xt.nsec -= 1000000000;
				xt.sec++;
			}

			boost::thread::sleep(xt);

			if (terminating) break;
			(*this)();
			if (count)
				if (--count==0)
					disable();
		}
		cond.notify_all();
	}
	///	Class mutex.
	boost::mutex mutex_;
	boost::condition cond;
	boost::xtime xinterval;
	volatile bool terminating;
	boost::xtime xt;
	int count;

	boost::thread *thread_object;
	volatile bool enabled;
};

} } // namespace boost::signals


#endif // SIGNAL_NETWORK_TIMED_GENERATOR_HPP
