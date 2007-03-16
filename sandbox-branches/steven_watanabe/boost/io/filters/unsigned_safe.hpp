#ifndef BOOST_IO_FILTERS_UNSIGNED_SAFE_HPP
#define BOOST_IO_FILTERS_UNSIGNED_SAFE_HPP
namespace boost{namespace io{namespace filters{

class unsigned_safe
  //.Purpose:
  //.  provides safeguards to prevent arith op
  //.  from creating negative value
  //.  (which may be interpreted as large positive
  //.  unsigned).
  {
  public:
    unsigned_safe(unsigned a_uns)
      : m_uns(a_uns)
      {}
    unsigned_safe(int a_int)
      : m_uns((a_int>0)?a_int:0)
      {}
      unsigned
    as_unsigned(void)const
      { return m_uns
      ;}
      unsigned_safe
    operator-(unsigned a_uns)
      { if(m_uns > a_uns)
        { return unsigned_safe(m_uns-a_uns)
        ;}
        else
        { return unsigned_safe(0)
        ;}
      ;}
      unsigned_safe
    operator+(unsigned a_uns)
      { return unsigned_safe(m_uns+a_uns)
      ;}
      unsigned_safe
    operator-(int a_uns)
      { if(a_uns > 0)
        { return operator-(unsigned(a_uns))
        ;}
        else
        { return operator+(unsigned(-a_uns))
        ;}
      ;}
      unsigned_safe
    operator+(int a_uns)
      { if(a_uns > 0)
        { return operator+(unsigned(a_uns))
        ;}
        else
        { return operator-(unsigned(-a_uns))
        ;}
      ;}
      unsigned_safe const&
    operator+=(int a_int)
      { m_uns = (operator+(a_int)).as_unsigned()
      ; return *this
      ;}
      unsigned_safe const&
    operator-=(int a_int)
      { m_uns = (operator-(a_int)).as_unsigned()
      ; return *this
      ;}
      unsigned
    operator++(void)
      { return (operator+=(1)).as_unsigned()
      ;}
      unsigned
    operator--(void)
      { return (operator-=(1)).as_unsigned()
      ;}
  private:
      unsigned
    m_uns
      ;
  };

}}}//exit namespace boost::io::filters
#endif
