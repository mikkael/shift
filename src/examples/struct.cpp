#include <iostream>
#include <iterator>
#include <iomanip>
#include <algorithm>
#include <functional>

#include <code_sample.hpp>

#include <shift/types/fixed_width_uint.hpp>
#include <shift/sink.hpp>
#include <shift/source.hpp>
#include <shift/buffer/static_buffer.hpp>
#include <shift/buffer/vector.hpp>
#include <shift/operator/universal.hpp>

#include <examples/utility.hpp>


using namespace shift;
typedef shift::buffer_position pos;

namespace {

struct test_struct {

	test_struct()
	: t1()
	, t2()
	, t3()
	, t4()
	, t5()
	{}

	test_struct( unsigned int p1
	           , unsigned int p2
	           , unsigned int p3
	           , bool         p4
	           , float        p5)
	: t1(p1)
	, t2(p2)
	, t3(p3)
	, t4(p4)
	, t5(p5)
	{}

	uint8_t        t1;
	uint2_t        t2;
	uint3_t        t3;
	bool           t4;
	float          t5;

	void print(const std::string& prefix = ""){
		std::cout << prefix << std::endl
		          << "t1: " << static_cast<unsigned int>(t1      ) << std::endl
		          << "t2: " << static_cast<unsigned int>(t2.get()) << std::endl
		          << "t3: " << static_cast<unsigned int>(t3.get()) << std::endl
		          << "t4: " << t4 << std::endl
		          << "t5: " << t5 << std::endl;
	}
};

template<typename CoderType>
CoderType& serialize(CoderType& msg, test_struct& obj) {
	typedef typename CoderType::mode mode;
	msg % pos(0   ) % obj.t1
	    % pos(1, 7) % obj.t2
	    % pos(1, 2) % obj.t3
	    % pos(2, 3) % obj.t4
	    % pos(3   ) % obj.t5;
	return msg;
}

template<typename CoderType>
CoderType& operator >> (CoderType& msg, test_struct& obj) {
	msg >> pos(0   ) >> obj.t1
	    >> pos(1, 7) >> obj.t2
	    >> pos(1, 2) >> obj.t3
	    >> pos(2, 3) >> obj.t4
	    >> pos(3   ) >> obj.t5;
	return msg;
}

template<typename CoderType>
CoderType& operator << (CoderType& msg, const test_struct& obj) {
	msg << pos(0   ) << obj.t1
	    << pos(1, 7) << obj.t2
	    << pos(1, 2) << obj.t3
	    << pos(2, 3) << obj.t4
	    << pos(3   ) << obj.t5;
	return msg;
}

CODE_SAMPLE(universal_operator, "universal operator for a test struct", "struct", "universal operator") {

	typedef shift::sink<little_endian, shift::static_buffer<64> > sink_type;
	sink_type sink_inst;
	test_struct obj(129, 3, 4, true, 3.14);
	sink_inst.clear();

	try {
		sink_inst % obj;
	} catch (shift::out_of_range& e) {
		std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
	}

	typedef source<little_endian> source_type;
	source_type source(sink_inst.buffer(), sink_inst.size());
	test_struct obj_deserialized;

	try {
		source % obj_deserialized;
	} catch (shift::out_of_range& e) {
		std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
	}

	example::debug_print(std::cout, sink_inst);
	obj.print("\noriginal:");
	obj_deserialized.print("\ndeserialized:");
}


CODE_SAMPLE(struct_test, "stream in/out operators for a test struct", "struct", "stream operator", ">>", "<<"){
	test_struct obj(129, 3, 4, true, 3.14);

	typedef shift::sink<little_endian, shift::static_buffer<64> > sink_message_type;
	sink_message_type sink_instance;
	sink_instance.clear();

	try {
		sink_instance << obj;
	} catch (shift::out_of_range& e) {
		std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
	}

	typedef source<little_endian> source_type;
	source_type source(sink_instance.buffer(), sink_instance.size());

	test_struct obj_deserialized;

	try {
		source >> obj_deserialized;
	} catch (shift::out_of_range& e) {
		std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
	}

	example::debug_print(std::cout, sink_instance);
	obj.print("\noriginal:");
	obj_deserialized.print("\ndeserialized:");
}


CODE_SAMPLE(free_serialize_function, "free function for serialization", "struct", "free function") {

	typedef shift::sink<little_endian, shift::vector > sink_type;
	sink_type sink_inst;
	sink_inst.clear();

	test_struct obj(129, 3, 4, true, 3.14);
	try {
		serialize(sink_inst, obj);
	} catch (shift::out_of_range& e) {
		std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
	}

	typedef source<little_endian> source_type;
	source_type source_inst(sink_inst.buffer(), sink_inst.size());

	test_struct obj_deserialized;
	try {
		serialize(source_inst, obj_deserialized);
	} catch (shift::out_of_range& e) {
		std::cout << e.what() << " " << e.file() << " " << e.line() << std::endl;
	}

	example::debug_print(std::cout, sink_inst);
	obj.print("\noriginal:");
	obj_deserialized.print("\ndeserialized:");
}

} // anonymous namespace
