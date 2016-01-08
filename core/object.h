#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace core {

class object;

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
	virtual value_type type() const = 0;
	virtual std::string str() const = 0;

	virtual std::string as_string() const = 0;
	virtual const object &operator[](const std::string &key) const = 0;
};


class object {
public:
	using array = std::vector<object>;
	using record = std::unordered_map<std::string, object>;

	object();
	object(const char *data);
	object(const std::string &data);
	object(const record &data);

	value_type type() const;
	std::string str() const;

	std::string as_string() const;
	const object &operator[](const std::string &key) const;

	std::string data;
	std::shared_ptr<value_base> value_ptr;

	static const object static_null;
};

template<value_type T>
class value;

template<>
class value<value_type::null_type> : public value_base {
public:
};

template<>
class value<value_type::int_type> : public value_base {
public:
	int v;
};

template<>
class value<value_type::string_type> : public value_base {
public:
	value(const std::string &v);

	value_type type() const override;
	std::string str() const override;

	std::string as_string() const override;
	const object &operator[](const std::string &key) const override;

	const std::string stored_value;
};

template<>
class value<value_type::record_type> : public value_base {
public:
	value(const object::record &v);

	value_type type() const override;
	std::string str() const override;

	std::string as_string() const override;
	const object &operator[](const std::string &key) const override;

	const object::record stored_value;
};

}
