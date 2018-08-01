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

/* BIG TODO!!! */

/* FACING PROBLEMS!!! Values can't be compared to types and inverted!!!

	TODO:   Implement a type and possible value struct to make it possible
		to check either or not a value is valid for the var type or not...
		Else the C++ compiler always has to take over and carry me...

	ALSO: 	This would help to implement default values... like in var $test...
		Atm this only works with the basic type, but I want it to be 
		compatible with anything! Can a class pointer equal NULL or should
		I use nullptr? Or plain 0?
*/

struct var {
	std::string name = "";
	std::string type = "";
	std::string scope = "";
	bool pointer = false; 
};

struct _struct {
	std::string name = "";
	std::string inheritance = "";
	std::vector<var> attributes;
	bool closed = false;
	std::string file_name = "";
	int line_num = 0;
};

struct func {
	std::string name = "";
	std::string type = "void";
	bool pointer = false;
	bool need_return = false;
	bool returns = false;
	std::vector<var> parameters;
	bool closed = false;
	std::string file_name = "";
	int line_num = 0;
};

struct _class {
	std::string name = "";
	std::string inheritance = "";
	std::vector<var> attributes;
	std::vector<func> methods; //Would be easier if I just add a new attribute to func and save methods global!
	func constructor;
	func deconstructor;
	bool closed = false;
	std::string file_name = "";
	int line_num = 0;
};

namespace syntax {
	std::vector<var> variables;
	std::vector<func> functions;
	std::vector<_struct> structs;
	std::vector<_class> classes;
}

std::string get_file_by_line(std::string line) {
	for(auto files: error::files) {
		for(auto tline: files.content) {
			if(tline == line) return files.name;
		}
	}

	return "UNDEFINED";
}

int get_linenum_by_line(std::string line) {
	for(auto files: error::files) {
		for(int i = 1; i < files.content.size() - 1; i++) {
			if(files.content.at(i - 1) == line) return i;
		}
	}

	return -1;
}

bool syncheck_as(std::vector<std::string> segments, int start_pos, std::string name, int linenum) {
	bool error = false;
	/* TODO: verify the used type! */
	if(segments.at(start_pos) == "as") {
		if(segments.size() > start_pos + 1) {
			if(segments.at(start_pos + 1) == "ptr") {
				std::cout << "[SYNTAX-ERROR] as: Missing identifier before \'ptr\'! [\'" << name << "\'; line: " << linenum << "]" << std::endl;
				std::cout << "\t\t[TIP] Syntax: identifier as type [ptr]" << std::endl;
				error = true;
			} else if(segments.at(start_pos + 1) == "(" || segments.at(start_pos + 1) == "=") {
				std::cout << "[SYNTAX-ERROR] as: Type identifier is missing! [\'" << name << "\'; line: " << linenum << "]" << std::endl;
				std::cout << "\t\t[TIP] Syntax: identifier as type [ptr]" << std::endl;
				error = true;
			} else if(segments.at(start_pos + 2).substr(0, 3) == "ptr" && segments.at(start_pos + 2) != "ptr") {
				std::cout << "[SYNTAX-ERROR] as: Unknown identifier! [\'" << name << "\'; line: " << linenum << "]" << std::endl;
				std::cout << "\t\t[TIP] Did you mean 'ptr' instead of '" << segments.at(start_pos + 2) << "'? [\'" << name << "\'; line: " << linenum << "]" << std::endl;
				error = true;
			}
		} else {
			std::cout << "[SYNTAX-ERROR] as: Type identifier is missing! [\'" << name << "\'; line: " << linenum << "]" << std::endl;
			std::cout << "\t\t[TIP] Syntax: identifier as type [ptr]" << std::endl;
			error = true;
		}
	}
	return error;
}

