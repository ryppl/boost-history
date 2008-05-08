struct rectangle_concept {
  rectangle_concept() {}

  template <typename rectangle_type>
  struct registration {
    typedef typename rectangle_traits<rectangle_type>::coordinate_type coordinate_type;
    typedef interval_data<coordinate_type> component_type;
  };

  template <orientation_2d_enum orient, typename T>
  static inline typename rectangle_traits<T>::interval_type 
  get(const T& rectangle) {
    return rectangle_traits<T>::get(rectangle, orient); 
  }
  
  template <typename T>
  static inline typename rectangle_traits<T>::interval_type 
  get(const T& rectangle, orientation_2d orient) {
    return rectangle_traits<T>::get(rectangle, orient); 
  }

  template <orientation_2d_enum orient, typename T, typename T2>
  static inline void set(T& rectangle, const T2& interval) {
    rectangle_traits<T>::set(rectangle, orient, interval); 
  }

  template <typename T, typename T2>
  static inline void set(T& rectangle, orientation_2d orient, const T2& interval) {
    rectangle_traits<T>::set(rectangle, orient, interval); 
  }

  template <typename T, typename T2, typename T3>
  static inline T construct(const T2& interval_horizontal,
                            const T3& interval_vertical) {
    return rectangle_traits<T>::construct(interval_horizontal, interval_vertical); }

  template <typename T, typename coordinate_type>
  static inline T construct(coordinate_type xl, coordinate_type yl, coordinate_type xh, coordinate_type yh) {
    return rectangle_traits<T>::construct(interval_data<coordinate_type>(xl, xh), 
                                          interval_data<coordinate_type>(yl, yh)); 
  }

  template <typename T, typename T2>
  static T copy_construct(const T2& rectangle) {
    return construct<T>
      (get(rectangle, HORIZONTAL),
       get(rectangle, VERTICAL));
  }

  template <typename rectangle_type_1, typename rectangle_type_2>
  static rectangle_type_1& assign(rectangle_type_1& lvalue, const rectangle_type_2& rvalue) {
    set(lvalue, HORIZONTAL, get(rvalue, HORIZONTAL));
    set(lvalue, VERTICAL, get(rvalue, VERTICAL));
    return lvalue;
  }

  template <typename T, typename T2>
  static bool equivilence(const T& rect1, const T2& rect2) {
    return interval_concept::equivilence(get(rect1, HORIZONTAL),
                                         get(rect2, HORIZONTAL)) &&
      interval_concept::equivilence(get(rect1, VERTICAL),
                                    get(rect2, VERTICAL));
  }

  template <typename rectangle_type>
  static typename rectangle_traits<rectangle_type>::coordinate_type
  xl(const rectangle_type& rectangle) {
    return interval_concept::get<LOW>(get<HORIZONTAL>(rectangle));
  }

  template <typename rectangle_type>
  static typename rectangle_traits<rectangle_type>::coordinate_type
  xh(const rectangle_type& rectangle) {
    return interval_concept::get<HIGH>(get<HORIZONTAL>(rectangle));
  }

  template <typename rectangle_type>
  static typename rectangle_traits<rectangle_type>::coordinate_type
  yl(const rectangle_type& rectangle) {
    return interval_concept::get<LOW>(get<VERTICAL>(rectangle));
  }

  template <typename rectangle_type>
  static typename rectangle_traits<rectangle_type>::coordinate_type
  yh(const rectangle_type& rectangle) {
    return interval_concept::get<HIGH>(get<VERTICAL>(rectangle));
  }

  template <typename rectangle_type, typename rectangle_type2>
  static bool contains_dispatch(const rectangle_type& rectangle, const rectangle_type2 rectangle_contained, rectangle_concept tag) {
    std::cout << "rectangle contains rectangle\n";
  }
  template <typename rectangle_type, typename point_type>
  static bool contains_dispatch(const rectangle_type& rectangle, const point_type point_contained, point_concept tag) {
    std::cout << "rectangle contains point\n";
  }
  template <typename concept_type, typename rectangle_type, typename geometry_type> 
  static bool contains(const rectangle_type& rectangle, const geometry_type& contained_geometry_object) {
    return contains_dispatch(rectangle, contained_geometry_object, concept_type());
  }

  template <typename rectangle_type, typename geometry_type>
  static bool contains(const rectangle_type& rectangle, const geometry_type& contained_geometry_object); 
};


