#!/bin/bash
set -e

if [ "$1" == "" ]; then
	echo "Specify environement, for example: $0 esp12e"
	exit 1
fi


~/.atom/packages/platformio-ide/penv/bin/platformio run --target upload --environment $1


