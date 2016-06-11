#include <iostream>
#include <iterator>
#include <iomanip>
#include <algorithm>
#include <functional>
#include <climits>
#include <tr1/functional>

#include <code_sample.hpp>

#include <shift/types/var_int.hpp>
#include <shift/operator/var_int.hpp>
#include <shift/sink.hpp>
#include <shift/source.hpp>
#include <shift/buffer/static_buffer.hpp>
#include <shift/operator/universal.hpp>

#include <examples/utility.hpp>

#include <cmath>

using namespace shift;
typedef shift::buffer_position pos;

namespace {

template<typename UintType, typename IntType>
UintType zig_zag_encode(IntType value) {
	return (value << 1) ^ (value >> (sizeof(IntType) * CHAR_BIT - 1));
}

template<typename UintType, typename IntType>
IntType zig_zag_decode(UintType value) {
	return (value & 1) ? -((value >> 1) + 1) : value >> 1;
}

void print(int i) {
	uint8_t* ptr = reinterpret_cast<uint8_t*>(&i);

	std::cout << example::to_binary_representation_string(*(ptr + 3)) << " "
	          << example::to_binary_representation_string(*(ptr + 2)) << " "
	          << example::to_binary_representation_string(*(ptr + 1)) << " "
	          << example::to_binary_representation_string(*(ptr + 0)) << std::endl;

}

CODE_SAMPLE( var_int_test, "var_int_test") {

	         int array        [] = {0, -1, 1, -2, 2, -3, 3, -4, 4, 2147483647, -2147483648 };
	unsigned int array_encoded[] = {0,  1, 2,  3, 4,  5, 6,  7, 8, 4294967294,  4294967295 };

	const unsigned int size = sizeof(array)/sizeof(int);

	for (unsigned int i=0; i<size; ++i) {
		unsigned int encoded = zig_zag_encode<unsigned int, int>(array[i]        );
		         int decoded = zig_zag_decode<unsigned int, int>(array_encoded[i]);

		std::cout << array[i] << " " << array_encoded[i] << " " << encoded << " " << decoded << std::endl;
	}
}

CODE_SAMPLE( var_int_test__2, "var_int_test") {
	typedef shift::sink  <shift::little_endian, shift::static_buffer<64> > sink_type;
	typedef shift::source<shift::little_endian>                            source_type;
	typedef shift::var_int<long long>                                     int_type;

	sink_type sink;

	long long v = 4;
	int_type value(v);

	try {
		sink << pos(0);
		sink << value;
	} catch (shift::out_of_range& e) {
		std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
	}

	example::debug_print(std::cout, sink);

	sink.clear();
}

} // namespace

