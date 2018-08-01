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

namespace default_values {
	std::string basic_type = "\"\"";
	std::string char_type = "''";
	std::string short_type = "0";
	std::string int_type = "0";
	std::string long_type = "0";
	std::string float_type = "0.0";
	std::string double_type = "0.0";
	std::string bool_type = "false";
	std::string unknown = "NULL";
	std::string unknown_ptr = "nullptr";

	std::array<std::string, 10> defaults = {basic_type,
						char_type,
						short_type,
						int_type,
						long_type,
						float_type,
						double_type,
						bool_type,
       						unknown,
						unknown_ptr};
	std::array<std::string, 8> types = { "std::string", "char", "short",
					     "int", "long", "float",
					     "double", "bool" };
}

std::string get_default_value_by_type(std::string type, bool pointer) {
	std::string default_value = "";
	for(int i = 0; i < default_values::types.size(); i++) {
		if(default_values::types[i] == type) {
			return default_values::defaults[i];
		}
	}
	if(!pointer) {
		return default_values::defaults[8];
	} else {
		return default_values::defaults[9];
	}
	return default_value;
}

void string_replace_all(std::string& source, const std::string& from, const std::string& to) {
    std::string newString;
    newString.reserve(source.length());
    std::string::size_type lastPos = 0;
    std::string::size_type findPos;
    while(std::string::npos != (findPos = source.find(from, lastPos))) {
        newString.append(source, lastPos, findPos - lastPos);
        newString += to;
        lastPos = findPos + from.length();
    }
    newString += source.substr(lastPos);
    source.swap(newString);
}

std::vector<std::string> string_split(std::string input, char delim) {
	std::stringstream test(input);
	std::string segment;
	std::vector<std::string> seglist;

	while(std::getline(test, segment, delim)) {
		seglist.push_back(segment); 
	}
	return seglist;
}

std::vector<std::string> split_line(std::string line) {
	std::vector<std::string> ret_segs;
	std::vector<std::string> splitted_line;
	std::string temp = "";
	bool contains_string = false;
	bool in_cpp = false;
	
	splitted_line = string_split(line, ' ');
	for(std::string seg: splitted_line) {
		if(!in_cpp) {
			if(!contains_string) {
				if(seg == "\"" || (seg.substr(0, 1) == "\"" && seg.substr(seg.length() - 1,1) != "\"")) {
					contains_string = true;
					temp += seg + " ";
				} else {
					if(seg == "cpp") {
						in_cpp = true;
					}
					ret_segs.push_back(seg);
				}
			} else {
				if(seg.substr(seg.length() - 1, 1) == "\"") {
					ret_segs.push_back(temp + seg);
					contains_string = false;
					temp = "";
				} else {
					temp += seg + " ";
				}
			}
		} else {
			temp += seg + " ";
		}
	}
	if(in_cpp) {
		ret_segs.push_back(temp);
	}

	return ret_segs;
}

int find_segment(std::vector<std::string> segments, std::string search) {
	for(int i = 0; i < segments.size(); i++) {
		if(segments.at(i) == search) return i;
	}
	return -1;
}

int find_next_segment(std::vector<std::string> segments, std::string search, int prev_pos) {
	if(segments.size() > prev_pos + 1) {
		for(int i = prev_pos + 1; i < segments.size(); i++) {
			if(segments.at(i) == search) return i;
		}
	}
	return -1;
}

std::string get_creation_parameters(std::vector<std::string> segments, int start_pos) {
	std::string params = "";
	
	if(segments.size() > start_pos + 1) { //<- Skip function definitions like this one: name()
		for(int i = start_pos; i < segments.size() - 1; i++) { //<- size() - 1, to skip the closing bracket
			if(segments.size() > i + 3 && segments.at(i + 1) == "as" && segments.at(i + 3) == "ptr") {
				params += segments.at(i + 2) + "* " + segments.at(i).substr(1, segments.at(i).length());
				i += 3;
			} else if(segments.size() > i + 2 && segments.at(i + 1) == "as") {
				params += segments.at(i + 2) + " " + segments.at(i).substr(1, segments.at(i).length());
				i += 2;
			} else if(segments.at(i).substr(0, 1) == "$") {
				params += "std::string " + segments.at(i).substr(1, segments.at(i).length());
			} else { //<- seperator or syntax-error
				params += segments.at(i) + " ";
			}
		}
	}

	return params;
}

std::string get_used_value(std::vector<std::string> segments, int start_pos, int end_pos) {
	std::string value = "";

	if(segments.size() > start_pos && segments.size() > end_pos) {
		for(int i = start_pos; i < end_pos + 1; i++) {
			if(segments.size() > i + 1 && segments.at(i + 1) == "ref") {
				value += "&" + segments.at(i).substr(1, segments.at(i).length());
				i++;
			} else if(segments.at(i).substr(0, 1) == "$") {
				value += segments.at(i).substr(1, segments.at(i).length());
			} else {
				value += segments.at(i);
			}
		}
	}

	return value;
}

