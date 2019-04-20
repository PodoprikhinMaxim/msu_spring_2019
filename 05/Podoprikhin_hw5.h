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

	Error toStream(bool t)
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
	}

	template <class T>
	Error process(T&& t)
	{
		toStream(std::forward<T>(t));
		return Error::NoError;
	}

	template <class T, class... ArgsT>
	Error process(T&& t, ArgsT&&... args)
	{
		Error error = toStream(std::forward<T>(t));
		process(std::forward<ArgsT>(args)...);
		return Error::NoError;		
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
	
	Error fromStream(bool& t)
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
	}
	
	template <class T>
	Error process(T&& t)
	{
		return fromStream(std::forward<T>(t));
	}

	template <class T, class... ArgsT>
	Error process(T&& t, ArgsT&&... args)
	{
		Error error = fromStream(std::forward<T>(t));
		if(error == Error::NoError)
		{
			return process(std::forward<ArgsT>(args)...);
		}
		else
		{
			return error;
		}
		
	}
	
};
