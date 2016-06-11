
//          Copyright Michael Mehling 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SRC_SHIFT_DETAIL_STATIC_ASSERT_HPP_
#define SRC_SHIFT_DETAIL_STATIC_ASSERT_HPP_

namespace shift { namespace detail {

template<bool x> struct assertion        { assertion(...){};};
template<      > struct assertion<false> {                  };

}} // shift::detail

#define SHIFT_STATIC_ASSERT(expr, message)                                                                          \
		class  ERROR_ ## message{};                                                                                 \
		enum   ENUM_ ## message { enum_##message##_value = sizeof((detail::assertion<expr>( ERROR_##message() ))) }

#endif /* SRC_SHIFT_DETAIL_STATIC_ASSERT_HPP_ */
