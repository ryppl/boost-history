template<typename T, typename Functor>
struct single_case
{
   typedef T handle_type;

   single_case(const Functor& functor) : functor_(functor) { }
   void operator()(const T& t)
   {
      functor_(t);
   }

   Functor functor_;
};


template<typename FirstCase, typename RestCase>
struct composite_case
{
   FirstCase first_;
   RestCase rest_;

   composite_case(FirstCase first, RestCase rest_)
      :  first_(first), rest_(rest) { }

   template<typename T>
   void operator()(const T& t)
   {
      if(is_same<T,

   }
}

