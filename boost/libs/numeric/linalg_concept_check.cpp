#include <boost/numeric/linalg_concepts.hpp>
#include <boost/rational.hpp>
#include <boost/numeric/linalg_valarray.hpp>

int main()
{
  using namespace boost;
  function_requires< FieldConcept< rational<int> > >();
  function_requires< CommutativeMultiplicativeRingWithIdentityConcept<int> >();
  function_requires< VectorSpaceConcept< std::valarray<float>, float> >();
  return 0;
}
