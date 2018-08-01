#! /bin/bash

if [ "$(whoami)" != "root" ]; then
	echo "Fatal error: You have to be root for installation! Please use sudo!"
	exit 1
fi

if [ ! -f /usr/bin/g++ ]; then
    echo "Fatal error: unmet dependency! g++ can't be found!"
    exit 1
fi

mkdir -p ./bin/

g++ -std=c++14 ./src/compiler/compiler.cpp -o ./bin/archc -no-pie
g++ -std=c++14 ./src/manager/manager.cpp -o ./bin/architech_manager -no-pie

echo "If you are running an update than please type 'y' on the following prompts!" 

mkdir -p /usr/include/architech/
cp -i ./inc/basics.arch /usr/include/architech/basics.arch

cp -i ./bin/archc /usr/bin/archc
chmod +x /usr/bin/archc

cp -i ./bin/architech_manager /usr/bin/architech_manager
chmod +x /usr/bin/architech_manager

if [ -f /usr/include/architech/basics.arch ]; then
    if [ -f /usr/bin/archc ]; then
        if [ -f /usr/bin/architech_manager ]; then
            echo "Installation successfully fineshed!"
            exit 0
        fi
    fi
fi

echo "Installation failed! Please try the process again or contact me on GitHub/Twitter/E-Mail!"


