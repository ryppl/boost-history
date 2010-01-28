///////////////////////////////////////////////////////////////////////////////
// flatten_iterator.hpp                              				 	 	 //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ITERATOR_FLATTEN_ITERATOR_HPP_ER_2010
#define BOOST_ITERATOR_FLATTEN_ITERATOR_HPP_ER_2010
#include <stdexcept>
#include <boost/mpl/if.hpp>
#include <boost/next_prior.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/range.hpp>
#include <boost/iterator/iterator_facade.hpp>

// As per M.P.'s suggestion

namespace boost{


	template<typename It>
	struct flatten_iterator_nested
    {
    	typedef typename boost::iterator_reference<It>::type ref_range_;
        typedef typename boost::remove_reference<ref_range_>::type range_;
    	typedef typename boost::range_iterator<range_>::type it_;
        typedef typename boost::iterator_reference<it_>::type ref_;
        typedef typename boost::remove_cv<
        	typename boost::remove_reference<ref_>::type
        >::type val_;
        typedef typename boost::iterator_difference<it_>::type diff_;
    };
    
	template<
    	class It,
        bool is_ref = true,
        typename R = typename boost::mpl::if_c<
        	is_ref,
            typename flatten_iterator_nested<It>::ref_,
            typename flatten_iterator_nested<It>::val_
        >::type,
        typename D = typename flatten_iterator_nested<It>::diff_
    >
	class flatten_iterator : public boost::iterator_facade<
        flatten_iterator<It,is_ref,R,D>
      , typename flatten_iterator_nested<It>::val_
      , boost::forward_traversal_tag
      , R
      , D
    >{
		typedef flatten_iterator_nested<It> nested_;
		typedef typename nested_::it_ nit_;

		typedef typename boost::iterator_facade<
        	flatten_iterator<It,is_ref,R,D>
      		, typename flatten_iterator_nested<It>::val_
      		, boost::forward_traversal_tag
      		, R
      		, D
    	> super_;

		typedef typename super_::difference_type diff_;
		typedef typename super_::reference ref_;
        
		public:        
        
        flatten_iterator():super_(){}
                        
        explicit flatten_iterator(It b,It e):super_(),b_(b),e_(e){
        	this->update();
        }

        flatten_iterator(const flatten_iterator& that)
        :super_(),b_(that.b_),e_(that.e_)
        {
        	this->update();
        }

		flatten_iterator& operator=(const flatten_iterator& that){
        	if(that!=&this){
            	super_::operator=(that);
				this->b_ = that.b_;
                this->e_ = that.e_;
            }
            this->update();
            return (*this);
        }

		protected:
        friend class boost::iterator_core_access;
			
        void increment(){ 
        	if(
            	(++(this->nb_)) == (this->ne_)
            )
        	{
            	++this->b_;
            	this->update();
        	}
		}
        
        //Not needed given category_traversal
        void decrement(){
			throw std::runtime_error(
				"flatten_iterator::decrement() not allowed"            
            );
        }
	
		void advance(diff_ n){
        	// Needs checking
        
        	diff_ k = std::distance(this->b_,this->e_);
			if(n<k){
            	this->nb_ += boost::next(this->nb_,n);
            }else{
            	++this->b_;
                this->update(); 
            	return this->advance(n-k);
            }
        }

    	ref_ dereference()const
    	{   
        	return (*this->nb_);
    	}

    	diff_ distance_to(const flatten_iterator& other)const
    	{   
        	if(this->b_ == other.b_){
            	return std::distance(this->nb_,other.nb_);
            }
            if(this->b_ < other.b_){
            	return dist_impl(other,*this);
            }
            if(this->b_ < other.b_){
            	return dist_impl(*this,other);
            }
            throw std::runtime_error(
                "flatten_iterator::distance_to"
            );
    	}

    	bool equal(const flatten_iterator& rhs)const
    	{   
        	bool is_end1 = (this->b_ == this->e_);
        	bool is_end2 = (rhs.b_ == rhs.e_);
        
        	if(is_end1 != is_end2)
            	return false;
        	if(is_end1 && is_end2)
            	return true;
        	return (this->nb_ == this->ne_);
    	}

        mutable It b_,e_;
        mutable nit_ nb_,ne_; // nested in *b_

    	void update()const
    	{
        	if((this->b_) != (this->e_)){
            	this->nb_ = boost::begin(*this->b_);
            	this->ne_ = boost::end(*this->b_);
        	}
    	}

    	static diff_ dist_impl(
        	const flatten_iterator& x,
            const flatten_iterator& y
        ){
        	BOOST_ASSERT(x.b_ < y.b_);
            diff_ d = std::distance(x.nb_,x.ne_);
            It it = x.b_;
            while((++it)<y.b_){
                d += std::distance(boost::begin(*it),boost::end(*it));
            }
        	BOOST_ASSERT(it == y.b_);
			if(it!=x.e_){
            	d += std::distance(boost::begin(*it),boost::end(x.nb_));
            }
			
        }

        
	};        


	template<typename It>	
    flatten_iterator<It>
	make_flatten_iterator(It b,It e){
    	typedef flatten_iterator<It> result_;
    	return result_(b,e);
    }

}//boost

#endif

