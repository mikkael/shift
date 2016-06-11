
//          Copyright Michael Mehling 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SHIFT_DETAIL_UTILITY_HPP_
#define SHIFT_DETAIL_UTILITY_HPP_

#include <shift/types/byte.hpp>
#include <shift/exception.hpp>

namespace shift { namespace detail {

template<typename BidirectionalIteratorType, typename OutputIteratorType>
OutputIteratorType reverse_copy(BidirectionalIteratorType begin, BidirectionalIteratorType end, OutputIteratorType destination) {
	BidirectionalIteratorType source=end;
	while(source != begin) *destination++ = *(--source);
	return destination;
}

}} // shift::detail

#endif /* SHIFT_DETAIL_UTILITY_HPP_ */
