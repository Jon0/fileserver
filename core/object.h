#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace core {

class value_base;

enum class value_type {
	null_type,
	bool_type,
	int_type,
	array_type,
	string_type,
	record_type
};


class object {
public:
	using array = std::vector<object>;
	using record = std::unordered_map<std::string, object>;

	object();
	object(bool data);
	object(int data);
	object(const array &data);
	object(const char *data);
	object(const std::string &data);
	object(const record &data);

	bool is_null() const;
	value_type type() const;
	std::string str() const;

	std::string as_string() const;
	record as_record() const;
	const object &operator[](const std::string &key) const;

	static const object static_null;

private:
	std::shared_ptr<value_base> value_ptr;
};

/**
 * implement defualts here so type objects
 * don't need to reimplement the same thing
 */
class value_base {
public:
	virtual value_type type() const = 0;
	virtual std::string str() const = 0;

	virtual int as_int() const = 0;
	virtual std::string as_string() const = 0;
	virtual object::record as_record() const = 0;
	virtual const object &operator[](const std::string &key) const = 0;
};


template<value_type T>
class value;

template<>
class value<value_type::null_type> : public value_base {
public:
};

template<>
class value<value_type::bool_type> : public value_base {
public:
	value(bool v);

	value_type type() const override;
	std::string str() const override;

	int as_int() const override;
	std::string as_string() const override;
	object::record as_record() const override;
	const object &operator[](const std::string &key) const override;

	const bool stored_value;
};

template<>
class value<value_type::int_type> : public value_base {
public:
	value(int v);

	value_type type() const override;
	std::string str() const override;

	int as_int() const override;
	std::string as_string() const override;
	object::record as_record() const override;
	const object &operator[](const std::string &key) const override;

	const int stored_value;
};


template<>
class value<value_type::array_type> : public value_base {
public:
	value(const object::array &v);

	value_type type() const override;
	std::string str() const override;

	int as_int() const override;
	std::string as_string() const override;
	object::record as_record() const override;
	const object &operator[](const std::string &key) const override;

	const object::array stored_value;
};

template<>
class value<value_type::string_type> : public value_base {
public:
	value(const std::string &v);

	value_type type() const override;
	std::string str() const override;

	int as_int() const override;
	std::string as_string() const override;
	object::record as_record() const override;
	const object &operator[](const std::string &key) const override;

	const std::string stored_value;
};

template<>
class value<value_type::record_type> : public value_base {
public:
	value(const object::record &v);

	value_type type() const override;
	std::string str() const override;

	int as_int() const override;
	std::string as_string() const override;
	object::record as_record() const override;
	const object &operator[](const std::string &key) const override;

	const object::record stored_value;
};

}