/* TODO: Implement the call case... That's what the parameters are for */
bool syncheck_var_declared(std::string var_name, std::string file_name, int linenum, int type_compare = 0, int param_pos = 0, bool msg = true) {
	bool error = true;
	func compare_func = syntax::functions.at(syntax::functions.size() - 1);
	for(var t: compare_func.parameters) {
		if(var_name == t.name) {
			if(type_compare == 1) {
				if(compare_func.type == t.type) {
					error = false;
					break;
				} else {
					std::cout << "[SYNTAX-ERROR] Variable type does not match up with the needed type! [\'" << file_name << "\'; line: " << linenum << "]" << std::endl;
					break;
				}
			} else {
				error = false;
			}
		}
	}
	if(error) {
		for(var t: syntax::variables) {
			if(t.scope == compare_func.name) {
				if(var_name == t.name) {
					if(type_compare == 1) {
						if(compare_func.type == t.type) {
							error = false;
							break;
						} else {
							std::cout << "[SYNTAX-ERROR] Variable type does not match up with the needed type! [\'" << file_name << "\'; line: " << linenum << "]" << std::endl;
							break;
						}
					} else {
						error = false;
					}
				}
			}
		}
	}
	if(error && msg) {
		std::cout << "[SYNTAX-ERROR] Variable used without being declared! [\'" << file_name << "\'; line: " << linenum << "]" << std::endl;
		std::cout << "\t\t[TIP] Place this line somewhere above: var $" << var_name << std::endl;
	}
	return error;
}

bool syncheck_create_variable(std::vector<std::string> segments, std::string line) {
	std::string current_file = get_file_by_line(line);
	int current_linenum = get_linenum_by_line(line);
	func current_func = syntax::functions.at(syntax::functions.size() - 1);
	std::string name = "";
	int bracket_pos = 0;
	bool error = false;

	/*
		TODO:
		[x] 0. var <- forgot nearly everything!
		[x] 1. var name <- forgot the $ sign!
		[ ] 2. var $name = 12 <- can't be checked atm!
		[-] 3. var $name as ppp ptrr <- It's ptr (and pp is undeclared)-> not handled in the syncheck_as!
		[ ] 4. var $name as Name = () <- Unknown Constructor call!
		[ ] 5. var $name = readline() <- Undefinied function!
		[x] 6. var $name <- already declared!
		[x]7. var $name = <- Missing value
	*/
	if(segments.size() == 1) {
		std::cout << "[SYNTAX-ERROR] var: Missing var-name! [\'" << current_file << "\'; line: " << current_linenum << "]" << std::endl;
		std::cout << "\t\t[TIP] Syntax: var $NAME [as type [ptr]] = [value]" << std::endl;
		error = true;
	} else {
		if(segments.size() > 1 && segments.at(1).substr(0, 1) != "$") {
			std::cout << "[SYNTAX-ERROR] var: You forgot the '$' sign! [\'" << current_file << "\'; line: " << current_linenum << "]" << std::endl;
			std::cout << "\t\t[TIP] Change the line into: var $" << segments.at(1) << "..." << std::endl;
			error = true;
		} else if(segments.size() > 1 && segments.at(1).substr(0, 1) == "$") {
			if(!syncheck_var_declared(segments.at(1).substr(1, segments.at(1).length()), current_file, current_linenum, 0, 0, false)) {
				std::cout << "[SYNTAX-ERROR] var: Variable is already declared! [\'" << current_file << "\'; line: " << current_linenum << "]" << std::endl;
				error = true;
			}
		} else if(segments.size() > 2) {
			if(syncheck_as(segments, 2, current_file, current_linenum)) error = true;
			if(segments.size() > 2 && find_segment(segments, "=") == segments.size() - 1) {
				std::cout << "[SYNTAX-ERROR] var: Missing value after allocation! [\'" << current_file << "\'; line: " << current_linenum << "]" << std::endl;
				std::cout << "\t\t[TIP] If you delete the '=' sign it will compile!" << std::endl;
				error = true;
			} else {
				int alloc_pos = find_segment(segments, "=");
				int start_bracket_pos = find_segment(segments, "(");
				int end_bracket_pos = find_segment(segments, ")");
				
				if(alloc_pos == -1) {
					/* TODO: Missing allocation */
				} else if(start_bracket_pos == -1) {
					/* TODO: Missing opening bracket */
				} else if(end_bracket_pos == -1) {
					/* TODO: Missing closing bracket */
				} else {
					if(alloc_pos + 1 == start_bracket_pos) {
						//Constructor call
					} else {

					}
				}
			}
		}
		
	}
	if(!error) {
		if(syntax::functions.size() > 0) {
			var checked_var;
			checked_var.name = segments.at(1).substr(1, segments.at(1).length());
			if(segments.size() > 3 && segments.at(2) == "as") {
				if(segments.at(3) == "var") {
					checked_var.type = "std::string";
				} else {
					checked_var.type = segments.at(3);
				}
				if(segments.size() > 4 && segments.at(4) == "ptr") {
					checked_var.pointer = true;
				}
			} else {
				checked_var.type = "std::string";
			}
			checked_var.scope = syntax::functions.at(syntax::functions.size() - 1).name;
			syntax::variables.push_back(checked_var);
		} else {
			std::cout << "[SYNTAX-ERROR] Global variables are forbidden! [\'" << current_file << "\'; line: " << current_linenum << "]" << std::endl;
			std::cout << "\t\t[TIP] Use namespaces instead!" << std::endl;
			error = true;
		}
	}

	return error;
}

