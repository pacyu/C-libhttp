#pragma once

#include "details/basic_types.h"
#include <string>
#include <cstdint>
#include <cassert>
#include <vector>
#include <memory>

namespace http
{
	namespace json
	{
		class _Value;
		class _Object;
		class _Array;
		class _String;
		class _Number;
		class _Boolean;
		class _Null;

		class JSON_Parser;

		class number;
		class array;
		class object;

		class value
		{
		public:
			enum value_type
			{
				Number,
				Boolean,
				String,
				Object,
				Array,
				Null
			};

			value();

			value(int32_t _val);

			value(int64_t _val);

			value(uint32_t _val);

			value(uint64_t _val);

			value(double _val);

			explicit value(bool _val);

			value(const char * _val);

			value(details::string_t _val);

			value(const value&);

			value(value&&) noexcept;

			value & operator=(const value&);

			value & operator=(value&&) noexcept;

			value & operator[](const details::string_t & _key);

			value & operator[](std::size_t index);

			static value null();

			static value number(double);

			static value number(int32_t);

			static value number(uint32_t);

			static value number(int64_t);

			static value number(uint64_t);

			static value boolean(bool);

			static value string();

			static value string(details::string_t);

			static value array();

			static value array(std::size_t);

			static value array(std::vector<value> elements);

			static value object();

			static value object(std::vector<std::pair<details::string_t, value>> fields);

			static value parse(details::string_t doc);

			value_type valuetype() const;

			double as_double() const;

			int as_integer() const;

			const json::number & as_number() const;

			bool as_bool() const;

			details::string_t as_string() const;

			json::array & as_array();

			const json::array & as_array() const;

			json::object & as_object();

			const json::object & as_object() const;

			details::string_t to_string() const;

		private:
			friend class _Object;
			friend class _Array;

			void format(details::string_t & str) const
			{
				m_value->format(str);
			}

			explicit value(std::unique_ptr<_Value> _val) : m_value(std::move(_val))
			{
			}

			std::unique_ptr<_Value> m_value;

			value_type m_kind;
		};

		class number
		{
			number(int32_t _val) : m_intval(_val), m_type(_val < 0 ? unsigned_type : signed_type) {}
			number(uint32_t _val) : m_uintval(_val), m_type(unsigned_type) {}
			number(int64_t _val) : m_intval(_val), m_type(_val < 0 ? unsigned_type : signed_type) {}
			number(uint64_t _val) : m_uintval(_val), m_type(unsigned_type) {}
			number(double _val) : m_double(_val), m_type(double_type) {}

		public:
			bool is_integer() const { return m_type != double_type; }

			double to_double() const
			{
				switch (m_type)
				{
				case signed_type: return static_cast<double>(m_intval);
				case unsigned_type: return static_cast<double>(m_uintval);
				case double_type: return m_double;
				}
			}

			int32_t to_int32() const
			{
				switch (m_type)
				{
				case signed_type: return static_cast<int32_t>(m_intval);
				case unsigned_type: return static_cast<int32_t>(m_uintval);
				case double_type: return static_cast<int32_t>(m_double);
				}
			}

			uint32_t to_uint32() const
			{
				switch (m_type)
				{
				case signed_type: return static_cast<uint32_t>(m_intval);
				case unsigned_type: return static_cast<uint32_t>(m_uintval);
				case double_type: return static_cast<uint32_t>(m_double);
				}
			}

			int64_t to_int64() const
			{
				switch (m_type)
				{
				case signed_type: return static_cast<int64_t>(m_intval);
				case unsigned_type: return static_cast<int64_t>(m_uintval);
				case double_type: return static_cast<int64_t>(m_double);
				}
			}

			uint64_t to_uint64() const
			{
				switch (m_type)
				{
				case signed_type: return static_cast<uint64_t>(m_intval);
				case unsigned_type: return static_cast<uint64_t>(m_uintval);
				case double_type: return static_cast<uint64_t>(m_double);
				}
			}

		private:
			union {
				int64_t m_intval;
				uint64_t m_uintval;
				double m_double;
			};

			enum number_type
			{
				signed_type,
				unsigned_type,
				double_type
			} m_type;

			friend class _Number;
		};

		class array
		{
			typedef std::vector<value> m_type;

		public:
			typedef m_type::iterator iterator;
			typedef m_type::const_iterator const_iterator;
			typedef m_type::size_type size_type;

		private:
			array() : m_elements() {}
			array(size_type _size) : m_elements(_size) {}
			array(m_type elements) : m_elements(elements) {}

