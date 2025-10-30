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


enum TokenType {
	lit,
	fun,
	ret,
};

struct Token {
	TokenType type;
	std::string value;
};

int main (int argc, char *argv[])
{
	// assert(argc == 3);

	std::string outpath = argc > 1 ? argv[2] : "C:/Projects/boglang/code/hello.asm";
	
	std::ifstream in(argc > 1 ? argv[1] : "C:/Projects/boglang/code/hello.glp");
	std::ofstream out(outpath);
	assert(in);
	assert(out);

	std::string content(
		(std::istreambuf_iterator<char>(in)),
		std::istreambuf_iterator<char>()
	);

	out.flush();
	

	std::replace(content.begin(), content.end(), '\n', ' ');
	std::replace(content.begin(), content.end(), '\t', ' ');

	std::stringstream strm(content);

	std::vector<std::string> words;
	
	std::string interm;
	while (getline(strm, interm, ' ')) {
		words.emplace_back(interm);
	}


	in.close();

	std::vector<Token> tokens;

	std::string buf = "";
	for (int i = 0; i != words.size(); i++) {
		auto& word = words[i];
		if (word == "" || word == "") continue;
		std::cout << word;
		std::string kw = word.substr(0, 3);
		if( kw == "fun") {
			// std::cout << word << " " << words[i+1] << std::endl;
			tokens.emplace_back(TokenType::fun, words[i+1]);
		}
		else if (kw == "ret") {
			// std::cout << word << " " << words[i+1] << std::endl;
			tokens.emplace_back(TokenType::ret, words[i+1]);
		}
	}
	std::cout << std::endl;

	
	std::string ASM = "\
global _start\n";

	for (auto& tok : tokens) {
		switch (tok.type) {
		case fun:
			ASM += tok.value + ":\n";
			break;
		case ret:
			ASM += "mov eax, " + tok.value + "\n";
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