#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace core {

enum class value_type {
	null_type,
	bool_type,
	int_type,
	array_type,
	string_type,
	record_type
};

class value_base {
public:
	value_base(const value_type &t);

	value_type type;


};

class object {
public:
	object(const std::string &data);

	std::string str() const;

	std::string data;
	std::shared_ptr<value_base> value;

};

template<value_type T>
struct value;

template<>
struct value<value_type::null_type> {};

template<>
struct value<value_type::int_type> {
	int v;
};

template<>
struct value<value_type::string_type> {
	std::string v;
};

template<>
struct value<value_type::record_type> {
	std::unordered_map<std::string, object> v;
};

}
