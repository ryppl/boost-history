// Copyright (c) 2003 Daniel Wallin

// Permission is hereby granted, free of charge, to any person or organization 
// obtaining a copy of the software covered by this license (the "Software") 
// to use, reproduce, display, distribute, execute, and transmit the Software, 
// and to prepare derivative works of the Software, and to permit others
// to do so, all subject to the following:

// The copyright notice in the Software and this entire statement, including 
// the above license grant, this restriction and the following disclaimer, 
// must be included in all copies of the Software, in whole or in part, and 
// all derivative works of the Software, unless such copies or derivative 
// works are solely in the form of machine-executable object code generated by 
// a source language processor.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT 
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE 
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE, 
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
// DEALINGS IN THE SOFTWARE.

#ifndef BOOST_LANGBINDING_CONFIG
#define BOOST_LANGBINDING_CONFIG

#ifdef BOOST_LANGBINDING_BUILD
  #define BOOST_LANGBINDING_DECL __declspec(dllexport)
#else
  #define BOOST_LANGBINDING_DECL __declspec(dllimport)
#endif

#endif


