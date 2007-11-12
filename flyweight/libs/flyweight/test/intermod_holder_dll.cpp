/* Boost.Flyweight test of intermodule_holder.
 *
 * Copyright 2006-2007 Joaqu�n M L�pez Mu�oz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/flyweight for library home page.
 */

#include "intermod_holder_dll.hpp"

intermodule_flyweight_string create_intermodule_flyweight_string(
  const std::string& str)
{
  return intermodule_flyweight_string(str);
}
