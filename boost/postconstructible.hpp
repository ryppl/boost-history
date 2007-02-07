// A simple framework for creating objects with postconstructors.
// The objects must inherit from boost::postconstructible, and
// have their lifetimes managed by
// boost::shared_ptr created with the boost::deconstruct_ptr()
// function.
//
// Author: Frank Mori Hess 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_POSTCONSTRUCTIBLE_HEADER
#define BOOST_POSTCONSTRUCTIBLE_HEADER


namespace boost
{
  class postconstructible;
  namespace deconstruct_detail
  {
    void do_postconstruct(const boost::postconstructible *ptr);
  }

  class postconstructible
  {
  public:
    friend void deconstruct_detail::do_postconstruct(const boost::postconstructible *ptr);
  protected:
    postconstructible(): _postconstructed(false) {}
    virtual ~postconstructible() {}
    virtual void postconstruct() {}
  private:
    void do_postconstruct()
    {
      if(_postconstructed == false)
      {
        _postconstructed = true;
        postconstruct();
      }
    }

    bool _postconstructed;
  };
}
#endif
