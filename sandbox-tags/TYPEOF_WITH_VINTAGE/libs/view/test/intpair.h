
struct intpair
{
  // These typedefs are necessary to use intpair as Incrementable type
  // of counting_iterator.
  
  typedef intpair                         value_type;
  typedef intpair*                        pointer;
  typedef intpair&                        reference;
  typedef intpair                         difference_type;
  typedef std::random_access_iterator_tag iterator_category;
  
  intpair( int x )
    : p(x,0), N( 1 ) 
  { normalize(); }

  intpair( int x, int y, int theN )
    : p( x, y ), N( theN ) 
  { normalize(); }

  intpair( const std::pair<int,int>& theP, int theN )
    : p( theP ), N( theN ) 
  { normalize(); }

  intpair& operator++()
  { 
    ++p.first; normalize(); 
    return *this; 
  }
  
  intpair& operator--()
  { 
    --p.first; normalize(); 
    return *this; 
  }
  
  intpair operator+=( const intpair& rhs )
  { 
    p.first += rhs.p.first; p.second += rhs.p.second; 
    normalize();
    return *this; 
  } 
  
  intpair operator-=( const intpair& rhs )
  { 
    p.first -= rhs.p.first; p.second -= rhs.p.second; 
    normalize();
    return *this; 
  } 
  
  void normalize() 
  {
    if( p.first >= 0 )
    {
      p.second += p.first / N;
      p.first = p.first % N;
    }
    else
    {
      p.second -= ((-p.first) / N) + 1;
      p.first = (N - ((-p.first) % N)) % N;
    }
  }
  
  int norm() const
  { return p.first + N*p.second; }
  
  std::pair<int,int> p;
  int N;
};

bool operator==( const intpair& lhs, const intpair& rhs )
{
  return( lhs.N == rhs.N && lhs.p == rhs.p );
}

bool operator!=( const intpair& lhs, const intpair& rhs )
{
  return !( lhs == rhs );
}

intpair operator+( const intpair& lhs, const intpair& rhs )
{ return intpair( lhs ) += rhs; } 

intpair operator-( const intpair& lhs, const intpair& rhs )
{ return intpair( lhs ) -= rhs; }


struct norm: public std::unary_function<intpair,int>
{
  norm() {}

  int operator()( const intpair& ip ) const
    { return ip.norm(); }
};
