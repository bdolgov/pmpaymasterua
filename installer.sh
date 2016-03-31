#!/bin/bash

URL="https://github.com/bdolgov/pmpaymasterua/archive/master.tar.gz"
FOLDER="pmpaymasterua"
AFTERUPDATER="https://github.com/bdolgov/core5-afterupdater/archive/master.tar.gz"

set -e

echo "BILLmanager5 pmpaymasterua installer"
echo

if [ -f /etc/redhat-release ]; then
	OS="centos"
	echo "Operating system is CentOS"
	BREL="$(rpm -q --whatprovides billmanager | cut -d- -f2)"
	if [ "$?" = "1" ]; then
		echo "No BILLmanager detected"
		exit 1
	fi
	echo "BILLmanager release is $BREL"
	echo "Installing ISPsystem development packages..."
	yum install -y coremanager-devel billmanager-$BREL-devel make
elif [ -f /etc/debian_version ]; then
	OS="debian"
	echo "Operating system is Debian"
	BREL="$(dpkg-query -f '${binary:Package}\n' -W | grep '^billmanager-[a-z]*$' | cut -d- -f2)"
	if [ -z "$BREL" ]; then
		echo "No BILLmanager detected"
		exit 1
	fi
	echo "BILLmanager release is $BREL"
	echo "Installing ISPsystem development packages..."
	apt-get -y install coremanager-dev billmanager-$BREL-dev
else
	echo "This OS is not yet supported by the installer"
	exit 1
fi

cd /usr/local/mgr5/src

echo "Installing $OS development packages..."
make -f isp.mk $OS-prepare

echo "Downloading plugin..."
wget -O /tmp/plugin.tgz "$URL"
echo "Extracting plugin..."
tar zxf /tmp/plugin.tgz
rm -f /tmp/plugin.tgz
echo "Installing plugin..."
mv "$FOLDER-master" "$FOLDER"
cd "$FOLDER"
make install
touch .autorebuild
cd ..
echo "Downloading autoupdater..."
wget -O /tmp/autoupdater.tgz "$AFTERUPDATER"
echo "Extracting autoupdater..."
tar zxf /tmp/autoupdater.tgz
rm /tmp/autoupdater.tgz
echo "Installing autoupdater..."
cd core5-afterupdater-master
make install MGR=billmgr
killall core || true
cd ..
echo "Finished"
