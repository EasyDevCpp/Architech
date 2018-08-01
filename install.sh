#! /bin/bash

if [ "$(whoami)" != "root" ]; then
	echo "You have to be root for installation! Please use sudo!"
	exit 1
fi

mkdir -p ./bin/

g++ -std=c++14 ./src/compiler/compiler.cpp -o ./bin/archc -no-pie
g++ -std=c++14 ./src/manager/manager.cpp -o ./bin/architech_manager -no-pie

echo "If you running an update than please choose 'yes' on the following prompts!" 

mkdir -p /usr/include/architech/
cp -i ./inc/basics.arch /usr/include/architech/basics.arch

cp -i ./bin/archc /usr/bin/archc
chmod +x /usr/bin/archc

cp -i ./bin/architech_manager /usr/bin/architech_manager
chmod +x /usr/bin/architech_manager

echo "Installation successfully fineshed!"


