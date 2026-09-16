#!/bin/sh
# Script to run tests
#
# Version: 20260714

if [ -f "${PWD}/libfvde/.libs/libfvde.1.dylib" ] && [ -f ./pyfvde/.libs/pyfvde.so ]
then
    install_name_tool -change /usr/local/lib/libfvde.1.dylib "${PWD}/libfvde/.libs/libfvde.1.dylib" ./pyfvde/.libs/pyfvde.so
fi

make check-build > /dev/null

# shellcheck disable=SC2068
make check $@
RESULT=$?

if [ ${RESULT} -ne 0 ]
then
    find . -name \*.log -path \*.dir/\*/\*.log -print -exec cat {} \;
fi
exit ${RESULT}