bool syncheck_create_function(std::vector<std::string> segments, std::string line) {
	std::string current_file = get_file_by_line(line);
	int current_linenum = get_linenum_by_line(line);
	std::string name = "";
	int bracket_pos = 0;
	bool error = false;
	
	if(segments.size() > 3) {
		name = segments.at(1);
		if(name.substr(0, 1) == "$") {
			std::cout << "[SYNTAX-ERROR] func: A variable can't be a function name! [\'" << current_file << "\'; line: " << current_linenum << "]" << std::endl;
			std::cout << "\t\t[TIP] You might revisit the documentation again..." << std::endl;
			error = true;
		}
		for(func f: syntax::functions) {
			if(f.name == name) {
				std::cout << "[SYNTAX-ERROR] func: This function name already exists! [\'" << current_file << "\'; line: " << current_linenum << "]" << std::endl;
				error = true;
			}
		}
		if(syncheck_as(segments, 2, current_file, current_linenum)) error = true;
		bracket_pos = find_segment(segments, "(");
		if(bracket_pos == -1) {
			std::cout << "[SYNTAX-ERROR] func: Missing bracket after identifier! [\'" << current_file << "\'; line: " << current_linenum << "]" << std::endl;
			std::cout << "\t\t[TIP] Function definitions without '()' are forbidden!" << std::endl;
			error = true;
		} else {
			if(find_next_segment(segments, ")", bracket_pos) == -1) {
				std::cout << "[SYNTAX-ERROR] func: Missing closing bracket! [\'" << current_file << "\'; line: " << current_linenum << "]" << std::endl;
				std::cout << "\t\t[TIP] Function definitions without '()' are forbidden!" << std::endl;
				error = true;
			} else {
				for(int i = bracket_pos + 1; i < segments.size() - 1; i++) {
					if(syncheck_as(segments, i + 1, current_file, current_linenum)) error = true;
				}	
			}
		}
		if(!error) {
			func checked_func;
			checked_func.name = name;
			checked_func.file_name = current_file;
			checked_func.line_num = current_linenum;
			if(segments.size() > 3 && segments.at(2) == "as") {
				if(segments.at(3) == "var") {
					checked_func.type = "std::string";
				} else {
					checked_func.type = segments.at(3);
				}
				if(segments.size() > 4 && segments.at(4) == "ptr") {
					checked_func.pointer = true;
				}
				checked_func.need_return = true;
			}
			if(name == "main") {
				checked_func.type = "int";
				checked_func.need_return = true;
			}
			bool was_seperated = true;	
			for(int i = bracket_pos + 1; i < segments.size() - 1; i++) {
				if(segments.at(i).substr(0, 1) == "$") {
					if(was_seperated) {
						was_seperated = false;
					} else {
						std::cout << "[SYNTAX-ERROR] func: Missing ',' before expression! [\'" << current_file << "\'; line: " << current_linenum << "]" << std::endl;
						error = true;
					}
					var param_var;
					param_var.name = segments.at(i).substr(1, segments.at(i).length());
					param_var.scope = name;
					if(segments.size() > i + 2 && segments.at(i + 1) == "as") {
						if(segments.at(i + 2) == "var") {
							param_var.type = "std::string"; 
						} else {
							param_var.type = segments.at(i + 2);
						}
						if(segments.size() > i + 3 && segments.at(i + 3) == "ptr") {
							param_var.pointer = true;
							i += 3;
						} else {
							param_var.pointer = false;
							i += 2;
						}
					} else {
						param_var.type = "std::string";
						param_var.pointer = false;
					}
					checked_func.parameters.push_back(param_var);
				} else if(segments.at(i) == "," && segments.at(i + 1) == ")") {
					std::cout << "[SYNTAX-ERROR] func: Prohibited use of a comma in front of a bracket! [\'" << current_file << "\'; line: " << current_linenum << "]" << std::endl;
					error = true;
				} else if(segments.size() > i + 1 && segments.at(i) == "," && segments.at(i + 1) == ",") {
					std::cout << "[SYNTAX-ERROR] func: Unqualified identifier(',') before expression! [\'" << current_file << "\'; line: " << current_linenum << "]" << std::endl;
					error = true;
				} else if(segments.at(i) == ",") {
					was_seperated = true;
				}
			}
			syntax::functions.push_back(checked_func);
		}
	} else {
		std::cout << "[SYNTAX-ERROR] func: Too less parameters provided! [\'" << current_file << "\'; line: " << current_linenum << "]" << std::endl;
		std::cout << "\t\t[TIP] You might forgot a bracket..." << std::endl;
		error = true;
	}
	return error;
}

