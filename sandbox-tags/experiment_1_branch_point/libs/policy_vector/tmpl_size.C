template <class T, unsigned int N>
void f( T x[N] ) { return; }

template <unsigned int N>
void g( float x[N] ) { return; }

template <class T>
void h( T x[10] ) { return; }

int
main()
{
  float x[10];
  f<float,10>( x ); // ok
  f( x );           // error
  g( x );           // error
  h( x );           // ok
  return 0;
}
