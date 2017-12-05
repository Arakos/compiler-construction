#!/bin/bash

# Set Colors
Yellow='\E[0;33m'
Green='\E[0;32m'
White='\E[0;39m' 	

# Create Log File
if [ -a /home/Shared_Folder/provision.log ]; then
rm -f /home/Shared_Folder/provision.log
fi
sudo touch /home/Shared_Folder/provision.log
LOG_FILE=/home/Shared_Folder/provision.log
echo -e "Vagrant Provision Log:" >> ${LOG_FILE}

# Create fd3, rdirect stdout to Log and redirect stderr to stdout
exec 3>&1 1>>${LOG_FILE} 2>&1

##################
# Start Provision
##################
echo -e "${Green}Starting provision..." | tee /dev/fd/3
# Set Locale
echo -e "${Yellow}Setting Locale" | tee /dev/fd/3
export LANGUAGE=en_US.UTF-8 
export LANG=de_DE.UTF-8 
export LC_ALL=en_US.UTF-8 
locale-gen en_US.UTF-8
dpkg-reconfigure locales
# Update Packages on the system 
echo -e "${Yellow}Updating packages..." | tee /dev/fd/3
echo -e "${White}"
sudo apt-get update 
# Upgrade Packages on System
echo -e "${Yellow}Upgrading packages..." | tee /dev/fd/3
echo -e "${White}"
sudo apt-get upgrade -y 
# Install build-essential Package
echo -e "${Yellow}Installing build-essential" | tee /dev/fd/3
echo -e "${White}"
sudo apt-get install build-essential
# Install GNU GCC, G++ and Make
echo -e "${Yellow}Installing GNU Tool Chain..." | tee /dev/fd/3
echo -e "${White}"
sudo apt-get install -y gcc
sudo apt-get install -y g++
echo -e "${Yellow}Installing Make..." | tee /dev/fd/3
echo -e "${White}"
sudo apt-get install -y make
# Install Flex Build Dependencies
echo -e "${Yellow}Installing m4, GNU Bison, autoconf, autopoint, automake, gettext, help2man, GNU texinfo, GNU indent, libtool..." | tee /dev/fd/3
echo -e "${White}"
sudo apt-get install -y m4
sudo apt-get install -y bison
sudo apt-get install -y autoconf
sudo apt-get install -y autopoint
sudo apt-get install -y automake
sudo apt-get install -y gettext
sudo apt-get install -y help2man
sudo apt-get install -y texinfo
sudo apt-get install -y indent
sudo apt-get install -y libtool
# Change Directory
echo -e "${Yellow}Changing Directory to Download GNU Flex" | tee /dev/fd/3
echo -e "${White}"
cd /home/
# Download Flex
echo -e "${Yellow}Downloading flex-2.6.4.tar.gz..." | tee /dev/fd/3
echo -e "${White}"
sudo wget https://github.com/westes/flex/files/981163/flex-2.6.4.tar.gz
# Extract Flex
echo -e "${Yellow}Extracting flex-2.6.4.tar.gz..." | tee /dev/fd/3
echo -e "${White}"
sudo tar -xzf flex-2.6.4.tar.gz
sudo rm -f flex-2.6.4.tar.gz
cd flex-2.6.4/
# Build Flex
echo -e "${Yellow}Running autogen.sh..." | tee /dev/fd/3
echo -e "${White}"
sudo ./autogen.sh
echo -e "${Yellow}Running configure..." | tee /dev/fd/3
echo -e "${White}"
sudo ./configure
echo -e "${Yellow}Running make..." | tee /dev/fd/3
echo -e "${White}"
sudo make
echo -e "${Yellow}Running make install..." | tee /dev/fd/3
echo -e "${White}"
sudo make install

echo -e "${Green}All Done! Read the log file in the shared folder if you encounter the problems." | tee /dev/fd/3
#################
# Provision Over
#################
echo -e "Also please run a sanity check using \"which\" on flex, bison and gcc.${White}" | tee /dev/fd/3

