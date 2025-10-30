#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cassert>
#include <stdint.h>
#include <algorithm>



std::vector<std::string> TOKENS = {
	"ret",
	"{",
	"}",
	";",
	"fun",
};
enum TOKTYPES {
	RET = 0,
	BRACEL,
	BRACER,
	SEMI,
	FUN,
	INDENT,
	LIT,
	LABEL
};

struct Token {
	TOKTYPES type;
	std::string value;
	Token(TOKTYPES type, const std::string& value) : type(type), value(value) {}
};



int main (int argc, char *argv[])
{
	// assert(argc == 3);

	std::string outpath = argc > 1 ? argv[2] : "C:/Projects/boglang/code/hello.asm";
	
	std::ifstream in(argc > 1 ? argv[1] : "C:/Projects/boglang/code/hello.glp");
	std::string content( (std::istreambuf_iterator<char>(in) ),
    (std::istreambuf_iterator<char>() ) );

	std::ofstream out(outpath);
	assert(in);
	assert(out);
	out.flush();
	
	char c;

	std::string buf = "";
	std::vector<Token> tokens;

	for (int s = 0; s != content.length(); s++) {
		c = content[s];
		if (std::isalpha(c)) {
			bool matching = true;
			int i = 0;
			int j;
			for (i = 0; i != TOKENS.size(); i++) {
				matching = true;
				for (j = 0; j != TOKENS[i].length(); j++) {
					char t = TOKENS[i][j];
					c = content[s + j];
					if (c == t) {
						// std::cout << c << " is still matching " << TOKENS[i] << "\n";
					}
					else {
						matching = false;
						// std::cout << "does not match " << c << " " << t << "\n";;
						break;
					}
				}
				if (matching) {
					tokens.emplace_back(static_cast<TOKTYPES>(i), "");
					// std::cout << "It's a match! " << c << " " << TOKENS[i] << "\n";
					if (s >= content.length()) goto escape;
					break;
				}
			}
			if (!matching) {
				buf = "";
				while (s < content.length() && std::isalpha(c)) {
					buf += c;
					s++;
					c = content[s];
				}
				tokens.emplace_back(static_cast<TOKTYPES>(LABEL), buf);
				if (s >= content.length()) goto escape;
			}
			else {
				s += j;
			}
		}
		else if (std::isdigit(c)) {
			buf = "";
			while (s < content.length() && std::isdigit(c)) {
				buf += c;
				s++;
				c = content[s];
			}
			tokens.emplace_back(static_cast<TOKTYPES>(LIT), buf);
			if (s >= content.length()) goto escape;
		}
	}
	escape:
	in.close();

	for (auto& tok : tokens) {
		std::cout << "token of type " << tok.type << " with value " << tok.value << std::endl;
	}
	
	std::string ASM = "\
global _start\n";

	for (int i = 0; i != tokens.size(); i++) {
		Token& tok = tokens[i];
		switch (tok.type) {
		case FUN:
			ASM += tok.value + " " + tokens[i+1].value + ":\n";
			break;
		case RET:
			ASM += "mov eax, " + tokens[i + 1].value + "\n";
			ASM += "ret\n";
			break;
		default:
			break;
		}
	}

	out << ASM << std::endl;
	
	out.close();

	// this assembles and links the resulting .asm file
	std::string bpath = outpath.substr(0, outpath.length() - 4);
	std::string ascmd = "C:\\msys64\\mingw32\\bin\\nasm.exe -f win32 ";
	std::string opath = bpath + ".o";
	ascmd += outpath + " -o " + opath;
	std::system(ascmd.c_str());

	std::string ldcmd = "C:\\msys64\\mingw32\\bin\\ld.exe "
	+ opath + " -o " + bpath + ".exe";
	std::system(ldcmd.c_str());

	return EXIT_SUCCESS;
}