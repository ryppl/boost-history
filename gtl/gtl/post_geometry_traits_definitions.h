template <typename rectangle_type, typename geometry_type>
bool rectangle_concept::contains(const rectangle_type& rectangle, const geometry_type& contained_geometry_object) {
  return rectangle_concept::contains_dispatch(rectangle, contained_geometry_object, typename geometry_traits<geometry_type>::geometry_concept());
}

template <typename geometry_type>
struct component_type { 
  typedef typename geometry_traits<geometry_type>::geometry_concept::template
  registration<geometry_type>::component_type type;
};

