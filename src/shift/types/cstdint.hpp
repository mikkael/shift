
//          Copyright Michael Mehling 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SHIFT_TYPES_CSTDINT_HPP_
#define SHIFT_TYPES_CSTDINT_HPP_

#include <shift/detail/static_assert.hpp>

#if __cplusplus >= 201103L
	#define USING_STANDARD_TYPES
	#include <cstdint>
#elif defined __STDC__
	#define USING_STANDARD_TYPES
	#include <stdint.h>
#endif

#if defined USING_STANDARD_TYPES
	namespace shift {
		using ::int8_t;
		using ::int16_t;
		using ::int32_t;
		using ::int64_t;
		using ::uint8_t;
		using ::uint16_t;
		using ::uint32_t;
		using ::uint64_t;
	}
#elif defined USE_BOOST_CSTDINT
	#include <boost/cstdint.hpp>

	namespace shift {
		typedef boost::int8_t   int8_t ;
		typedef boost::int16_t  int16_t;
		typedef boost::int32_t  int32_t;
		typedef boost::int64_t  int64_t;
		typedef boost::uint8_t  uint8_t ;
		typedef boost::uint16_t uint16_t;
		typedef boost::uint32_t uint32_t;
		typedef boost::uint64_t uint64_t;
	}
#else
	SHIFT_STATIC_ASSERT(false, "could not define c std int types");
#endif

#endif /* SHIFT_TYPES_CSTDINT_HPP_ */
