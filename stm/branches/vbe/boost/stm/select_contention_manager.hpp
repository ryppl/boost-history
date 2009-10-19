//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Justin E. Gottchlich 2009.
// (C) Copyright Vicente J. Botet Escriba 2009.
// Distributed under the Boost
// Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/stm for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_STM_SELECT_CONTENTION_MANAGER__HPP
#define BOOST_STM_SELECT_CONTENTION_MANAGER__HPP

//-----------------------------------------------------------------------------
#include <boost/stm/detail/config.hpp>
//-----------------------------------------------------------------------------

#if defined(BOOST_STM_CM_STATIC_CONF)
#if defined(BOOST_STM_CM_STATIC_CONF_except_and_back_off_on_abort_notice_cm)
#include <boost/stm/contention_managers/except_and_back_off_on_abort_notice_cm.hpp>
#else
#include BOOST_STM_CM_STATIC_CONF
//#include <boost/stm/contention_managers/default.hpp>
#endif
#else
#if defined(BOOST_STM_CM_DYNAMIC_CONF)
#include <boost/stm/contention_managers/polymorphic.hpp>
#endif
#endif

namespace boost { namespace stm {


#if defined(BOOST_STM_CM_STATIC_CONF)
    inline static contention_manager_type* get_contention_manager() {
       static contention_manager_type cm_;return &cm_;
    }
    inline static void cm_abort_on_new(transaction const &t) {
       return contention_manager_type::abort_on_new(t);
    }
    inline static void cm_abort_on_delete(transaction const &t,
                            base_transaction_object const &in) {
        return contention_manager_type::abort_on_delete(t,in);
    }

    inline static void cm_abort_on_read(transaction const &t,
                            base_transaction_object const &in) {
          return contention_manager_type::abort_on_read(t,in);
    }
    inline static void cm_abort_on_write(transaction &t,
                            base_transaction_object const &in) {
        return contention_manager_type::abort_on_write(t,in);
    }

    inline static bool cm_abort_before_commit(transaction const &t) {
       return contention_manager_type::abort_before_commit(t);
    }

    inline static bool cm_permission_to_abort(transaction const &lhs, transaction const &rhs) {
       return contention_manager_type::permission_to_abort(lhs,rhs);
    }

    inline static bool cm_permission_to_abort(transaction const &lhs, std::list<transaction*> &rhs) {
        return contention_manager_type::permission_to_abort(lhs,rhs);
    }

    inline static bool cm_allow_lock_to_abort_tx(int const & lockWaitTime, int const &lockAborted,
                            bool txIsIrrevocable, transaction const &rhs) {
          return contention_manager_type::allow_lock_to_abort_tx(lockWaitTime,lockAborted,txIsIrrevocable,rhs);
    }

    inline static int cm_lock_sleep_time() {
        return contention_manager_type::lock_sleep_time();
    }

    inline static void cm_perform_isolated_tx_wait_priority_promotion(transaction &t) {
        return contention_manager_type::perform_isolated_tx_wait_priority_promotion(t);
    }
    inline static void cm_perform_irrevocable_tx_wait_priority_promotion(transaction &t) {
        return contention_manager_type::perform_irrevocable_tx_wait_priority_promotion(t);
    }

#else
    struct poly_contention_manager {
        static base_contention_manager *cm_;
    };
    inline static void contention_manager(base_contention_manager *rhs) {
       delete poly_contention_manager::cm_; poly_contention_manager::cm_ = rhs;
    }
    inline static base_contention_manager* get_contention_manager() {
        return poly_contention_manager::cm_;
    }
    inline static base_contention_manager* get_cm() {
        return poly_contention_manager::cm_;
    }
    
    inline static void cm_abort_on_new(transaction const &t) {
        return get_cm()->abort_on_new(t);
    }
    inline static void cm_abort_on_delete(transaction const &t, base_transaction_object const &in) {
        return get_cm()->abort_on_delete(t,in);
    }

    inline static void cm_abort_on_read(transaction const &t, base_transaction_object const &in) {
        return get_cm()->abort_on_read(t,in);
    }
    inline static void cm_abort_on_write(transaction &t, base_transaction_object const &in) {
        return get_cm()->abort_on_write(t,in);
    }

    inline static bool cm_abort_before_commit(transaction const &t) {
        return get_cm()->abort_before_commit(t);
    }

    inline static bool cm_permission_to_abort(transaction const &lhs, transaction const &rhs) {
        return get_cm()->permission_to_abort(lhs,rhs);
    }

    inline static bool cm_permission_to_abort(transaction const &lhs, std::list<transaction*> &rhs) {
       return get_cm()->permission_to_abort(lhs,rhs);
    }

    inline static bool cm_allow_lock_to_abort_tx(int const & lockWaitTime, int const &lockAborted,
                            bool txIsIrrevocable, transaction const &rhs) {
        return get_cm()->allow_lock_to_abort_tx(lockWaitTime,lockAborted,txIsIrrevocable,rhs); }

    inline static int cm_lock_sleep_time() {
        return get_cm()->lock_sleep_time();
    }

    inline static void cm_perform_isolated_tx_wait_priority_promotion(transaction &t) {
        return get_cm()->perform_isolated_tx_wait_priority_promotion(t);
    }
    inline static void cm_perform_irrevocable_tx_wait_priority_promotion(transaction &t) {
        return get_cm()->perform_irrevocable_tx_wait_priority_promotion(t);
    }

#endif


}}
#endif // CONTENTION_MANAGER_H
