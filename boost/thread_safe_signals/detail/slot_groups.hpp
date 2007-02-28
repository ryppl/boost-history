// Thread-safe signals library

// Copyright Frank Mori Hess 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#ifndef BOOST_TSS_SLOT_GROUPS_HEADER
#define BOOST_TSS_SLOT_GROUPS_HEADER

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

#include <boost/thread_safe_signals/connection.hpp>
#include <boost/optional.hpp>
#include <cassert>
#include <list>
#include <map>
#include <utility>

namespace boost {
	namespace signalslib {
		namespace detail {
			enum slot_meta_group {front_ungrouped_slots, grouped_slots, back_ungrouped_slots};
			template<typename Group>
			struct group_key
			{
				typedef std::pair<enum slot_meta_group, boost::optional<Group> > type;
			};
#if 0
			template <typename T>
			void print_key(const T &key)
			{
				std::cerr << "key: " << key.first << " ";
				if(key.second)
					std::cerr << key.second.get();
				else
					std::cerr << "uninitialized";
				std::cerr << std::endl;
			}
#endif
			template<typename Group, typename GroupCompare>
			class group_key_less
			{
			public:
				group_key_less()
				{}
				group_key_less(const GroupCompare &group_compare): _group_compare(group_compare)
				{}
				bool operator ()(const typename group_key<Group>::type &key1, const typename group_key<Group>::type &key2) const
				{
					if(key1.first != key2.first) return key1.first < key2.first;
					if(key1.first != grouped_slots) return false;
					return _group_compare(key1.second.get(), key2.second.get());
				}
			private:
				GroupCompare _group_compare;
			};
			template<typename Group, typename GroupCompare, typename ValueType>
			class grouped_list
			{
			private:
				typedef std::list<ValueType> list_type;
				typedef std::map<typename group_key<Group>::type, typename list_type::iterator> map_type;
				typedef typename map_type::iterator map_iterator;
			public:
				typedef typename list_type::iterator iterator;
				typedef typename group_key<Group>::type group_key_type;
				typedef group_key_less<Group, GroupCompare> group_key_compare_type;

				grouped_list(const group_key_compare_type &group_key_compare):
					_group_key_compare(group_key_compare)
				{}
				iterator begin()
				{
					return _list.begin();
				}
				iterator end()
				{
					return _list.end();
				}
				iterator lower_bound(const group_key_type &key)
				{
					map_iterator map_it = _group_map.lower_bound(key);
					return get_list_iterator(map_it);
				}
				iterator upper_bound(const group_key_type &key)
				{
					map_iterator map_it = _group_map.upper_bound(key);
					return get_list_iterator(map_it);
				}
				void push_front(const group_key_type &key, const ValueType &value)
				{
					map_iterator map_it;
					if(key.first == front_ungrouped_slots)
					{// optimization
						map_it = _group_map.begin();
					}else
					{
						map_it = _group_map.lower_bound(key);
					}
					m_insert(map_it, key, value);
				}
				void push_back(const group_key_type &key, const ValueType &value)
				{
					map_iterator map_it;
					if(key.first == back_ungrouped_slots)
					{
						map_it = _group_map.end();
					}else
					{
						map_it = _group_map.upper_bound(key);
					}
					m_insert(map_it, key, value);
				}
				void erase(const group_key_type &key)
				{
					map_iterator map_it = _group_map.lower_bound(key);
					iterator begin_list_it = get_list_iterator(map_it);
					iterator end_list_it = upper_bound(key);
					if(begin_list_it != end_list_it)
					{
						_list.erase(begin_list_it, end_list_it);
						_group_map.erase(map_it);
					}
				}
				iterator erase(const group_key_type &key, const iterator &it)
				{
					assert(it != _list.end());
					map_iterator map_it = _group_map.lower_bound(key);
					assert(map_it != _group_map.end());
					assert(weakly_equivalent(map_it->first, key));
					if(map_it->second == it)
					{
						iterator next = it;
						++next;
						// if next is in same group
						if(next != _list.end() && next != upper_bound(key))
						{
							// also erases old entry
							_group_map.insert(map_it, typename map_type::value_type(key, next));
						}else
						{
							_group_map.erase(map_it);
						}
					}
					return _list.erase(it);
				}
				void clear()
				{
					_list.clear();
					_group_map.clear();
				}
			private:
				bool weakly_equivalent(const group_key_type &arg1, const group_key_type &arg2)
				{
					if(_group_key_compare(arg1, arg2)) return false;
					if(_group_key_compare(arg2, arg1)) return false;
					return true;
				}
				void m_insert(map_iterator map_it, const group_key_type &key, const ValueType &value)
				{
					iterator list_it = get_list_iterator(map_it);
					iterator new_it = _list.insert(list_it, value);
					if(map_it != _group_map.end() && weakly_equivalent(key, map_it->first))
					{
						_group_map.erase(map_it);
					}
					map_iterator lower_bound_it = _group_map.lower_bound(key);
					if(lower_bound_it == _group_map.end() ||
						weakly_equivalent(lower_bound_it->first, key) == false)
					{
						_group_map.insert(typename map_type::value_type(key, new_it));
					}
				}
				iterator get_list_iterator(const map_iterator &map_it)
				{
					iterator list_it;
					if(map_it == _group_map.end())
					{
						list_it = _list.end();
					}else
					{
						list_it = map_it->second;
					}
					return list_it;
				}

				list_type _list;
				// holds iterators to first list item in each group
				map_type _group_map;
				group_key_compare_type _group_key_compare;
			};
		} // end namespace detail
		enum connect_position { at_back, at_front };
	} // end namespace signalslib
} // end namespace EPG

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif // BOOST_TSS_SLOT_GROUPS_HEADER