std::vector<std::string> _convert(std::vector<std::string> parsed_file) {
	std::vector<std::string> converted_file;
	std::vector<std::string> segments;

	converted_file.push_back("#include <string>");

	for(std::string line: parsed_file) {
		segments = split_line(line);
		
		/* For debugging reasons TODO: delete those reasons
		for(auto t: segments) {
			std::cout << t << std::endl;
		}
		*/

		if(segments.size() > 0) {
			if(segments.at(0) == "cpp") {
				string_replace_all(segments.at(1),"$","");
				converted_file.push_back(segments.at(1));
			} else if(segments.at(0) == "func") {
				std::string name = segments.at(1);
				std::string type = "void";
				std::string params = "";
				int params_pos = 0;
				if(segments.at(2) == "as") {
					if(segments.at(3) == "var") type = "std::string";
					else type = segments.at(3);
					if(segments.at(4) == "ptr") type += "*";
				}
				params_pos = find_segment(segments, "(");
				params = get_creation_parameters(segments, params_pos + 1);
				if(name == "main") type = "int";
				converted_file.push_back(type + " " + name + "(" + params + ") {");
			} else if(segments.at(0) == "var") {
				std::string name = segments.at(1).substr(1, segments.at(1).length());
				std::string type = "std::string";
				bool pointer = false;
				std::string value = "\"\"";
				int value_pos = 0;
				std::string params = "";
				int params_pos = 0;
				if(segments.size() > 3) {
					if(segments.at(2) == "as") {
						if(segments.at(3) == "var") type = "std::string";
						else type = segments.at(3);
						if(segments.size() > 4 && segments.at(4) == "ptr") {
							type += "*";
							pointer = true;
						}
					}
				}
				value_pos = find_segment(segments, "=");
				if(value_pos != -1) {
					if(segments.at(value_pos + 1) == "(") {
						params = get_used_value(segments, value_pos + 2, segments.size() - 2);
						params_pos = 1;
					} else {
						value = get_used_value(segments, value_pos + 1, segments.size() - 1);	
					}
				} else {
					value = get_default_value_by_type(type, pointer);
				}	
				if(params_pos == 0) {
					converted_file.push_back(type + " " + name + " = " + value + ";");
				} else {
					if(pointer) {
						converted_file.push_back(type + " " + name + " = new " + type.substr(0, type.length() - 1) + "(" + params + ");");
					} else {
						converted_file.push_back(type + " " + name + " = "+ type + "(" + params + ");");
					}
				}
			} else if(segments.at(0).substr(0, 1) == "$") {
				std::string name = "";
				std::string value = "";
				if(segments.size() > 2) {
					name = segments.at(0).substr(1, segments.at(0).length());
					if(segments.at(1) == "=") {
						value = get_used_value(segments, 2, segments.size() - 1);
						converted_file.push_back(name + " = " + value + ";");
					}
				}
			} else if(segments.at(0) == "ret") {
				/* TODO: What about eg: ret ($t * 3)/2 ? */
				if(segments.size() == 3 && segments.at(2) == "ref") {
					string_replace_all(segments.at(1), "$", "");
				       	converted_file.push_back("return &" + segments.at(1) + ";");
				} else if(segments.size() == 2) {
					string_replace_all(segments.at(1), "$", "");
					converted_file.push_back("return " + segments.at(1) + ";");
				} else {
					converted_file.push_back("return 0;");
				}
			} else if(segments.at(0) == "end") {
				converted_file.push_back("}");
			} else {
				std::string out = "";
				if(segments.size() > 2) {
					out = get_used_value(segments, 1, segments.size() - 1);
					converted_file.push_back(segments.at(0) + out + ";");
				} else {
					converted_file.push_back(segments.at(0) + "();");
				}
			}
		}
	}

	return converted_file;
}

void write_to_file(std::string output_path, std::vector<std::string> converted_file) {
	std::ofstream output_file;

	output_file.open(output_path + ".cpp", std::ios_base::out);
	if(output_file.is_open()) {
		for(std::string line: converted_file) {
			output_file << line << std::endl;
		}	
	} else {
		std::cout << "[ERROR] Can't write/create the output file!" << std::endl;
		error::code = -1;
	}
}

void convert_source(std::vector<std::string> parsed_file, std::string output_path) {
	std::vector<std::string> converted_file;

	converted_file = _convert(parsed_file);
	write_to_file(output_path, converted_file);
}