		public:
			void push(value && _Elem)
			{
				m_elements.push_back(std::move(_Elem));
			}

			void pop()
			{
				m_elements.pop_back();
			}

			void erase(size_type index)
			{
				m_elements.erase(m_elements.begin() + index);
			}

			value & operator[](size_type index)
			{
				return m_elements[index];
			}

			iterator begin()
			{
				return m_elements.begin();
			}

			const_iterator begin() const
			{
				return m_elements.cbegin();
			}

			const_iterator cbegin() const
			{
				return m_elements.cbegin();
			}

			iterator end()
			{
				return m_elements.end();
			}

			const_iterator end() const
			{
				return m_elements.end();
			}

			const_iterator cend() const
			{
				return m_elements.cend();
			}

			bool empty() const
			{
				return m_elements.empty();
			}

			size_type size() const { return m_elements.size(); }

		private:
			friend class _Array;

			m_type m_elements;
		};

		class object {
			typedef std::vector<std::pair<details::string_t, value>> m_type;

		public:
			typedef m_type::iterator iterator;
			typedef m_type::const_iterator const_iterator;
			typedef m_type::size_type size_type;

		private:
			size_type hash(const char * rhs) const
			{
				size_type hash_ = 5381;
				while (*rhs)
					hash_ = ((hash_ << 5) + hash_) + int(*rhs++);
				return hash_ % 20011;
			}

			object()
				: m_elements()
				, ord()
			{
			}

			object(m_type elements)
				: m_elements(std::move(elements))
				, ord()
			{
				for (size_type i = 0; i < m_elements.size(); i++)
				{
					ord[hash(m_elements[i].first.c_str())] = i + 1;
				}
			}

		public:
			value & operator[](const details::string_t rhs)
			{
				size_type hash_ = hash(rhs.c_str());
				if (!ord[hash_])
				{
					m_elements.push_back(std::pair<details::string_t, value>(rhs, value()));
					ord[hash_] = m_elements.size();
				}
				return m_elements[ord[hash_] - 1].second;
			}

			void push(std::pair<details::string_t, value> field)
			{
				this->operator[](field.first) = std::move(field.second);
			}

			void pop(details::string_t _key)
			{
				size_type inx_ = ord[hash(_key.c_str())];
				assert(inx_ > 1);

				for (size_type i = inx_ - 1; i < m_elements.size(); i++)
					ord[hash(m_elements[i].first.c_str())] = i;

				m_elements.erase(m_elements.begin() + (inx_ - 1));
			}

			iterator begin()
			{
				return m_elements.begin();
			}

			const_iterator begin() const
			{
				return m_elements.begin();
			}

			const_iterator cbegin() const
			{
				return m_elements.cbegin();
			}

			iterator end()
			{
				return m_elements.end();
			}

			const_iterator end() const
			{
				return m_elements.end();
			}

			const_iterator cend() const
			{
				return m_elements.cend();
			}

			bool empty() const
			{
				return m_elements.empty();
			}

			size_type size() const { return m_elements.size(); }

		private:
			friend class _Object;

			m_type m_elements;
			size_type ord[20011];
		};

		class _Value
		{
		public:
			virtual std::unique_ptr<_Value> _copy_value() = 0;

			virtual value::value_type valuetype() const { return value::Null; }

			virtual bool is_exist(const details::string_t) const { return false; }

			virtual bool is_integer() const { throw std::exception("not a number"); }

			virtual bool is_double() const { throw std::exception("not a number"); }

			virtual value & index(const details::string_t &) { throw std::exception("not an object"); }
			virtual value & index(json::array::size_type) { throw std::exception("not an array"); }
			virtual const value & cindex(const details::string_t&) const { throw std::exception("not an object"); }
			virtual const value & cindex(json::array::size_type) const { throw std::exception("not an array"); }

			virtual void serialize_impl(details::string_t & str) const { format(str); }

			virtual details::string_t to_string() const
			{
				details::string_t str;
				serialize_impl(str);
				return str;
			}

			virtual bool as_bool() const { throw std::exception("not a boolean"); }
			virtual int as_integer() const { throw std::exception("not a number"); }
			virtual double as_double() const { throw std::exception("not a number"); }
			virtual const number & as_number() { throw std::exception("not a number"); }
			virtual details::string_t as_string() const { throw std::exception("not a string"); }
			virtual array & as_array() { throw std::exception("not an array"); }
			virtual const array & as_array() const { throw std::exception("not an array"); }
			virtual object & as_object() { throw std::exception("not an object"); }
			virtual const object & as_object() const { throw std::exception("not an object"); }

