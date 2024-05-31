#include "stdafx.h"
#include "json.h"

using namespace http;

json::value::value()
	: m_value(std::make_unique<_Null>())
	, m_kind(json::value::Null)
{
}

json::value::value(int32_t _val)
	: m_value(std::make_unique<_Number>(std::move(_val)))
	, m_kind(json::value::Number)
{
}

json::value::value(uint32_t _val)
	: m_value(std::make_unique<_Number>(std::move(_val)))
	, m_kind(json::value::Number)
{
}

json::value::value(int64_t _val)
	: m_value(std::make_unique<_Number>(std::move(_val)))
	, m_kind(json::value::Number)
{
}

json::value::value(uint64_t _val)
	: m_value(std::make_unique<_Number>(std::move(_val)))
	, m_kind(json::value::Number)
{
}

json::value::value(double _val)
	: m_value(std::make_unique<_Number>(std::move(_val)))
	, m_kind(json::value::Number)
{
}

json::value::value(bool _val)
	: m_value(std::make_unique<_Boolean>(std::move(_val)))
	, m_kind(json::value::Boolean)
{
}

json::value::value(const char * _val)
	: m_value(std::make_unique<_String>(std::move(_val)))
	, m_kind(json::value::String)
{
}

json::value::value(details::string_t _val)
	: m_value(std::make_unique<_String>(std::move(_val)))
	, m_kind(json::value::String)
{
}

json::value::value(const json::value & other)
	: m_value(other.m_value->_copy_value())
	, m_kind(other.m_kind)
{
}

json::value::value(json::value && other) noexcept
	: m_value(std::move(other.m_value))
	, m_kind(other.m_kind)
{
}

json::value & json::value::operator=(const json::value & other)
{
	if (this != &other)
	{
		m_value = std::unique_ptr<json::_Value>(other.m_value->_copy_value());
		m_kind = other.m_kind;
	}
	return *this;
}

json::value & json::value::operator=(json::value && other) noexcept
{
	if (this != &other)
	{
		m_value.swap(other.m_value);
		m_kind = other.m_kind;
	}
	return *this;
}

json::value & json::value::operator[](const details::string_t & _key)
{
	return m_value->index(_key);
}

json::value & json::value::operator[](std::size_t inx)
{
	return m_value->index(inx);
}

json::value json::value::null()
{
	std::unique_ptr<json::_Value> ptr = std::make_unique<json::_Null>();
	return json::value(std::move(ptr));
}

json::value json::value::boolean(bool value)
{
	std::unique_ptr<json::_Value> ptr = std::make_unique<json::_Boolean>(std::move(value));
	return json::value(std::move(ptr));
}

json::value json::value::number(double value)
{
	std::unique_ptr<json::_Value> ptr = std::make_unique<json::_Number>(std::move(value));
	return json::value(std::move(ptr));
}

json::value json::value::number(int32_t value)
{
	std::unique_ptr<json::_Value> ptr = std::make_unique<json::_Number>(std::move(value));
	return json::value(std::move(ptr));
}

json::value json::value::number(uint32_t value)
{
	std::unique_ptr<json::_Value> ptr = std::make_unique<json::_Number>(std::move(value));
	return json::value(std::move(ptr));
}

json::value json::value::number(int64_t value)
{
	std::unique_ptr<json::_Value> ptr = std::make_unique<json::_Number>(std::move(value));
	return json::value(std::move(ptr));
}

json::value json::value::number(uint64_t value)
{
	std::unique_ptr<json::_Value> ptr = std::make_unique<json::_Number>(std::move(value));
	return json::value(std::move(ptr));
}

json::value json::value::string()
{
	std::unique_ptr<json::_Value> ptr = std::make_unique<json::_String>();
	return json::value(std::move(ptr));
}

json::value json::value::string(details::string_t value)
{
	std::unique_ptr<json::_Value> ptr = std::make_unique<json::_String>(std::move(value));
	return json::value(std::move(ptr));
}

json::value json::value::array()
{
	std::unique_ptr<json::_Value> ptr = std::make_unique<json::_Array>();
	return json::value(std::move(ptr));
}

json::value json::value::array(std::size_t size)
{
	std::unique_ptr<json::_Value> ptr = std::make_unique<json::_Array>(size);
	return json::value(std::move(ptr));
}

json::value json::value::array(std::vector<json::value> elements)
{
	std::unique_ptr<json::_Value> ptr = std::make_unique<json::_Array>(std::move(elements));
	return json::value(std::move(ptr));
}

json::value json::value::object()
{
	std::unique_ptr<json::_Value> ptr = std::make_unique<json::_Object>();
	return json::value(std::move(ptr));
}

json::value json::value::object(std::vector<std::pair<details::string_t, json::value>> fields)
{
	std::unique_ptr<json::_Value> ptr = std::make_unique<json::_Object>(std::move(fields));
	return json::value(std::move(ptr));
}

json::value json::value::parse(details::string_t doc)
{
	return json::value();
}

double json::value::as_double() const
{
	return m_value->as_double();
}

int json::value::as_integer() const
{
	return m_value->as_integer();
}

const json::number & json::value::as_number() const
{
	return m_value->as_number();
}

bool json::value::as_bool() const
{
	return m_value->as_bool();
}

details::string_t json::value::as_string() const
{
	return m_value->as_string();
}

json::array & json::value::as_array()
{
	return m_value->as_array();
}

const json::array & json::value::as_array() const
{
	return m_value->as_array();
}

json::object & json::value::as_object()
{
	return m_value->as_object();
}

const json::object & json::value::as_object() const
{
	return m_value->as_object();
}

details::string_t json::value::to_string() const
{
	return m_value->to_string();
}

json::value::value_type json::value::valuetype() const { return m_value->valuetype(); }
