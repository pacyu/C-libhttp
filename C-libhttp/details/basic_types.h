#pragma once

#include <xtr1common>
#include <xstring>
#include <vector>

namespace details
{
	class string_t : public std::string
	{
	public:
		string_t()
			: std::string()
		{
		}
		string_t(char * const _Ptr)
			: std::string(_Ptr)
		{
		}
		string_t(const char * const _Ptr)
			: std::string(_Ptr)
		{
		}
		string_t(std::string && _Right)
			: std::string(std::move(_Right))
		{
		}
		string_t(const std::string & _Right)
			: std::string(_Right)
		{
		}
		string_t(string_t && _Right) noexcept
			: std::string(static_cast<std::string>(std::move(_Right)))
		{
		}
		string_t(const string_t & _Right)
			: std::string(static_cast<std::string>(_Right))
		{
		}

		string_t operator=(const char *const _Ptr)
		{
			return static_cast<string_t>(assign(_Ptr));
		}

		string_t operator=(const std::string & _Right)
		{
			return static_cast<string_t>(assign(_Right.c_str()));
		}

		string_t & operator=(const string_t & _Right)
		{// assign _Right
			if (this != _STD addressof(_Right))
			{	// different, assign it
#pragma warning(push)
#pragma warning(disable: 4127)	// conditional expression is constant
				if (_Alty_traits::propagate_on_container_copy_assignment::value
					&& this->_Getal() != _Right._Getal())
				{	// change allocator before copying
					_Tidy_deallocate();
				}
#pragma warning(pop)

				this->_Copy_alloc(_Right._Getal());

				auto& _Right_data = _Right._Get_data();
				assign(_Right_data._Myptr(), _Right_data._Mysize);
			}
			return *this;
		}

		std::vector<string_t> split(const char _ch)
		{
			std::vector<string_t> _Array_str;
			std::string _My_str;
			auto _My_data = this->_Get_data();
			for (std::size_t i = 0; i < size(); i++)
			{
				char _My_ch = _My_data._Myptr()[i];
				if (_My_ch == _ch)
				{
					_Array_str.push_back(_My_str);
					_My_str.clear();
				}
				else
				{
					_My_str += _My_ch;
				}
			}
			_Array_str.push_back(_My_str);
			return _Array_str;
		}
	};
}
