
#ifndef SORTED_SEQUENCE_HPP
#define SORTED_SEQUENCE_HPP

/**
* A sorted sequence implements an ordering on elements inserted into the
* sequence.
*/
template <
    class Type,
    class Sequence = std::deque<Type>,
    class Compare = std::less<typename Sequence::value_type>
>
class insertion_queue
{
    typedef Sequence sequence_type;
public:

    // value, pointer and reference types
    typedef typename sequence_type::value_type value_type;
    typedef typename sequence_type::pointer				pointer;
    typedef typename sequence_type::reference			reference;
    typedef typename sequence_type::const_reference		const_reference;

    // size type
    typedef typename sequence_type::size_type			size_type;
    typedef Compare										key_compare;

    insertion_queue()
        : m_comp()
        , m_data()
    {}

    insertion_queue(key_compare const& comp)
        : m_comp(comp)
        , m_data()
    {}

    insertion_queue(insertion_queue const& q)
        : m_comp()
        , m_data(q)
    {}

    size_type size() const
    { return m_data.size(); }

    bool empty() const
    { return m_data.empty(); }

    sequence_type const& sequence() const
    { return m_data; }

    /**
    * The front() method returns the first element in the
    * insertion queue. Runs in Theta(1).
    */
    const_reference front() const
    { return m_data.front(); }

    /**
    * The enqueue() method inserts the element into the
    * correct position of the queue. The queue is sorted
    * in descending order (highest first). Therefore, the
    * highest element will always be returned first.
    */
    void enqueue(const_reference value)
    {
        for(typename sequence_type::iterator i = m_data.begin(); i != m_data.end(); ++i) {
            const_reference elem = *i;
            if(!m_comp(value, elem)) {
                m_data.insert(i, value);
                return;
            }
        }

        // if we've made it this far, we actually just
        // have to push the value.
        m_data.push_back(value);
    }

    /**
    * The dequeue() method removes the first element in the
    * insertion_queue. Runs in Theta(1).
    */
    void dequeue()
    { m_data.pop_front(); }

private:
    key_compare m_comp;
    sequence_type m_data;
};

/**
* The insertion sort algorithm uses a priority queue (specifically
* an insertion queue in this case) to sort data. It runs in O(n^2)
* time. There are two major phases to the algorithm. The first is
* the construction of the priority queue and the second is the
* removal and re-insertion to sort the list.
*/
template <class RandomAccessIterator>
void insertion_sort(RandomAccessIterator first, RandomAccessIterator last)
{
    typedef RandomAccessIterator iterator;
    typedef typename iterator::value_type value_type;

    // use a selection queue to implement a selection sort
    insertion_queue<value_type> iq;

    // construct the queue by inserting all the elements\
    // into the queue
    for(iterator i = first; i != last; ++i) {
        iq.enqueue(*i);
    }

    // dequeue all the elements to get the sorted sequence
    // elements are re-assigned into the iterator's positions
    iterator i = first;
    while(!iq.empty()) {
        // get the highest element
        value_type elem = iq.front();
        iq.dequeue();

        // put it back into the original sequence
        *i = elem;
        ++i;
    }
}

#endif
