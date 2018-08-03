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
			std::cout << "Architech Manager Help[ver.: 0.0.2]:" << std::endl;
			std::cout << "\t\t\tnew [Project name]:\tInits a new project" << std::endl;
			std::cout << "\t\t\tbuild\t\t  :\tBuilds the current project" << std::endl;
			std::cout << "\t\t\trun\t\t  :\tRun the current project" << std::endl;
			std::cout << "\t\t\tcheck\t\t  :\tCheck the current project" << std::endl;
            std::cout << "\t\t\tconvert\t\t  :\tConvert the current project to C++-code" << std::endl;
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
					std::system(("mkdir -p ~/Projects/" + arguments.at(2) + "/bin/").c_str());
					const char *homedir;
					if((homedir = getenv("HOME")) == NULL) {
	   	 				homedir = getpwuid(getuid())->pw_dir;
					}
					build_sheet.open(std::string(homedir) + "/Projects/" + arguments.at(2) + "/build.sh", std::ios_base::out);
					run_sheet.open(std::string(homedir) + "/Projects/" + arguments.at(2) + "/run.sh", std::ios_base::out);
					source_file.open(std::string(homedir) + "/Projects/" + arguments.at(2) + "/src/" + arguments.at(2) + ".arch", std::ios_base::out);

					if(build_sheet.is_open()) {
					 	build_sheet << "#! /bin/bash" << std::endl;
						build_sheet << "OPTIONS=\"\"" << std::endl;
						build_sheet << "PARAMETERS=\"\"" << std::endl;
						build_sheet << "if [ $1 == \"build\" ]\n\tthen" << std::endl;
						build_sheet << "\t\tarchc $OPTIONS ./src/" + arguments.at(2) + ".arch build ./bin/" + arguments.at(2) + " $PARAMETERS" << std::endl;
						build_sheet << "\telif [ $1 == \"check\" ]\n\tthen" << std::endl;
						build_sheet << "\t\t\tarchc $OPTIONS ./src/" + arguments.at(2) + ".arch check" << std::endl;
						build_sheet << "\telif [ $1 == \"convert\" ]\n\tthen" << std::endl;
						build_sheet << "\t\t\tarchc $OPTIONS ./src/" + arguments.at(2) + ".arch convert ./" + arguments.at(2) + ".cpp" << std::endl;
						build_sheet << "fi" << std::endl;
						build_sheet.close();
					} else {
						std::cout << "Fatal Error: Couldn't create build file!" << std::endl;
					}

					if(run_sheet.is_open()) {
						run_sheet << "#! /bin/bash" << std::endl;
						run_sheet << "./bin/" +  arguments.at(2) << std::endl;
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
			std::system("bash build.sh build");
		} else if(arguments.at(1) == "check") {
			std::system("bash build.sh check");
		} else if(arguments.at(1) == "convert") {
			std::system("bash build.sh convert");
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
