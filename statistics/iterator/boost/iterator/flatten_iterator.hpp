///////////////////////////////////////////////////////////////////////////////
// flatten_iterator.hpp                              				 	 	 //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ITERATOR_FLATTEN_ITERATOR_HPP_ER_2010
#define BOOST_ITERATOR_FLATTEN_ITERATOR_HPP_ER_2010
#include <stdexcept>
#include <boost/next_prior.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/range.hpp>
#include <boost/iterator/iterator_facade.hpp>

// As per M.P.'suggestion

namespace boost{

	template<typename It>
	struct flatten_iterator_default{
    	typedef typename boost::iterator_value<It>::type r_;
    	typedef typename boost::range_iterator<r_>::type it_;
        typedef typename boost::iterator_reference<it_>::type reference_;
        typedef typename boost::iterator_difference<it_>::type diff_;
    };

	template<
    	class It,
        typename R = typename flatten_iterator_default<It>::reference_,
        typename D = typename flatten_iterator_default<It>::diff_
    >
	class flatten_iterator : public boost::iterator_facade<
        flatten_iterator<It,R,D>
      , It
      , boost::forward_traversal_tag
      , R
      , D
    >{
		typedef typename boost::iterator_value<It>::type nested_range_;
		typedef typename boost::range_iterator<nested_range_>::type nested_;

		typedef typename boost::iterator_facade<
        	flatten_iterator<It,R,D>
      		, It
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

    	diff_ distance_to()
    	{   
			diff_ d = 0;
            It it = this->b_; 
            if(it!=this->e_){ 
				d += std::distance(this->nb_,this->ne_);
                ++it; 
            } 
            while(it<this->e_){
				d += std::distance(boost::begin(*it),boost::end(*it));				
            	++it;
            }
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

        It b_,e_;
        bool ready_;
        nested_ nb_,ne_; // nested in b_

    	void update()
    	{
        	if((this->b_) != (this->e_)){
            	this->nb_ = boost::begin(*this->b_);
            	this->ne_ = boost::end(*this->b_);
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

