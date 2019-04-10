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

	Error cout_(bool& t)
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

	Error cout_(uint64_t& t)
	{
		out_<<t<<Separator;
		return Error::NoError;
	}

	template <class T>
	Error process(T&& t)
	{
		cout_(std::forward<T>(t));
		return Error::NoError;
	}

	template <class T, class... ArgsT>
	Error process(T&& t, ArgsT&&... args)
	{
		Error error = cout_(std::forward<T>(val));
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
	
	Error cin_(bool& t)
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

	Error cin_(uint64_t& t)
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
		return cin_(std::forward<T>(t));
	}

	template <class T, class... ArgsT>
	Error process(T&& t, ArgsT&&... args)
	{
		Error error = cin_(std::forward<T>(t));
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
