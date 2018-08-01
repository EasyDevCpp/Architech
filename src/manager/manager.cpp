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
 *	Usage: architech_manager help
 *				 run
 *				 build
 *				 new [Project Name]
 */

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

std::vector<std::string> get_arguments(int argc, char *argv[]) {
	std::vector<std::string> arguments;

	for(int i = 0; i < argc; i++) {
		arguments.push_back(std::string(argv[i]));	
	}

	return arguments;
}

int arch_manager(int argc, char* argv[]) {
	std::vector<std::string> arguments = get_arguments(argc, argv);

	if(argc > 1) {
		if(arguments.at(1) == "help") {
			std::cout << "Architech Manager Help[ver.: 0.0.1]:" << std::endl;
			std::cout << "\t\t\tnew [Project name]:\tInits a new project" << std::endl;
			std::cout << "\t\t\tbuild\t\t  :\tBuilds the current project" << std::endl;
			std::cout << "\t\t\trun\t\t  :\tRun the current project" << std::endl;
			std::cout << "\t\t\thelp\t\t  :\tshow help" << std::endl;
		} else if(arguments.at(1) == "new") {
			if(argc == 3) {
				std::ofstream source_file;
				std::ofstream build_sheet;
				std::ofstream run_sheet;
				int mkdir = 0;

				std::system("mkdir -p ~/Projects/");
				/* mkdir = */ std::system(("mkdir ~/Projects/" + arguments.at(2) + "/").c_str());

				if(mkdir != 256) {
					std::system(("mkdir -p ~/Projects/" + arguments.at(2) + "/inc/").c_str());
					std::system(("mkdir -p ~/Projects/" + arguments.at(2) + "/src/").c_str());
					const char *homedir;
					if((homedir = getenv("HOME")) == NULL) {
	   	 				homedir = getpwuid(getuid())->pw_dir;
					}
					build_sheet.open(std::string(homedir) + "/Projects/" + arguments.at(2) + "/build.sh", std::ios_base::out);
					run_sheet.open(std::string(homedir) + "/Projects/" + arguments.at(2) + "/run.sh", std::ios_base::out);
					source_file.open(std::string(homedir) + "/Projects/" + arguments.at(2) + "/src/" + arguments.at(2) + ".arch", std::ios_base::out);

					if(build_sheet.is_open()) {
					 	build_sheet << "#! /bin/bash" << std::endl;
						build_sheet << "archc ./src/" + arguments.at(2) + ".arch ./" + arguments.at(2) << std::endl;
						build_sheet.close();
					} else {
						std::cout << "Fatal Error: Couldn't create build file!" << std::endl;
					}

					if(run_sheet.is_open()) {
						run_sheet << "#! /bin/bash" << std::endl;
						run_sheet << "./" +  arguments.at(2) << std::endl;
						run_sheet.close();
					} else {
						std::cout << "Fatal Error: Couldn't create run file!" << std::endl;
					}

					if(source_file.is_open()) {
						source_file << "import ./inc/basics.arch" << std::endl;
						source_file << std::endl;
						source_file << "func main()" << std::endl;
						source_file << "\t# Happy coding :D" << std::endl;
						source_file << "\tret" << std::endl;
						source_file << "end" << std::endl;
						source_file << std::endl;
					} else {
						std::cout << "Fatal Error: Couldn't create source file!" << std::endl;

					}

					std::system(("cp /usr/include/architech/basics.arch " + std::string(homedir) + "/Projects/" + arguments.at(2) + "/inc/basics.arch").c_str());
				} else {
					std::cout << "Fatal Error: Project already exists!" << std::endl;
				}
			} else {
				std::cout << "Please use: 'architech_manager new [project_name]'!" << std::endl;
				std::cout << "Or view help: 'architech_manager help'!" << std::endl;
			}
		} else if(arguments.at(1) == "build") {
			std::system("sh build.sh");
		} else if(arguments.at(1) == "run") {
			std::system("sh run.sh");
		} else {
			std::cout << "Unknown parameter! Use 'architech_manager help'!" << std::endl;
		}
	} else {
		std::cout << "You might use: architech_manager help" << std::endl;
	}

	return 0;
}

int main(int argc, char* argv[]) {
	return arch_manager(argc, argv);
}
