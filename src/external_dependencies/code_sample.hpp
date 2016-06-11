/*
 *  CODE-SAMPLE
 *  Copyright (c) 2016 michael mehling
 *  distributed under the Boost Sofware License 1.0 (see "LICENSE_1_0.txt")
 */

#ifndef CODE_SAMPLE_HPP_
#define CODE_SAMPLE_HPP_

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <iterator>

namespace code_sample {
namespace detail { 

typedef void (*example_function_ptr) ();

template<typename FordwardIteratorType>
struct repeated {
	repeated(FordwardIteratorType begin, FordwardIteratorType end, const std::string& delimiter) :
	begin(begin), end(end), delimiter(delimiter) {}

	FordwardIteratorType begin;
	FordwardIteratorType end;
	const std::string&   delimiter;
};

template<typename FordwardIteratorType>
std::ostream& operator<<(std::ostream& stream, const repeated<FordwardIteratorType>& repeated_) {
	for (FordwardIteratorType it = repeated_.begin; it != repeated_.end;) {
		stream << *it;
		if (++it != repeated_.end) stream << repeated_.delimiter;
	}
	return stream;
}

struct example {

	struct invoker {
		void operator()(example& e) { e.invoke(); }
	};

	template<typename MatcherType>
	struct conditional_invoker {
		conditional_invoker(MatcherType matcher) : matcher(matcher) {}
		void operator()(example& e) {
			if (matcher(e))
				e.invoke();
		}
		MatcherType matcher;
	};

	struct name_matcher {
		name_matcher(const char* name) : name(name) {}
		bool operator()(const example& e) { return strcmp(e.name, name) == 0; }
		const char* const name;
	};

	struct tag_matcher{
		tag_matcher(const char* tag): tag(tag) {}
		bool operator()(const example& e) {
			if (e.tags.first == NULL || e.tags.second == NULL)
				return false;
			for (const char** t=e.tags.first; t!=e.tags.second; ++t) {
				if (strcmp(*t, tag) ==0)
					return true;
			}
			return false;
		}
		const char* const tag;
	};

	example(const char* name, const char* description, std::pair<const char**, const char**> tags, const char* file, unsigned int line, example_function_ptr f)
	: name(name), description(description), tags(tags), file(file), line(line), f(f), was_invoked(false) {}

	const char*                           name;
	const char*                           description;
	std::pair<const char**, const char**> tags;
	const char*                           file;
	unsigned int                          line;
	example_function_ptr                  f;
	bool                                  was_invoked;

	void invoke()  {
		if (was_invoked)
			return;
		was_invoked = true;
		std::cout << "\n\n"
		          << ">>> " << name        << "\n"
		          << ">>> " << description << "\n"
		          << ">>> " << repeated<const char**>(tags.first, tags.second, ", ") << "\n"
		          << ">>> " << file << ", line  " << line << "\n" << std::endl;
		(*f)();
	}
};

class registry {
public:
	typedef std::vector<example>     container_type;
	typedef std::vector<std::string> arguments_type;
	
	static void register_example(const example& ex) {
		registry_instance().push_back( ex );
	}
	
	static void run_all() {
		container_type& reg = registry_instance();
		std::for_each(reg.begin(), reg.end(), example::invoker());
	}

	static void invoke_by_name(const arguments_type& names) {
		invoke<example::name_matcher>(names);
	}

	static void invoke_by_tag(const arguments_type& tags) {
		invoke<example::tag_matcher>(tags);
	}

private:
	registry(){}

	inline static container_type& registry_instance() {
		static container_type reg;
		return reg;
	}

	template<typename MatcherType>
	static void invoke(const arguments_type& args) {
		std::for_each(args.begin(), args.end(), conditional_invoker<MatcherType>(registry_instance()));
	}

	template<typename MatcherType>
	struct conditional_invoker {
		conditional_invoker(container_type& reg) : reg(reg) {}
		void operator()(const std::string& arg){
			std::for_each(reg.begin(), reg.end(), example::conditional_invoker<MatcherType>(arg.c_str()));
		}
		container_type& reg;
	};
};

class runner {
public:

