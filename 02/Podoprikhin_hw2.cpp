#include <iostream>
#include <sstream>
#include <string>
#include <exception>

enum token_value
{
    ERR,
    NUM,
    END,
    ADD,
    SUB,
    MUL,
    DIV
};

struct token 
{      
    token_value T;
	char operation;
	int64_t number = 0;

    token(token_value T_, char operation_ = 0, int64_t number_ = 0) : T(T_), operation(operation_), number(number_) {}
};

token curr_tok(std::istream* input)
{
	char c;
	while(input->get(c))
	{
		if(c == ' ')
			continue;
		else if(c == '*')
			return token(MUL, '*');
		else if(c == '/')
			return token(DIV, '/');
		else if(c == '+')
			return token(ADD, '+');
		else if(c == '-')
			return token(SUB, '-');
		else if(c >= '0' && c <= '9')
			return token(NUM, c, c - '0');
		else
			return token(ERR, c);
	}
	return token(END);
}

int64_t prim(std::istream* input)
{
	token c = curr_tok(input);
	if(c.T == ADD)
		return prim(input);
	else if(c.T == SUB)
		return -prim(input);
	else if(c.T == NUM)
		return c.number;
	else if(c.T == ERR)
		throw std::invalid_argument("error");	 
	else if(c.T == END)
		throw std::logic_error("error");
}

int64_t term(std::istream* input) 
{
    int64_t res = prim(input);

    while (true) 
	{
        token c = curr_tok(input);
        switch(c.T) 
		{
            case MUL:
                res *= prim(input);
                break;
            case DIV:
			{
                int64_t d = prim(input);
                if (d)
				{
                    res /= d;
					break;
				}
				throw std::runtime_error("error");
            }
            case ERR:
                throw std::invalid_argument("error");
            default:
                input->putback(c.operation);
                return res;
        }
    }
}

/*int64_t term(std::istream* input)
{
	int64_t res = prim(input);
	while(true)
	{
		token c = curr_tok(input);
		if(c.T == MUL)
		{
			res *= prim(input);
			break;
		}
		else if(c.T == DIV)
		{
			int64_t d = prim(input);
			if (d)
			{
				res /= d;
				break;
			}
			throw std::runtime_error("error");
		}
		else if(c.T == ERR)
		{
			throw std::invalid_argument("error");
			break;
		}
		else 
		{
			input->putback(c.operation);
			return res;
		}
	}
}*/

int64_t expr(std::istream* input) 
{
    int64_t res = term(input);

    while (true) 
	{
        token c = curr_tok(input);
        switch (c.T) 
		{
            case ADD:
                res += term(input);
                break;
            case SUB:
                res -= term(input);
                break;
            case ERR:
                throw std::invalid_argument("error");
            default:
                return res;
        }
    }
}

/*int64_t expr(std::istream* input)
{
	int64_t res = term(input);
	while(true)
	{
		token c = curr_tok(input);
		if(c.T == ADD)
		{
			res += term(input);
			break;
		}
		else if(c.T == SUB)
		{
			res -= term(input);
			break;
		}
		else if(c.T == ERR)
		{
			throw std::invalid_argument("error");
			break;
		}
		else
		{
			return res;
		}
	}
}*/

int main(int argc, char* argv[]) 
{
	if (argc != 2) 
	{
		std::cout << "error" << std::endl;
		return 1;
	}
	auto text = new std::istringstream(argv[1]);
	try
	{
		std::cout << expr(text) << std::endl;
	}

	catch (std::runtime_error& e)
	{
		std::cout << "error" << std::endl;
		return 1;
	}
	catch  (std::logic_error& e)
	{
		std::cout << "error" << std::endl;
		return 1;
	}
	return 0;
}


