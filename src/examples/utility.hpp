#ifndef SRC_EXAMPLES_UTILITY_HPP_
#define SRC_EXAMPLES_UTILITY_HPP_

#include <iosfwd>
#include <cstddef>
#include <iomanip>

namespace example {

inline std::string to_binary_representation_string(uint8_t v) {
	std::string str(8, '0');
	for (uint8_t i=8; i>0; --i)
		if( v & (1 << (i-1))) str[8-i] = '1';
	return str;
}

inline unsigned int number_of_digits(unsigned int n, unsigned int base = 10) {
	unsigned int n_digits = 0;
	while(n > 0) {
		++n_digits;
		n /= base;
	}
	return n_digits;
}

template<typename SizeType>
void debug_print(std::ostream& stream, const shift::byte_type* buffer, SizeType size) {
	const unsigned int n_digits = number_of_digits(size-1);
	for (std::size_t i=0; i<size; ++i){
		stream << " | "
		       << std::dec << std::setw(n_digits) << std::setfill(' ') << i
		       << " | "
		       << to_binary_representation_string(buffer[i])
		       << " | "
		       << std::dec << std::setw(3) << std::setfill(' ') << static_cast<unsigned int>(buffer[i])
		       << " | "
		       << "0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(buffer[i])
		       << " | "
		       << static_cast<char>(std::max(static_cast<shift::byte_type>(' '), buffer[i]))
		       << " |" << std::dec << std::endl;
	}
}

template<typename MessageType>
void debug_print(std::ostream& stream, MessageType& msg) {
	typedef MessageType message_type;
	const shift::byte_type* buffer = msg.buffer();
	debug_print(stream, msg.buffer(), msg.size());
}

template<typename FordwardIteratorType, typename OutputType>
struct stream_repeated {
	stream_repeated(FordwardIteratorType begin, FordwardIteratorType end, const std::string& delimiter) :
	begin(begin), end(end), delimiter(delimiter) {}

	FordwardIteratorType begin;
	FordwardIteratorType end;
	const std::string&   delimiter;
};

template<typename FordwardIteratorType, typename OutputType>
std::ostream& operator<<(std::ostream& stream, const stream_repeated<FordwardIteratorType, OutputType>& repeated_) {
	for (FordwardIteratorType it = repeated_.begin; it != repeated_.end;) {
		stream << static_cast<OutputType>(*it);
		if (++it != repeated_.end) stream << repeated_.delimiter;
	}
	return stream;
}

}

#endif /* SRC_EXAMPLES_UTILITY_HPP_ */
