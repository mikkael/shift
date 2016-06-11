#include <catch.hpp>

#include <iterator>
#include <cstdlib>
#include <iostream>

#include <shift/detail/utility.hpp>
#include <shift/types/cstdint.hpp>

namespace test { namespace {

template<typename T>
T rand() {
	return (static_cast<double>(std::rand()) / std::numeric_limits<int>::max()) * std::numeric_limits<T>::max();
}

template<typename T>
void test_reverse_copy_vector(unsigned int N) {
	std::srand(std::time(0));

	typedef std::vector<T> container_type;
	container_type v_original;
	std::generate_n(std::back_inserter(v_original), N, rand<T>);

	container_type v_copy;
	shift::detail::reverse_copy(v_original.begin(), v_original.end(), std::back_inserter(v_copy));

	REQUIRE(v_original.size() == v_copy.size());

	for (unsigned i=0, n=v_original.size(); i<n; ++i)
		CHECK(v_original[i] == v_copy[n-1-i]);
}

TEST_CASE( "reverse_copy: vector<int>"
         , "[reverse_copy]" )
{
	test_reverse_copy_vector<shift::uint8_t>(0);
	test_reverse_copy_vector<shift::uint8_t>(1);
	test_reverse_copy_vector<shift::uint8_t>(13);
	test_reverse_copy_vector<shift::uint8_t>(1024);

	test_reverse_copy_vector<shift::uint16_t>(0);
	test_reverse_copy_vector<shift::uint16_t>(1);
	test_reverse_copy_vector<shift::uint16_t>(13);
	test_reverse_copy_vector<shift::uint16_t>(1024);

	test_reverse_copy_vector<int>(0);
	test_reverse_copy_vector<int>(1);
	test_reverse_copy_vector<int>(13);
	test_reverse_copy_vector<int>(1024);

	test_reverse_copy_vector<long>(0);
	test_reverse_copy_vector<long>(1);
	test_reverse_copy_vector<long>(13);
	test_reverse_copy_vector<long>(1024);

	test_reverse_copy_vector<float>(0);
	test_reverse_copy_vector<float>(1);
	test_reverse_copy_vector<float>(13);
	test_reverse_copy_vector<float>(1024);

	test_reverse_copy_vector<double>(0);
	test_reverse_copy_vector<double>(1);
	test_reverse_copy_vector<double>(13);
	test_reverse_copy_vector<double>(1024);
}

TEST_CASE( "reverse_copy: double[]"
         , "[reverse_copy]" )
{
	const double original[] = {11, 22, 33, 44, 55, 66};
	const unsigned n = sizeof(original)/sizeof(double);
	double copy[n];

	shift::detail::reverse_copy(original+0, original+n, copy+0);

	for (unsigned i=0; i<n; ++i)
		CHECK(original[i] == copy[n-1-i]);
}

}} // test
