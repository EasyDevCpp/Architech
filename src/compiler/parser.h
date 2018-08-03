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

std::vector<std::string> remove_tabs(std::vector<std::string> source_file) {
	std::vector<std::string> trimmed_file;

	for(std::string line: source_file) {
		line.erase(std::remove(line.begin(), line.end(), '\t'), line.end());
		trimmed_file.push_back(line);
	}

	return trimmed_file;
}

std::vector<std::string> empty_comment_lines(std::vector<std::string> source_file, std::string file) {
	std::vector<std::string> trimmed_file;
	bool comment_block = false;
	bool in_cpp = false;

	for(std::string line: source_file) {
		if(line == "__cpp") {
			trimmed_file.push_back(""); //Hotfix wrong line number
			in_cpp = true;
		} else if(line == "cpp__") {
			trimmed_file.push_back(""); //Hotfix wrong line number
			in_cpp = false;
		}

		if(!comment_block) {
			if(line.substr(0, 3) == "#cs") {
				comment_block = true;
				trimmed_file.push_back("");
			} else if(!in_cpp) {
				for(int i = 0; i < line.length(); i++) {
					if(line.substr(i, 1) == "#") {
						if(i - 1 == -1) {
							line = "";
						} else {
							line = line.substr(0, i - 1);
						}
						break;
					}
				}
				trimmed_file.push_back(line);
			} else {
				trimmed_file.push_back(line);
			}
		} else {
			if(line.substr(0, 3) == "#ce") {
				comment_block = false;
			}
			trimmed_file.push_back("");
		}
	}

	if(comment_block) {
		std::cout << "[SYNTAX-ERROR] Unclosed comment-block in file: " << file << std::endl;
		std::cout << "[ERROR] Compilation aborted." << std::endl;
		error::code = -1;
	}

	return trimmed_file;
}

std::vector<std::string> remove_comments(std::vector<std::string> source_file) {
	std::vector<std::string> trimmed_file;
	bool comment_block = false;
	bool in_cpp = false;

	for(std::string line: source_file) {
		if(line == "__cpp") in_cpp = true;
		else if(line == "cpp__") in_cpp = false;

		if(!comment_block) {
			if(line.substr(0, 3) == "#cs") {
				comment_block = true;	
			} else if(!in_cpp) {
				for(int i = 0; i < line.length(); i++) {
					if(line.substr(i, 1) == "#") {
						if(i - 1 == -1) {
							line = "";
						} else {
							line = line.substr(0, i - 1);
						}
						break;
					}
				}
				trimmed_file.push_back(line);
			} else {
				trimmed_file.push_back(line);
			}
		} else {
			if(line.substr(0, 3) == "#ce") {
				comment_block = false;
			}
		}
	}

	return trimmed_file;
}

std::vector<std::string> remove_empty_lines(std::vector<std::string> source_file) {
	std::vector<std::string> trimmed_file;

	for(std::string line: source_file) {
		if(line != "") trimmed_file.push_back(line);
	}

	return trimmed_file;
}

std::vector<std::string> remove_imports(std::vector<std::string> source_file) {
	std::vector<std::string> trimmed_file;

	for(std::string line: source_file) {
		if(line.substr(0,6) != "import") trimmed_file.push_back(line);
	}

	return trimmed_file;
}

std::vector<std::string> put_broken_lines_together(std::vector<std::string> source_file, bool add_empty_line = false) {
	std::vector<std::string> combined_file;
	bool line_break = false;
	bool next_line = false;
	std::string put_together = "";

	for(int i = 0; i < source_file.size(); i++) {
		std::string line = source_file.at(i);
		for(int j = 0; j < line.length(); j++) {
			if(line.substr(j, 2) == "&_") {
				line = line.substr(0, j);
				line_break = true;
				break;
			}
		}
		if(line_break) {
			if(next_line) {
				combined_file.push_back(put_together + line);
				if(add_empty_line) combined_file.push_back("");
				next_line = false;
				line_break = false;
			} else {
				next_line = true;
				put_together = line;
			}
		} else {
			combined_file.push_back(line);
		}
	}

	return combined_file;
}

std::vector<std::string> _parse(std::vector<std::string> source_file) {
	std::vector<std::string> parsed_file;
	bool string_opened = false;
	std::string temp_line = "";
	bool cpp_inline = false;
	bool is_text = false;
	bool dont_push_back = false;
	std::array<std::string, 28> tokens = { std::string("("), ")", " ", ",", "<=", ">=", ".", ":", "[", "]", "-=", "+=", "*=", "/=", "-", "+", "*", "/",
						"==", "!=", "<", ">", "=", "!", "%", "&", "&&", "||" };

	for(std::string line: source_file) {
		if(!cpp_inline) {
			if(line == "__cpp") {
				cpp_inline = true;
				dont_push_back = true;
			} else {
				for(int i = 0; i < line.length(); i++) {
					if(line.substr(i, 1) == "\"" && line.substr(i-1, 2) != "\\\"") {
						if(!string_opened) {
							string_opened = true;
							temp_line += "\"";
						} else {
							string_opened = false;
							temp_line += "\"";
						}
					} else {
						if(string_opened) {
							temp_line += line.substr(i, 1);
						} else {
							bool is_token = false;
							for(auto t: tokens) {
								if(line.substr(i, t.length()) == t) {
									if(is_text) {
										is_text = false;
									}
									if(t != " ") {
										if(line.substr(i - 1, 1) != " " && line.substr(i - 1, 1) != "(") {
											temp_line += " " + line.substr(i, t.length());
										} else {
											temp_line += line.substr(i, t.length());
										}
										if(line.substr(i + 1, 1) != " ") {
											temp_line += " ";
										}
									} else {
										temp_line += " ";
									}
									i += t.length() - 1;
									is_token = true;
									break;
								}
							}
							if(!is_token) {
								temp_line += line.substr(i, 1);
								is_text = true;
							}
						}
					}
				}
			}
		} else {
			if(line == "cpp__") {
				cpp_inline = false;
				dont_push_back = true;	
			} else { 
				temp_line = "cpp " + line;
			}
		}
		if(!dont_push_back) {
			parsed_file.push_back(temp_line);
		} else {
			dont_push_back = false;
		}
		temp_line = "";
	}

	if(string_opened) {
		std::cout << "[ERROR] You forgot a quotation mark!" << std::endl;
		error::code = -1;
	}
	if(cpp_inline) {
		std::cout << "[ERROR] You forgot to close the cpp-inline tag!" << std::endl;
	}

	return parsed_file;
}

std::vector<std::string> parse_source(std::vector<std::string> source_file) {
	std::vector<std::string> parsed_file;

	parsed_file = remove_comments(source_file);
	parsed_file = remove_empty_lines(parsed_file);
	parsed_file = remove_imports(parsed_file);
	parsed_file = put_broken_lines_together(parsed_file);
	parsed_file = _parse(parsed_file);
	
	return parsed_file;
}
