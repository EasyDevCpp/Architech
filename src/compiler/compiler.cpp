/*
 *	Architech - Yet simple but yet effective programming language
 *   	Copyright (C) 2018 - Robin Krause
 *
 *   	This program is free software: you can redistribute it and/or modify
 *   	it under the terms of the GNU General Public License as published by
 *   	the Free Software Foundation, either version 3 of the License, or
 *   	(at your option) any later version.
 *
 *   	This program is distributed in the hope that it will be useful,
 *   	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  	GNU General Public License for more details.
 *
 *   	You should have received a copy of the GNU General Public License
 *   	along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
	std::vector <std::string> arguments;
	std::vector<std::string> source_file;
	std::vector<std::string> parsed_file;
	std::string out_file = "";
	std::string parameters = "";
	bool options = false;
	bool help = false;

	if(argc > 1) {
		arguments = get_arguments(argc, argv);
		if(arguments.at(1) == "help") {
			std::cout << "Architech Compiler Help[ver..: 0.0.2]:" << std::endl;
			std::cout << "\tSyntax: archc [options] input_file [action] [output_file] [libaries]\n" << std::endl;
			std::cout << "\toptions:" << std::endl;
			std::cout << "\t\tno-warnings :\t\tdisable warnings" << std::endl;
			std::cout << std::endl;
			std::cout << "\tactions:" << std::endl;
			std::cout << "\t\thelp        :\t\tshow help" << std::endl;
			std::cout << "\t\tcheck       :\t\tonly syntax check" << std::endl;
			std::cout << "\t\tbuild       :\t\tbuild an executable" << std::endl;
			std::cout << "\t\tconvert     :\t\tconvert to c++" << std::endl;
			help = true;
		} else if(arguments.at(1) == "no-warnings") {
			if(argc > 3) {
				syntax::show_warnings = false;
				options = true;
			} else {
				std::cout << "[ERROR] Too less paramters used! Use 'archc help' to view help!" << std::endl;
				return -1;
			}
		}
		if(argc > 2 && !help) {
			if(arguments.at(2) == "check" || (argc > 3 && arguments.at(3) == "check")) {
				if(options) {
					if(argc > 3) {
						source_file = read_input_file(arguments.at(2));
					} else {
						std::cout << "[ERROR] Too less paramters used! Use 'archc help' to view help!" << std::endl;
						return -1;
					}
				} else {
					source_file = read_input_file(arguments.at(1));
				}
				if(error::code != -1) parsed_file = parse_source(source_file);
				if(error::code != -1) check_source(parsed_file);
				return 0;
			}
		}
		if(argc > 3 && !help) {
			if(options) {
				if(argc > 4) {
					source_file = read_input_file(arguments.at(2));
					out_file = arguments.at(4);
					if(argc > 5) {
						for(int i = 5; i < arguments.size(); i++) {
							parameters += arguments.at(i) + " ";
						}
					}
				} else {
					std::cout << "[ERROR] Too less paramters used! Use 'archc help' to view help!" << std::endl;
					return -1;
				}
			} else {
				source_file = read_input_file(arguments.at(1));
				out_file = arguments.at(3);
				if(argc > 4) {
					for(int i = 4; i < arguments.size(); i++) {
						parameters += arguments.at(i) + " ";
					}
				}
			}
			if(arguments.at(2) == "build" || arguments.at(3) == "build") {	
				if(error::code != -1) parsed_file = parse_source(source_file);
				if(error::code != -1 && !check_source(parsed_file)) {
					convert_source(parsed_file, out_file);
					std::system(("g++ -std=c++14 " + out_file + ".cpp -o " + out_file + " " + parameters + "-no-pie").c_str());
					std::remove((out_file + ".cpp").c_str());
				}
				return error::code = -1;
			} else if(arguments.at(2) == "convert" || arguments.at(3) == "convert") {
				if(error::code != -1) parsed_file = parse_source(source_file);
				if(error::code != -1 && !check_source(parsed_file)) {
					convert_source(parsed_file, out_file);
				}
				return error::code = -1;
			}
		}
	} else {
		std::cout << "[ERROR] Missing parameters! Use 'archc help' to view help!" << std::endl;
		return -1;
	}
	return 0;	
}

int main(int argc, char *argv[]) {
	return compile(argc, argv);
}
