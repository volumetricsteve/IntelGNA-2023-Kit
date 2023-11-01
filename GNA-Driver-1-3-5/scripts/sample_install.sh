#!/bin/bash -e
# SPDX-License-Identifier: GPL-2.0-only
# Copyright(c) 2019-2021 Intel Corporation

GNA_DRV=intel_gna

help() {
    echo 'Compile, load and optionally install intel_gna module into the system'

    echo -e "\nUsage: $0 [OPTION]"
    echo 'OPTIONs:'
    echo '  -i install module into the system'
    echo '  -h this help'
}

install=0

if [ "$#" -gt 0 ]
then
    args=`getopt -u -o ih -- "$@"`
    if test $? != 0
    then
        help
        return
    fi
    set -- $args

    for i
    do
        case "$i" in
            -i) install=1;;
            -h) help;exit;;
        esac
    done
fi

if ! which gcc 1>/dev/null; then
	echo "ERROR: gcc not found" >&2
	exit
fi

if ! which make 1>/dev/null; then
	echo "ERROR: make not found" >&2
	exit
fi

if ! `uname -s | grep -qi 'Linux'`; then
	read -p "Warning: Unsupported kernel. Proceed? " -n 1 -r; echo
	if [[ ! $REPLY =~ ^[Yy]$ ]]; then
		echo $GNA_DRV driver module installation stopped.
		exit
	fi
fi

if ! `uname -r | grep -Eq '^5\.(1[0-2]|[4-9])\.'`; then
	read -p "Warning: Unsupported kernel version. Proceed? " -n 1 -r; echo
	if [[ ! $REPLY =~ ^[Yy]$ ]]; then
		echo $GNA_DRV driver module installation stopped.
		exit
	fi
fi

if `lsmod | grep -q $GNA_DRV`; then
	read -p "$GNA_DRV module already inserted. Remove it and proceed? " -n 1 -r; echo
	if [[ ! $REPLY =~ ^[Yy]$ ]]; then
		echo $GNA_DRV driver module installation stopped.
		exit
	fi
fi

echo Installing $GNA_DRV driver module ...

cd `dirname $0`/../src

make || exit

if `lsmod | grep -q $GNA_DRV`; then
	sudo -u root rmmod $GNA_DRV || exit
fi

if ((install))
then
    sudo -u root make install || exit
    sudo -u root modprobe $GNA_DRV || exit
else
    sudo -u root insmod $GNA_DRV.ko || exit
fi

echo $GNA_DRV driver module installation completed successfully.
