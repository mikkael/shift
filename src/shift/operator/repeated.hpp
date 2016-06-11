
//          Copyright Michael Mehling 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SHIFT_OPERATOR_REPEATED_HPP_
#define SHIFT_OPERATOR_REPEATED_HPP_

#include <shift/sink.hpp>
#include <shift/source.hpp>
#include <shift/concepts/size_tags.hpp>
#include <shift/detail/size_encoding.hpp>

namespace shift {

template<typename SizeType, typename ForwardIteratorType>
class orepeated {
public:
	orepeated(ForwardIteratorType begin, ForwardIteratorType end) : begin(begin), end(end) {}

	ForwardIteratorType begin;
	ForwardIteratorType end;
private:
	template<endianness EncodingEndianness, typename BufferType, typename ForwardIteratorType__, typename SizeType__>
	friend sink<EncodingEndianness, BufferType>& operator << (sink<EncodingEndianness, BufferType>&, const orepeated<SizeType__, ForwardIteratorType__>&);

	template<typename SinkType>
	unsigned int encode_size(SinkType& sink) const {
		return detail::size_encoder<SizeType>::encode(sink, std::distance(begin, end));
	}
};

template<endianness EncodingEndianness, typename BufferType, typename ForwardIteratorType, typename SizeType>
sink<EncodingEndianness, BufferType>& operator << (sink<EncodingEndianness, BufferType>& sink_, const orepeated<SizeType, ForwardIteratorType>& repeated_) {
	repeated_.encode_size(sink_);
	for (ForwardIteratorType it = repeated_.begin; it != repeated_.end; ++it) sink_ << *it;
	return sink_;
}

/////

template<typename SizeType, typename OutputIteratorType>
class irepeated {
public:
	explicit irepeated(OutputIteratorType back_inserter                    ) : iterator(back_inserter), size(size_), size_() {}
	         irepeated(OutputIteratorType back_inserter, unsigned int& size) : iterator(back_inserter), size(size ), size_() {}
	OutputIteratorType iterator;
	unsigned int& size;
private:
	unsigned int size_;
	template<endianness EncodingEndianness, typename SizeType__, typename OutputIteratorType__>
	friend source<EncodingEndianness>& operator >> (source<EncodingEndianness>&, irepeated<SizeType__, OutputIteratorType__>);

	template<typename SourceType>
	unsigned int decode_size(SourceType& source) {
		size = detail::size_decoder<SizeType>::decode_size(source);
		return size;
	}
};

template<typename OutputIteratorType>
class irepeated<no_size_field, OutputIteratorType> {
public:
	irepeated(OutputIteratorType back_inserter, unsigned int size) : iterator(back_inserter), size(size) {}
	OutputIteratorType iterator;
	const unsigned int size;
private:
	template<endianness EncodingEndianness, typename SizeType__, typename OutputIteratorType__>
	friend source<EncodingEndianness>& operator >> (source<EncodingEndianness>&, irepeated<SizeType__, OutputIteratorType__>);

	template<typename SourceType>
	unsigned int decode_size(SourceType& sink) {
		return size;
	}
};

template<unsigned int Size, typename OutputIteratorType>
class irepeated<static_size<Size>, OutputIteratorType> {
public:
	explicit irepeated(OutputIteratorType back_inserter) : iterator(back_inserter) {}
	OutputIteratorType iterator;
	static const unsigned int size = Size;
private:
	template<endianness EncodingEndianness, typename SizeType__, typename OutputIteratorType__>
	friend source<EncodingEndianness>& operator >> (source<EncodingEndianness>&, irepeated<SizeType__, OutputIteratorType__>);

	template<typename SourceType>
	unsigned int decode_size(SourceType& sink) {
		return size;
	}
};

template<endianness EncodingEndianness, typename ContainerType>
source<EncodingEndianness>& operator >> (source<EncodingEndianness>& source_, std::back_insert_iterator<ContainerType>& iterator) {
	typedef typename std::back_insert_iterator<ContainerType>::container_type::value_type value_type;
	value_type tmp;
	source_ >> tmp;
	iterator = tmp;
	return source_;
}

template<endianness EncodingEndianness, typename SizeType, typename OutputIteratorType>
source<EncodingEndianness>& operator >> (source<EncodingEndianness>& source_, irepeated<SizeType, OutputIteratorType> repeated_) {
	const unsigned int length = repeated_.decode_size(source_);
	for (unsigned int i=0; i<length; ++i) source_ >> *(repeated_.iterator)++;
	return source_;
}

} // shift

#endif /* SHIFT_OPERATOR_REPEATED_HPP_ */
