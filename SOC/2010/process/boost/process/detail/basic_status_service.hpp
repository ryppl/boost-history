//
// Boost.Process
// ~~~~~~~~~~~~~
//
// Copyright (c) 2006, 2007 Julio M. Merino Vidal
// Copyright (c) 2008, 2009 Boris Schaeling
// Copyright (c) 2010 Felipe Tanus, Boris Schaeling
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

/**
 * \file boost/process/detail/basic_status_service.hpp
 *
 * Includes the declaration of the basic status service class.
 */

#ifndef BOOST_PROCESS_DETAIL_BASIC_STATUS_SERVICE_HPP
#define BOOST_PROCESS_DETAIL_BASIC_STATUS_SERVICE_HPP

#include <boost/process/config.hpp>
#include <boost/process/pid_type.hpp>
#include <boost/process/detail/status_impl.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/system/error_code.hpp>
#include <vector>
#include <algorithm>

#if defined(BOOST_POSIX_API)
#elif defined(BOOST_WINDOWS_API)
#   include <windows.h>
#else
#   error "Unsupported platform."
#endif

namespace boost {
namespace process {
namespace detail {

template <typename StatusImplementation = status_impl>
class basic_status_service
    : public boost::asio::detail::service_base<StatusImplementation>
{
public:
    explicit basic_status_service(boost::asio::io_service &io_service)
        : boost::asio::detail::service_base<StatusImplementation>(io_service),
        run_(true),
        work_thread_(&basic_status_service<StatusImplementation>::work_thread, this)
    {
        handles_.push_back(::CreateEvent(NULL, FALSE, FALSE, NULL));
        if (handles_[0] == NULL)
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("CreateEvent() failed");
    }

    ~basic_status_service()
    {
        stop_work_thread();
        work_thread_.join();
        if (!::CloseHandle(handles_[0]))
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("CloseHandle() failed");
    }

    typedef boost::shared_ptr<StatusImplementation> implementation_type;

    void construct(implementation_type &impl)
    {
        impl = boost::make_shared<StatusImplementation>(StatusImplementation());
        boost::unique_lock<boost::mutex> lock(work_thread_mutex_);
        impls_.push_back(impl);
    }

    void destroy(implementation_type &impl)
    {
        // impl->destroy();
        impl.reset();
        // remove impl from impls_
    }

    int wait(implementation_type &impl, pid_type pid)
    {
        /*
        boost::system::error_code ec;
        int exit_code = impl->wait(pid, ec);
        boost::asio::detail::throw_error(ec);
        */
        int exit_code = 0;
        return exit_code;
    }

    template <typename Handler>
    void async_wait(implementation_type &impl, pid_type pid, Handler handler)
    {
        HANDLE handle = ::OpenProcess(SYNCHRONIZE | PROCESS_QUERY_INFORMATION, FALSE, pid);
        if (handle == NULL)
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("OpenProcess() failed");

        boost::unique_lock<boost::mutex> lock(work_thread_mutex_);
        interrupt_work_thread();
        work_thread_cond_.wait(work_thread_mutex_);
        if (!work_)
            work_.reset(new boost::asio::io_service::work(this->get_io_service()));
        handles_.push_back(handle);
        impl->async_wait(handle, this->get_io_service().wrap(handler));
        work_thread_cond_.notify_all();
    }

private:
    void shutdown_service()
    {
    }

    void work_thread()
    {
        while (running())
        {
            DWORD res = ::WaitForMultipleObjects(handles_.size(), &handles_[0], FALSE, INFINITE);
            if (res == WAIT_FAILED)
                BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("WaitForMultipleObjects() failed");
            else if (res - WAIT_OBJECT_0 == 0)
            {
                boost::unique_lock<boost::mutex> lock(work_thread_mutex_);
                if (!run_)
                    break;
                work_thread_cond_.notify_all();
                work_thread_cond_.wait(work_thread_mutex_);
            }
            else if (res - WAIT_OBJECT_0 > 0)
            {
                HANDLE handle = handles_[res - WAIT_OBJECT_0];
                DWORD exit_code;
                if (!::GetExitCodeProcess(handle, &exit_code))
                    BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("GetExitCodeProcess() failed");
                boost::unique_lock<boost::mutex> lock(work_thread_mutex_);
                for (std::vector<implementation_type>::iterator it = impls_.begin(); it != impls_.end(); ++it)
                    (*it)->complete(handle, exit_code);
                std::vector<HANDLE>::iterator it = handles_.begin();
                std::advance(it, res - WAIT_OBJECT_0);
                handles_.erase(it);
                if (handles_.size() == 1)
                    work_.reset();
            }
        }
    }

    bool running()
    {
        // Access to run_ is sychronized with stop_work_thread().
        boost::mutex::scoped_lock lock(work_thread_mutex_);
        return run_;
    }

    void interrupt_work_thread()
    {
        // By signaling the event in the first slot WaitForMultipleObjects() will return.
        // The work thread won't do anything except checking if it should continue to run.
        if (!::SetEvent(handles_[0]))
            BOOST_PROCESS_THROW_LAST_SYSTEM_ERROR("SetEvent() failed");
    }

    void stop_work_thread()
    {
        // Access to run_ is sychronized with running().
        boost::mutex::scoped_lock lock(work_thread_mutex_);
        run_ = false;
        interrupt_work_thread();
    }

    boost::scoped_ptr<boost::asio::io_service::work> work_;
    std::vector<implementation_type> impls_;
    boost::mutex work_thread_mutex_;
    boost::condition_variable_any work_thread_cond_;
    bool run_;
    boost::thread work_thread_;
    std::vector<HANDLE> handles_;
};

}
}
}

#endif 
