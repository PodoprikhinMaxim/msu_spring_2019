#pragma once
#include <iostream>

enum class Error
{
	NoError,
	CorruptedArchive
};

class Serializer
{
	static constexpr char Separator = ' ';
public:
	explicit Serializer(std::ostream& out)
		: out_(out)
	{
	}

	template <class T>
	Error save(T& object)
	{
		return object.serialize(*this);
	}

	template <class... ArgsT>
	Error operator()(ArgsT... args)
	{
		return process(args...);
	}
    
private:
	std::ostream& out_;

	template <class T>
	Error toStream(T t)
	{
		if(std::is_same<T, bool>::value)
		{
			if (t == true)
			{
				out_<<"true"<<Separator;
			}
			else if (t == false)
			{
				out_<<"false"<<Separator;
			}
		}
		else if(std::is_same<T, uint64_t>::value)
		{
			out_<<t<<Separator;
		}
		return Error::NoError;
	}
	/*Error toStream(bool t)
	{
		if (t == true)
		{
			out_<<"true"<<Separator;
		}
		else if (t == false)
		{
			out_<<"false"<<Separator;
		}
		return Error::NoError;
	}

	Error toStream(uint64_t t)
	{
		out_<<t<<Separator;
		return Error::NoError;
	}*/

	template <class T>
	Error process(T&& t)
	{
		return toStream(std::forward<T>(t));
	}

	template <class T, class... ArgsT>
	Error process(T&& t, ArgsT&&... args)
	{
		Error err = toStream(std::forward<T>(t));
		if(err == Error::NoError)
		{
			return process(std::forward<ArgsT>(args)...);
		}
		else
		{
			return err;
		}		
	}
};



class Deserializer
{
    static constexpr char Separator = ' ';
public:
    explicit Deserializer(std::istream& in)
        : in_(in)
    {
    }

    template <class T>
    Error load(T& object)
    {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&... args)
    {
        return process(args...);
    }
    
private:
    	std::istream& in_;
	
	template<class T>
	Error fromStream(T& t)
	{
		if(std::is_same<T, bool>::value)
		{
			std::string str;
			in_>>str;
			if (str == "true")
			{
				t = true;
				return Error::NoError;
			}
			else if (str == "false")
			{
				t = false;
				return Error::NoError;
			}	
		}
		else if(std::is_same<T, uint64_t>::value)
		{
			std::string str;
			uint64_t tmp = 0;
			in_>>str;
			if (str.length() == 0)
			{
				return Error::CorruptedArchive;
			}		
			for (size_t i = 0; i < str.length(); i++)
			{
				if (!(isdigit(str[i])))
				{
					return Error::CorruptedArchive;
				}
				else
				{
					tmp = tmp * 10 + (str[i] - '0');
				}
			}
			t = tmp;
			return Error::NoError;	
		}
		return Error::CorruptedArchive;
	}
	
	/*Error fromStream(bool& t)
	{
		std::string str;
		in_>>str;
		if (str == "true")
		{
			t = true;
			return Error::NoError;
		}
		else if (str == "false")
		{
			t = false;
			return Error::NoError;
		}
		return Error::CorruptedArchive;
	}

	Error fromStream(uint64_t& t)
	{
		std::string str;
		uint64_t tmp = 0;
		in_>>str;
		if (str.length() == 0)
		{
			return Error::CorruptedArchive;
		}
		
		for (size_t i = 0; i < str.length(); i++)
		{
			if (!(isdigit(str[i])))
			{
				return Error::CorruptedArchive;
			}
			else
			{
				tmp = tmp * 10 + (str[i] - '0');
			}
		}
		t = tmp;
		return Error::NoError;
	}*/
	
	template <class T>
	Error process(T&& t)
	{
		return fromStream(std::forward<T>(t));
	}

	template <class T, class... ArgsT>
	Error process(T&& t, ArgsT&&... args)
	{
		Error err = fromStream(std::forward<T>(t));
		if(err == Error::NoError)
		{
			return process(std::forward<ArgsT>(args)...);
		}
		else
		{
			return err;
		}
		
	}
	
};
