/*
 * Boost.Reflection / first prototype - paramater map for calls
 *
 * (C) Copyright Mariano G. Consoni 2007
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/ for latest version.
 */


#ifndef BOOST_EXTENSION_PARAMETER_MAP_HPP
#define BOOST_EXTENSION_PARAMETER_MAP_HPP


namespace boost { 
  namespace extension {

    template<class ParameterType, class ParameterIDType>
    struct parameter_pair
    {
      parameter_pair() {}
      parameter_pair(ParameterType p, ParameterIDType p_id)
        : parameter(p), parameter_id(p_id) {}

      ParameterType parameter;
      ParameterIDType parameter_id;
    };


    class parameter_map
    {
      class generic_parameter_map_container
      {
      public:
        virtual ~generic_parameter_map_container(void) {}
      };

      template<class ParameterType, class ParameterIDType>
      class parameter_map_container
        : public parameter_pair<ParameterType, ParameterIDType>,
          public generic_parameter_map_container
      {
      public:
        parameter_map_container(void) {}
        virtual ~parameter_map_container(void) {}
      };


      typedef std::list<generic_parameter_map_container *> ParameterList;
      ParameterList parameters_;


      template<class ParameterType, class ParameterIDType>
      typename ParameterList::iterator 
      find_parameter(ParameterIDType id)
      {
        typename ParameterList::iterator it = parameters_.begin();

        for(; it != parameters_.end(); ++it) {
          parameter_map_container<ParameterType, ParameterIDType> *pc = 
            dynamic_cast< parameter_map_container<ParameterType, 
            ParameterIDType> *>(*it);

          if(pc == NULL) {
            // if the cast failed it means that this element of the list
            // it isn't of the expected type, so we skip it.
            continue;
          }

          // now we check if the id is correct (knowing that the parameters
          // types are correct)
           if(pc->parameter_id == id) {
             return it;
           }
        }
        return parameters_.end();
      }


      template<class ParameterType, class ParameterIDType>
      parameter_pair<ParameterType, ParameterIDType> &
      get(ParameterIDType id)
      {
        typename ParameterList::iterator it = 
          find_parameter<ParameterType, ParameterIDType>(id);
      
         if(it == parameters_.end()) {
          // FIXME: free
          parameter_map_container<ParameterType, ParameterIDType> * ret = new 
            parameter_map_container<ParameterType, ParameterIDType>();

          parameters_.push_back(ret);
          return *ret;
         } else {
           // Change to dynamic if this fails
           return static_cast<parameter_map_container<ParameterType, 
            ParameterIDType> &> (*(*it));
         }
      }

    public:

      template<class ParameterType, class ParameterIDType>
      void add(ParameterType p, ParameterIDType parameter_id)
      {
         typedef parameter_pair<ParameterType, ParameterIDType> ElementType;

         ElementType & s = this->get<ParameterType, 
          ParameterIDType>(parameter_id);

        parameter_pair<ParameterType, ParameterIDType> pp(p, parameter_id);
         s = pp;
      }

    };

  } // extension
} // boost

#endif // BOOST_EXTENSION_PARAMETER_MAP_HPP
