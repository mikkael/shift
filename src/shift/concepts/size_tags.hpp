
//          Copyright Michael Mehling 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SHIFT_CONCEPTS_SIZE_TAGS_HPP_
#define SHIFT_CONCEPTS_SIZE_TAGS_HPP_

namespace shift {

struct variable_length {};

struct no_size_field {};

template<unsigned int Size> struct static_size { static const unsigned int size = Size; };

}

#endif /* SHIFT_CONCEPTS_SIZE_TAGS_HPP_ */
