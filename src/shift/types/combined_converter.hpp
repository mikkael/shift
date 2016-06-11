
//          Copyright Michael Mehling 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SHIFT_TYPES_COMBINED_CONVERTER_HPP_
#define SHIFT_TYPES_COMBINED_CONVERTER_HPP_

#include <shift/types/converter.hpp>

namespace shift {

template<typename C1, typename C2, typename C3 = void, typename C4 = void, typename C5 = void>
class combined_converter;

template<typename C1, typename C2>
class combined_converter<C1, C2, void, void, void> : public converter<typename C1::decoded_type, typename C2::encoded_type>{
public:
	typedef C1                                 converter_1;
	typedef C2                                 converter_2;
	typedef typename converter_1::decoded_type decoded_type;
	typedef typename converter_2::encoded_type encoded_type;

	combined_converter(decoded_type& value, typename C1::params p1, typename C2::params p2)
	: conv2_result()
	, c1(value       , p1)
	, c2(conv2_result, p2)
	{}

	encoded_type encode()                 const { conv2_result = c1.encode(); return c2.encode(); }
	void         decode(encoded_type arg) const { c2.decode(arg); c1.decode(conv2_result);        }

private:

	typedef typename converter_2::decoded_type conv2_result_type;

	mutable conv2_result_type conv2_result;

	converter_1 c1;
	converter_2 c2;
};

template<typename C1, typename C2, typename C3>
class combined_converter<C1, C2, C3, void, void> : public converter<typename C1::decoded_type, typename C3::encoded_type>{
public:
	typedef C1                                 converter_1;
	typedef C2                                 converter_2;
	typedef C3                                 converter_3;
	typedef typename converter_1::decoded_type decoded_type;
	typedef typename converter_3::encoded_type encoded_type;

	combined_converter(decoded_type& value, typename C1::params p1, typename C2::params p2, typename C3::params p3)
	: conv2_decoded()
	, conv3_decoded()
	, c1(value        , p1)
	, c2(conv2_decoded, p2)
	, c3(conv3_decoded, p3)
	{}

	encoded_type encode() const {
		conv2_decoded = c1.encode();
		conv3_decoded = c2.encode();
		return          c3.encode();
	}

	void decode(encoded_type arg) const {
		c3.decode(arg          );
		c2.decode(conv3_decoded);
		c1.decode(conv2_decoded);
	}

private:

	typedef typename converter_2::decoded_type conv2_decoded_type;
	typedef typename converter_3::decoded_type conv3_decoded_type;

	mutable conv2_decoded_type conv2_decoded;
	mutable conv3_decoded_type conv3_decoded;

	converter_1 c1;
	converter_2 c2;
	converter_3 c3;
};

template<typename C1, typename C2, typename C3, typename C4>
class combined_converter<C1, C2, C3, C4, void> : public converter<typename C1::decoded_type, typename C4::encoded_type>{
public:
	typedef C1                                 converter_1;
	typedef C2                                 converter_2;
	typedef C3                                 converter_3;
	typedef C4                                 converter_4;
	typedef typename converter_1::decoded_type decoded_type;
	typedef typename converter_4::encoded_type encoded_type;

	combined_converter( decoded_type& value
	                  , typename C1::params p1
	                  , typename C2::params p2
	                  , typename C3::params p3
	                  , typename C4::params p4)
	: conv2_decoded()
	, conv3_decoded()
	, conv4_decoded()
	, c1(value        , p1)
	, c2(conv2_decoded, p2)
	, c3(conv3_decoded, p3)
	, c4(conv4_decoded, p4)
	{}

	encoded_type encode() const {
		conv2_decoded = c1.encode();
		conv3_decoded = c2.encode();
		conv4_decoded = c3.encode();
		return          c4.encode();
	}

	void decode(encoded_type arg) const {
		c4.decode(arg          );
		c3.decode(conv4_decoded);
		c2.decode(conv3_decoded);
		c1.decode(conv2_decoded);
	}

private:

	typedef typename converter_2::decoded_type conv2_decoded_type;
	typedef typename converter_3::decoded_type conv3_decoded_type;
	typedef typename converter_4::decoded_type conv4_decoded_type;

	mutable conv2_decoded_type conv2_decoded;
	mutable conv3_decoded_type conv3_decoded;
	mutable conv4_decoded_type conv4_decoded;

	converter_1 c1;
	converter_2 c2;
	converter_3 c3;
	converter_4 c4;
};

template<typename C1, typename C2, typename C3, typename C4, typename C5>
class combined_converter : public converter<typename C1::decoded_type, typename C5::encoded_type>{
public:
	typedef C1                                 converter_1;
	typedef C2                                 converter_2;
	typedef C3                                 converter_3;
	typedef C4                                 converter_4;
	typedef C5                                 converter_5;
	typedef typename converter_1::decoded_type decoded_type;
	typedef typename converter_5::encoded_type encoded_type;

	combined_converter( decoded_type& value
	                  , typename C1::params p1
	                  , typename C2::params p2
	                  , typename C3::params p3
	                  , typename C4::params p4
	                  , typename C5::params p5)
	: conv2_decoded()
	, conv3_decoded()
	, conv4_decoded()
	, conv5_decoded()
	, c1(value        , p1)
	, c2(conv2_decoded, p2)
	, c3(conv3_decoded, p3)
	, c4(conv4_decoded, p4)
	, c5(conv5_decoded, p5)
	{}

	encoded_type encode() const {
		conv2_decoded = c1.encode();
		conv3_decoded = c2.encode();
		conv4_decoded = c3.encode();
		conv5_decoded = c4.encode();
		return          c5.encode();
	}

	void decode(encoded_type arg) const {
		c5.decode(arg          );
		c4.decode(conv5_decoded);
		c3.decode(conv4_decoded);
		c2.decode(conv3_decoded);
		c1.decode(conv2_decoded);
	}

private:

	typedef typename converter_2::decoded_type conv2_decoded_type;
	typedef typename converter_3::decoded_type conv3_decoded_type;
	typedef typename converter_4::decoded_type conv4_decoded_type;
	typedef typename converter_5::decoded_type conv5_decoded_type;

	mutable conv2_decoded_type conv2_decoded;
	mutable conv3_decoded_type conv3_decoded;
	mutable conv4_decoded_type conv4_decoded;
	mutable conv5_decoded_type conv5_decoded;

	converter_1 c1;
	converter_2 c2;
	converter_3 c3;
	converter_4 c4;
	converter_5 c5;
};

}

#endif /* SHIFT_TYPES_COMBINED_CONVERTER_HPP_ */
