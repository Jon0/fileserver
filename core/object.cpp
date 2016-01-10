#include "object.h"

namespace core {


const object object::static_null = object();


object::object()
 	:
	value_ptr(nullptr) {
}


object::object(bool data)
    :
    value_ptr(std::make_shared<value<value_type::bool_type>>(data)) {
}


object::object(int data)
    :
    value_ptr(std::make_shared<value<value_type::int_type>>(data)) {
}


object::object(const char *data)
	:
	value_ptr(std::make_shared<value<value_type::string_type>>(data)) {
}


object::object(const std::string &data)
	:
	value_ptr(std::make_shared<value<value_type::string_type>>(data)) {
}


object::object(const record &data)
 	:
	value_ptr(std::make_shared<value<value_type::record_type>>(data)) {
}


value_type object::type() const {
	if (value_ptr) {
		return value_ptr->type();
	}
	return value_type::null_type;
}

std::string object::str() const {
	if (value_ptr) {
		return value_ptr->str();
	}
	return "null";
}


std::string object::as_string() const {
	if (value_ptr) {
		return value_ptr->as_string();
	}
	return "";
}


const object &object::operator[](const std::string &key) const {
	if (value_ptr) {
		return (*value_ptr)[key];
	}
	return *this;
}


value<value_type::bool_type>::value(bool v)
	:
	stored_value(v) {
}


value_type value<value_type::bool_type>::type() const {
	return value_type::bool_type;
}


std::string value<value_type::bool_type>::str() const {
	return stored_value ? "true" : "false";
}


int value<value_type::bool_type>::as_int() const {
    return 0;
}


std::string value<value_type::bool_type>::as_string() const {
	return "";
}


const object &value<value_type::bool_type>::operator[](const std::string &key) const {
	return object::static_null;
}


value<value_type::int_type>::value(int v)
	:
	stored_value(v) {
}


value_type value<value_type::int_type>::type() const {
	return value_type::int_type;
}


std::string value<value_type::int_type>::str() const {
	return std::to_string(stored_value);
}


int value<value_type::int_type>::as_int() const {
    return stored_value;
}


std::string value<value_type::int_type>::as_string() const {
	return "";
}


const object &value<value_type::int_type>::operator[](const std::string &key) const {
	return object::static_null;
}


value<value_type::array_type>::value(const object::array &v)
	:
	stored_value(v) {
}


value_type value<value_type::array_type>::type() const {
	return value_type::array_type;
}


std::string value<value_type::array_type>::str() const {
    std::string result = "[";
    bool first = true;
    for (auto &e : stored_value) {
        if (first) {
            first = false;
        }
        else {
            result += ", ";
        }
        result += e.str();
    }
    result += "]";
    return result;
}


int value<value_type::array_type>::as_int() const {
    return 0;
}


std::string value<value_type::array_type>::as_string() const {
	return "";
}


const object &value<value_type::array_type>::operator[](const std::string &key) const {
	return object::static_null;
}


value<value_type::string_type>::value(const std::string &v)
	:
	stored_value(v) {
}


value_type value<value_type::string_type>::type() const {
	return value_type::string_type;
}


std::string value<value_type::string_type>::str() const {
	return "\"" + stored_value + "\"";
}


int value<value_type::string_type>::as_int() const {
    return 0;
}


std::string value<value_type::string_type>::as_string() const {
	return stored_value;
}


const object &value<value_type::string_type>::operator[](const std::string &key) const {
	return object::static_null;
}


value<value_type::record_type>::value(const object::record &v)
	:
	stored_value(v) {
}


value_type value<value_type::record_type>::type() const {
	return value_type::record_type;
}


std::string value<value_type::record_type>::str() const {
	std::string result = "{";
	bool first = true;
	for (auto &e : stored_value) {
		if (first) {
			first = false;
		}
		else {
			result += ", ";
		}
		result += "{" + e.first + ": " + e.second.str() + "}";
	}
	result += "}";
	return result;
}


int value<value_type::record_type>::as_int() const {
    return 0;
}


std::string value<value_type::record_type>::as_string() const {
	return "";
}

const object &value<value_type::record_type>::operator[](const std::string &key) const {
    if (stored_value.count(key) == 0) {
        return object::static_null;
    }
	return stored_value.at(key);
}


}
