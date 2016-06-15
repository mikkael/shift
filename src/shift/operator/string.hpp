#ifndef SHIFT_OPERATOR_STRING_
#define SHIFT_OPERATOR_STRING_

#include <shift/sink.hpp>
#include <shift/types/cstdint.hpp>
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

template<endianness EncodingEndianness, typename BufferType>
sink<EncodingEndianness, BufferType>& operator << (sink<EncodingEndianness, BufferType>& sink_, const std::string& str) {
	return sink_ << ostring<shift::uint16_t>(str);
}

///

template<typename SizeType>
class istring {
public:
	explicit istring(std::string& str) : str(str) {}
	mutable std::string& str;
private:
	template<endianness EncodingEndianness, typename SizeType_>
	friend source<EncodingEndianness>& operator >> (source<EncodingEndianness>& source_, const istring<SizeType_>& str_);

	template<typename SourceType>
	unsigned int decode_size(SourceType& source) const {
		return detail::size_decoder<SizeType>::decode_size(source);
	}
};

template<>
class istring<no_size_field> {
public:
	istring(std::string& str, unsigned int size) :str(str), size(size) {}
	mutable std::string& str;
	const unsigned int size;
private:
	template<endianness EncodingEndianness, typename SizeType_>
	friend source<EncodingEndianness>& operator >> (source<EncodingEndianness>& source_, const istring<SizeType_>& str_);

	template<typename SourceType>
	unsigned int decode_size(SourceType& sink) const {
		return size;
	}
};

template<unsigned int Size>
class istring<static_size<Size> > {
public:
	explicit istring(std::string& str) : str(str) {}
	mutable std::string& str;
	static const unsigned int size = Size;
private:
	template<endianness EncodingEndianness, typename SizeType_>
	friend source<EncodingEndianness>& operator >> (source<EncodingEndianness>& source_, const istring<SizeType_>& str_);

	template<typename SourceType>
	unsigned int decode_size(SourceType& sink) const {
		return size;
	}
};

template<endianness EncodingEndianness, typename SizeType>
source<EncodingEndianness>& operator >> (source<EncodingEndianness>& source_, const istring<SizeType>& str_) {
	const unsigned int length = str_.decode_size(source_);
	std::pair<const shift::byte_type*, const shift::byte_type*> block = detail::istream_operator_interface<source<EncodingEndianness> >::get_array(source_, length);
	str_.str.assign(reinterpret_cast<const char*>(block.first), reinterpret_cast<const char*>(block.second));
	return source_;
}

template<endianness EncodingEndianness>
source<EncodingEndianness>& operator >> (source<EncodingEndianness>& source_, std::string& str_) {
	return source_ >> istring<shift::uint16_t>(str_);
}

}

#endif // SHIFT_OPERATOR_STRING_