bool syncheck_ret(std::vector<std::string> segments, std::string line) {
	bool error = false;
	std::string current_file = get_file_by_line(line);
	int current_linenum = get_linenum_by_line(line);

	for(int i = 1; i < segments.size(); i++) {
		if(segments.size() > i + 2 && segments.at(i).substr(0, 1) == "$" && segments.at(i + 1).substr(0, 3) == "ref" && segments.at(i + 1) != "ref") {
			std::cout << "[SYNTAX-ERROR] ret: unknown symbol! [\'" << current_file << "\'; line: " << current_linenum << "]" << std::endl;
			std::cout << "\t\t[TIP] Did you mean 'ref' instead of '" << segments.at(2) << "'?" << std::endl;
			error = true;
		} else if(segments.size() > 1 && segments.at(i).substr(0, 1) == "$") {
			if(syncheck_var_declared(segments.at(i).substr(1, segments.at(i).length()), current_file, current_linenum, 1)) error = true;
		}
	}

	return error;
}

bool check_source(std::vector<std::string> parsed_file) {
	bool error = false;
	
	std::vector<std::string> segments;

	for(std::string line: parsed_file) {
		segments = split_line(line);

		if(segments.size() > 0) {
			if(segments.at(0) == "func") {
				if(syncheck_create_function(segments, line)) error = true;
			} else if(segments.at(0) == "var") {
				if(syncheck_create_variable(segments, line)) error = true;
			} else if(segments.at(0).substr(0, 1) == "$") {
				/*
				std::string name = "";
				std::string value = "";
				if(segments.size() > 2) {
					name = segments.at(0).substr(1, segments.at(0).length());
					if(segments.at(1) == "=") {
						value = get_used_value(segments, 2, segments.size() - 1);
						converted_file.push_back(name + " = " + value + ";");
					}
				}
				*/
			} else if(segments.at(0) == "ret") {
				if(syncheck_ret(segments, line)) error = true;
				if(!syntax::functions.at(syntax::functions.size() - 1).need_return) {
					std::cout << "[SYNTAX-ERROR] ret: Used in a void function! [\'" << get_file_by_line(line) << "\'; line: " << get_linenum_by_line(line) << "]" << std::endl;
					std::cout << "\t\t[TIP] A function created without an 'as' never returns anything!" << std::endl; 
					error = true;
				} else {
					syntax::functions.at(syntax::functions.size() - 1).returns = true;
				}
			} else if(segments.at(0) == "end") {
				//TODO: Add new stuff ABOVE this line!
				if(!syntax::functions.at(syntax::functions.size() - 1).closed) {
					syntax::functions.at(syntax::functions.size() - 1).closed = true;
				} else {
					std::cout << "[SYNTAX-ERROR] end: Used without anything to close! [\'" << get_file_by_line(line) << "\'; line: " << get_linenum_by_line(line) << "]" << std::endl;
					std::cout << "\t\t[TIP] Use tabs to make things more clearer!" << std::endl;
					error = true;
				}
			} else {
				/*
				std::string out = "";
				if(segments.size() > 2) {
					out = get_used_value(segments, 1, segments.size() - 1);
					converted_file.push_back(segments.at(0) + out + ";");
				} else {
					converted_file.push_back(segments.at(0) + "();");
				}
				*/
			}
		}
	}

	for(func t: syntax::functions) {
		if(!t.closed) {
			std::cout << "[SYNTAX-ERROR] Unclosed function! [\'" << t.file_name << "\'; line: " << t.line_num << "]" << std::endl;
			std::cout << "\t\t[TIP] Every function has to be closed with the 'end' keyword!" << std::endl;
			error = true;
		}
		if(t.need_return && !t.returns) {
			std::cout << "[SYNTAX-ERROR] Missing 'ret' statement in non-void function! [\'" << t.file_name << "\'; line: " << t.line_num << "]" << std::endl;
			std::cout << "\t\t[TIP] Once you used as you need a ret statement, else just delete the as to make the function void!" << std::endl;
			error = true;
		}
	}	
	
	std::cout << "---------------- Debug Output ----------------" << std::endl;

	for(auto f: syntax::functions) {
		std::cout << "---FUNCTION: " << f.file_name << "; " << f.line_num << std::endl;
		std::cout << "name..........: " << f.name << std::endl;
		std::cout << "type..........: " << f.type << std::endl;
		std::cout << "pointer?......: " << f.pointer << std::endl;
		std::cout << "return?.......: " << f.need_return << std::endl;
		std::cout << "closed?.......: " << f.closed << std::endl;
		std::cout << "---" << std::endl;
		for(auto t: f.parameters) {
			std::cout << "\t\t\t---PARAM" << std::endl;
			std::cout << "\t\t\tname..........: " << t.name << std::endl;
			std::cout << "\t\t\ttype..........: " << t.type << std::endl;
			std::cout << "\t\t\tscope.........: " << t.scope << std::endl;
			std::cout << "\t\t\tpointer?......: " << t.pointer << std::endl;
			std::cout << "\t\t\t---" << std::endl;
		}
	}

	for(auto f: syntax::variables) {
		std::cout << "---VARIABLE: " << std::endl;
		std::cout << "name..........: " << f.name << std::endl;
		std::cout << "type..........: " << f.type << std::endl;
		std::cout << "scope.........: " << f.scope << std::endl;
		std::cout << "pointer?......: " << f.pointer << std::endl;
		std::cout << "---" << std::endl;
	}


	return error;
}
