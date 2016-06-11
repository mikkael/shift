
//          Copyright Michael Mehling 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SHIFT_TYPES_CONVERTER_HPP_
#define SHIFT_TYPES_CONVERTER_HPP_

namespace shift {

template<typename DecodedType, typename EncodedType>
class converter {
public:
	virtual ~converter(){}
	virtual EncodedType encode()                const = 0;
	virtual void        decode(EncodedType arg) const = 0;
};

template<typename DecodedType, typename EncodedType, typename ScaleType = DecodedType>
class scale_converter : public converter<DecodedType, EncodedType> {
public:
	typedef ScaleType   scale_type;
	typedef DecodedType decoded_type;
	typedef EncodedType encoded_type;

	struct params {
		params(scale_type factor) : factor(factor){}
		scale_type factor;
	};

	scale_converter(decoded_type& value, scale_type factor)
	: converter<DecodedType, EncodedType>()
	, value(value)
	, factor(factor)
	{}

	scale_converter(decoded_type& value, params p)
	: converter<DecodedType, EncodedType>()
	, value(value)
	, factor(p.factor)
	{}

	encoded_type encode()                 const { return  static_cast<encoded_type>(value * factor); }
	void         decode(encoded_type arg) const { value = static_cast<decoded_type>(arg   / factor) ; }

private:
	mutable decoded_type& value;
	scale_type            factor;
};

template<typename DecodedType, typename EncodedType, typename OffsetType = DecodedType>
class offset_converter : public converter<DecodedType, EncodedType> {
public:
	typedef OffsetType  offset_type;
	typedef DecodedType decoded_type;
	typedef EncodedType encoded_type;

	struct params {
		params(offset_type offset) : offset(offset){}
		offset_type offset;
	};

	offset_converter(decoded_type& value, offset_type offset) : value(value), offset(  offset) {}
	offset_converter(decoded_type& value, params      p     ) : value(value), offset(p.offset) {}

	encoded_type encode()                 const { return  static_cast<encoded_type>(value + offset); }
	void         decode(encoded_type arg) const { value = static_cast<decoded_type>(arg   - offset); }

private:
	mutable decoded_type& value;
	offset_type           offset;
};

}

#endif /* SHIFT_TYPES_CONVERTER_HPP_ */
