
//          Copyright Michael Mehling 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SHIFT_OPERATOR_NOOP_HPP_
#define SHIFT_OPERATOR_NOOP_HPP_

namespace shift {

struct noop{};

template<endianness EncodingEndianness, typename BufferType>
sink<EncodingEndianness, BufferType>& operator << (sink<EncodingEndianness, BufferType>& sink_, noop value) { /* empty */ }

template<endianness EncodingEndianness>
source<EncodingEndianness>&           operator >> (source<EncodingEndianness>& source_        , noop value) { /* empty */ }

}

#endif /* SHIFT_OPERATOR_NOOP_HPP_ */