	static void run_examples(const int argc, const char* argv[]) {

		typedef std::vector<std::string> arg_vector;
		arg_vector args = to_string_vector(argc, argv);

		arg_vector names = get_args(args, "-e");
		arg_vector tags  = get_args(args, "-t");

		if (names.empty() && tags.empty()) {
			registry::run_all();
		} else {
			registry::invoke_by_name(names);
			registry::invoke_by_tag (tags );
		}
	}

private:
	runner(){}

	static std::vector<std::string> to_string_vector(const int argc, const char * argv[]) {
		std::vector<std::string> result;
		for (int i=0; i<argc; ++i) result.push_back(argv[i]);
		return result;
	}

	static std::vector<std::string> get_args(const std::vector<std::string>& args, const std::string& cmd) {
		typedef std::vector<std::string> arg_vector;
		std::vector<std::string> result;
		arg_vector::const_iterator it = std::find(args.begin(), args.end(), cmd);

		if (it == args.end()) {
			return result;
		} else {
			do {
				if(++it == args.end())
					break;
				arg_vector::const_iterator it_args_end = std::find_if(it, args.end(), first_char_matcher('-') );
				std::copy(it, it_args_end, std::back_inserter(result));
				it = std::find(it_args_end, args.end(), cmd);
			} while (it != args.end());
		}
		return result;
	}

	struct first_char_matcher {
		first_char_matcher(char c) : c(c){}
		bool operator()(const std::string& str) { return (!str.empty()) && (str[0] == c); }
		char c;
	};
};

template<typename T>
struct registrar {
	registrar(const char* name, const char* description, std::pair<const char**, const char**> tags, const char* file, const unsigned int line) {
		registry::register_example( example(name, description, tags, file, line, T::run) );
	}
};

}} // namespace code_sample::detail

#define INTERNAL_CODE_SAMPLE_CONCAT_X(a, b)                        a ## b
#define INTERNAL_CODE_SAMPLE_CONCAT(a, b)                          INTERNAL_CODE_SAMPLE_CONCAT_X(a, b)
#define INTERNAL_CODE_SAMPLE_APPEND_LINE(a)                        INTERNAL_CODE_SAMPLE_CONCAT( INTERNAL_CODE_SAMPLE_CONCAT(a, __), __LINE__)
#define INTERNAL_CODE_SAMPLE_CLASS_NAME(name)                      INTERNAL_CODE_SAMPLE_APPEND_LINE( INTERNAL_CODE_SAMPLE_CONCAT( INTERNAL_CODE_SAMPLE_CONCAT(name, __), example) )
#define INTERNAL_CODE_SAMPLE_REGISTRAR_INSTANCE(name, description) INTERNAL_CODE_SAMPLE_CONCAT( INTERNAL_CODE_SAMPLE_CONCAT( INTERNAL_CODE_SAMPLE_CLASS_NAME(name), __), reg_instance)( #name, description, INTERNAL_CODE_SAMPLE_CLASS_NAME(name)::tags(), __FILE__, __LINE__ )
#define INTERNAL_CODE_SAMPLE_RUN_SIGNATURE(Name)                   void INTERNAL_CODE_SAMPLE_CLASS_NAME(Name)::run()

#define INTERNAL_CODE_SAMPLE_MAIN()\
	int main(int argc, const char* argv[])\
	{\
		code_sample::detail::runner::run_examples(argc, argv);\
		return 0;\
	}\

#define CODE_SAMPLE(Name, Description, ...)\
	struct INTERNAL_CODE_SAMPLE_CLASS_NAME(Name)\
	{\
		static void run();\
		static std::pair<const char**, const char**> tags(){\
			static const char* tags[] = {\
				__VA_ARGS__ \
			};\
			return std::make_pair(tags, tags + sizeof(tags)/sizeof(char*));\
		}\
	};\
	code_sample::detail::registrar< INTERNAL_CODE_SAMPLE_CLASS_NAME(Name) > INTERNAL_CODE_SAMPLE_REGISTRAR_INSTANCE(Name, Description);\
	INTERNAL_CODE_SAMPLE_RUN_SIGNATURE(Name)\

#ifdef CODE_SAMPLE_DEFINE_MAIN
	INTERNAL_CODE_SAMPLE_MAIN()
#endif

#endif /* CODE_SAMPLE_HPP_ */
