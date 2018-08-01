/*
 *	Author: Robin Krause
 *	
 *	E-Mail: easy-dev[at]web.de
 *
 *	License: GNU Public License v3
 *
 *	Project:
 *		Architech - Simple yet effictive programming language
 *
 *	Dependancies:
 *		g++ or any other c++ compiler
 *
 *	Usage: archc input-file output-file [compiler] [c/c++ libaries]
 */

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <array>
#include <fstream>
#include <algorithm>

struct file {
	std::string name;
	std::vector<std::string> content;
};

namespace error {
	int code = 0;
	std::vector<file> files;
}

#include "./parser.h"
#include "./converter.h"
#include "./syntax-check.h"

std::vector<std::string> get_arguments(int argc, char *argv[]) {
	std::vector<std::string> arguments;

	for(int i = 0; i < argc; i++) {
		arguments.push_back(std::string(argv[i]));	
	}

	return arguments;
}

std::vector<std::string> read_input_file(std::string file_path) {
	std::ifstream input_file;
	std::vector<std::string> source_file;
	std::string line = "";
	
	input_file.open(file_path, std::ios_base::in);
	if(input_file.is_open()) {
		file temp_file;
		temp_file.name = file_path;
		while(!input_file.eof()) {
			std::getline(input_file, line);
			source_file.push_back(line);
			temp_file.content.push_back(line);
			line = ""; //Not sure if this is necessary???
		}
		temp_file.content = _parse(put_broken_lines_together(empty_comment_lines(remove_tabs(temp_file.content), file_path), true));
		error::files.push_back(temp_file);
		source_file = remove_tabs(source_file);

		auto iterator = source_file.begin();
		int line_count = 0;
		for(std::string tline: source_file) {
			if(tline.substr(0, 6) == "import") {
				std::vector<std::string> temp = read_input_file(tline.substr(7, tline.length()));
				iterator = source_file.insert(iterator + line_count, temp.begin(), temp.end());
			}
			line_count++;
			iterator = source_file.begin();
		}
	} else {
		std::cout << "[ERROR] The input file[\'" << file_path << "\'] does not exist!" << std::endl;
		error::code = -1;
	}
	return source_file;
}

int compile(int argc, char *argv[]) {
	if(argc >= 3 && argc < 6) {
		std::vector<std::string> arguments;
		std::vector<std::string> source_file;
		std::vector<std::string> parsed_file;

		arguments = get_arguments(argc, argv);
		source_file = read_input_file(arguments.at(1));
		if(error::code != -1) parsed_file = parse_source(source_file);
		if(error::code != -1 && !check_source(parsed_file)) {
			convert_source(parsed_file, arguments.at(2));
			int t = system(("g++ -std=c++14 " + arguments.at(2) + ".cpp -o " + arguments.at(2) + " -no-pie").c_str());
			std::remove((arguments.at(2) + ".cpp").c_str());
		} else {
			error::code = -1;
		}
		return error::code;
	} else {
		std::cout << "[ERROR] Too few or too many parameters used!" << std::endl;
		return -1;
	}
}

int main(int argc, char *argv[]) {
	return compile(argc, argv);
}
