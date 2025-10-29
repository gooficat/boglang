#include <cctype>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <fstream>
#include <cassert>
#include <stdint.h>

enum TokenType {
	ret,
	lit_int,
	newl
};

struct Token {
	TokenType type;
	std::string val;
	Token(const TokenType& type, const std::string& val) : type(type), val(val) {}
};

std::vector<Token> tokenize(const std::string& content)
{
	std::vector<Token> tokens;

	std::string buffer = "";
	for (int i = 0; i < content.length(); i++)
	{
		char c = content.at(i);
		if (isalnum(c))
		{
			buffer.push_back(c);
			++i;
			while(i != content.length() && isalnum(content.at(i)))
			{
				buffer = buffer + content.at(i);
				++i;
			}
			TokenType type;
			if (atoi(buffer.c_str())) {
				type = lit_int;
			}
			else {
				type = ret;
			}
			tokens.push_back(
				Token(type, buffer)
			);
		}
		else {
			std::cout << "Not a literal" << std::endl;
		}
		buffer = "";
	}
	return tokens;
}

int main (int argc, char *argv[])
{
	// assert(argc == 3);
	
	std::ifstream in(argc > 1 ? argv[1] : "C:/Projects/boglang/code/hello.glp");
	std::ofstream out(argc > 1 ? argv[2] : "C:/Projects/boglang/code/hello.asm");
	assert(in);
	assert(out);

	std::string content(
		(std::istreambuf_iterator<char>(in)),
		std::istreambuf_iterator<char>()
	);

	out.flush();
	
	std::vector<Token> tokens = tokenize(content);

	std::string buffer = "";
	for (int i = 0; i != tokens.size(); i++) {
		Token& token = tokens[i];
		switch (token.type)
		{
			case ret:
				buffer = "mov eax, " + tokens[i+1].val +
						"\nret\n";
				i++;
				break;
			case newl:
				break;

			default:
				break;
		}

		out << buffer;
		buffer = "";
	}


	in.close();
	out.close();


	return EXIT_SUCCESS;
}