			virtual std::size_t size() const { return 0; }

			virtual ~_Value() {}

		protected:
			_Value() {}

			virtual void format(details::string_t & str) const { str.append("null"); }

		private:
			friend class value;
		};

		class _Null : public _Value
		{
		public:
			virtual std::unique_ptr<_Value> _copy_value() { return std::make_unique<_Null>(); }
		};

		class _Boolean : public _Value
		{
		public:
			_Boolean(bool _val) : m_bool(_val) {}

			virtual std::unique_ptr<_Value> _copy_value() { return std::make_unique<_Boolean>(*this); }

			virtual bool as_bool() const { return m_bool; }

			virtual value::value_type valuetype() const { return value::Boolean; }

		protected:
			virtual void format(details::string_t & str) const { str.append(m_bool ? "true" : "false"); }

		private:
			bool m_bool;
		};

		class _Number : public _Value
		{
		public:
			_Number(int32_t _val) : m_number(_val) {}
			_Number(int64_t _val) : m_number(_val) {}
			_Number(uint32_t _val) : m_number(_val) {}
			_Number(uint64_t _val) : m_number(_val) {}
			_Number(double _val) : m_number(_val) {}

			virtual std::unique_ptr<_Value> _copy_value() { return std::make_unique<_Number>(*this); }

			virtual bool is_integer() const { return m_number.is_integer(); }
			virtual bool is_double() const { return !m_number.is_integer(); }

			virtual int as_integer() const { return m_number.to_int32(); }
			virtual double as_double() const { return m_number.to_double(); }
			virtual const number & as_number() { return m_number; }

			virtual value::value_type valuetype() const { return value::Number; }

		protected:
			virtual void format(details::string_t & str) const { str += std::to_string(m_number.to_int32()); }

		private:
			number m_number;
		};

		class _String : public _Value
		{
		public:
			_String() : m_string() {}
			_String(details::string_t _val) : m_string(_val) {}

			virtual std::unique_ptr<_Value> _copy_value() { return std::make_unique<_String>(*this); }
			virtual details::string_t as_string() const { return m_string; }
			virtual value::value_type valuetype() const { return value::String; }

			virtual void serialize_impl(details::string_t & str) const { serialize_impl_char_type(str); }

		protected:
			virtual void format(details::string_t & stream) const { stream += m_string; }

		private:
			void serialize_impl_char_type(details::string_t & str) const { format(str); }

			details::string_t m_string;
		};

		class _Object : public _Value
		{
		public:
			_Object() : m_object() {}
			_Object(object::m_type fields) : m_object(std::move(fields)) {}

			virtual std::unique_ptr<_Value> _copy_value() { return std::make_unique<_Object>(*this); }

			virtual value & index(const details::string_t & _key) { return m_object[_key]; }

			virtual object & as_object() { return m_object; }
			virtual const object & as_object() const { return m_object; }

			virtual bool is_exist(const details::string_t rhs) const
			{
				if (m_object.ord[m_object.hash(rhs.c_str())] > 0)
					return true;
				return false;
			}

			virtual void serialize_impl(details::string_t & str) const { format(str); }

			virtual value::value_type valuetype() const { return value::Object; }

			std::size_t size() const { return m_object.size(); }

		protected:
			virtual void format(details::string_t & str) const { format_impl(str); }

		private:
			void format_impl(details::string_t & str) const
			{
				str += '{';
				if (!m_object.empty())
				{
					for (auto iter : m_object)
					{
						str += iter.first;
						str += ':';
						iter.second.format(str);
					}
				}
				str += '}';
			}

			object m_object;
		};

		class _Array : public _Value
		{
		public:
			_Array() : m_array() {}
			_Array(array::size_type _size) : m_array(_size) {}
			_Array(array::m_type elements) : m_array(std::move(elements)) {}

			virtual std::unique_ptr<_Value> _copy_value() { return std::make_unique<_Array>(*this); }

			virtual value & index(array::size_type inx) { return m_array[inx]; }

			virtual json::array & as_array() { return m_array; }
			virtual const json::array & as_array() const { return m_array; };

			virtual value::value_type valuetype() const { return value::Array; }

		protected:
			virtual void format(details::string_t & str) const { format_impl(str); }

		private:
			void format_impl(details::string_t & str) const
			{
				str += '[';
				if (!m_array.m_elements.empty())
				{
					for (auto iter : m_array)
					{
						iter.format(str);
						str += ',';
					}
				}
				str += ']';
			}

			array m_array;
		};
	} // namespace json
} // namespace http