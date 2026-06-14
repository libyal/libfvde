#!/bin/sh
# Script to run tests
#
# Version: 20260609

if test -f ${PWD}/libfvde/.libs/libfvde.1.dylib && test -f ./pyfvde/.libs/pyfvde.so
then
	install_name_tool -change /usr/local/lib/libfvde.1.dylib ${PWD}/libfvde/.libs/libfvde.1.dylib ./pyfvde/.libs/pyfvde.so
fi

make check-build > /dev/null

make check $@
RESULT=$?

if test ${RESULT} -ne 0
then
	find . -name \*.log -path \*.dir/\*/\*.log -print -exec cat {} \;
fi
exit ${RESULT}

