#ifndef SHIFT_OPERATOR_STRING_
#define SHIFT_OPERATOR_STRING_

#include <shift/sink.hpp>
#include <shift/detail/size_encoding.hpp>

namespace shift {

template<typename SizeType>
class ostring {
public:
	ostring(const std::string& str) : str(str) {}
	const std::string& str;
private:
	template<endianness EncodingEndianness, typename BufferType, typename StringSizeType>
	friend sink<EncodingEndianness, BufferType>& operator << (sink<EncodingEndianness, BufferType>&, const ostring<StringSizeType>&);

	template<typename SinkType>
	unsigned int encode_size(SinkType& sink) const {
		return detail::size_encoder<SizeType>::encode(sink, str.length());
	}
};

template<endianness EncodingEndianness, typename BufferType, typename SizeType>
sink<EncodingEndianness, BufferType>& operator << (sink<EncodingEndianness, BufferType>& sink_, const ostring<SizeType>& str) {
	typedef sink<EncodingEndianness, BufferType> sink_type;
	const unsigned int length = str.encode_size(sink_);
	return detail::ostream_operator_interface<sink_type>::write_array(sink_, reinterpret_cast<const byte_type*>(str.str.c_str()), length);
}

///

}

#endif // SHIFT_OPERATOR_STRING_
