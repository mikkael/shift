
//          Copyright Michael Mehling 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SHIFT_EXCEPTION_HPP_
#define SHIFT_EXCEPTION_HPP_

#include <exception>
#include <utility>
#include <string>
#include <sstream>

namespace shift {

class exception;

namespace detail {

struct exception_origin {
	exception_origin(                                   ) : line(    ), file(NULL){}
	exception_origin(unsigned int line, const char* file) : line(line), file(file){}
	unsigned int line;
	const char* file;
};

template<typename ExceptionType>
ExceptionType add_origin(ExceptionType e, detail::exception_origin origin) {
	e.origin_ = origin;
	return e;
}

} // detail

#define SHIFT_THROW( ex ) throw detail::add_origin(ex, detail::exception_origin(__LINE__, __FILE__));

class exception : public std::exception {
public:

	exception(const char*        what) : what_(what) {}
	exception(const std::string& what) : what_(what) {}

	virtual ~exception()             throw() {}

	virtual const char* what() const throw() { return what_.c_str(); }
	const char*         file() const         { return origin_.file ; }
	unsigned int        line() const         { return origin_.line ; }

private:

	template<typename ExceptionType>
	friend ExceptionType detail::add_origin(ExceptionType e, detail::exception_origin origin);

	const std::string        what_;
	detail::exception_origin origin_;
};

class not_initialized : public exception {
public:
	not_initialized(const std::string& str = "") : exception(str) {}
};

class out_of_range : public exception {
public:

	typedef std::pair<unsigned, unsigned> range_type;

	out_of_range(unsigned value, unsigned range_begin, unsigned range_end)
	: exception(generate_what_string(value, range_begin, range_end))
	, value_   (value)
	, range_   (range_begin, range_end)
	{}

	unsigned   value() const { return value_; }
	range_type range() const { return range_; }

private:

	std::string generate_what_string(unsigned value, unsigned range_begin, unsigned range_end) {
		std::stringstream sstr;
		sstr << "value '" << value << "' out of range [" << range_begin << ", " << range_end << ")";
		return sstr.str();
	}

	const unsigned   value_;
	const range_type range_;
};

#define SHIFT_THROW_ON_INDEX_OUT_OF_RANGE(index, range_begin, range_end)\
	if (index < range_begin || index >= range_end) SHIFT_THROW( out_of_range(index, range_begin, range_end) )

class malformed_var_uint : public exception {
public:
	malformed_var_uint(const std::string& str="") : exception(""){}
};

} // shift

#endif /* SHIFT_EXCEPTION_HPP_ */
