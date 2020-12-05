#!/bin/sh
# Script to run tests
#
# Version: 20201121

if test -f ${PWD}/libfvde/.libs/libfvde.1.dylib && test -f ./pyfvde/.libs/pyfvde.so;
then
	install_name_tool -change /usr/local/lib/libfvde.1.dylib ${PWD}/libfvde/.libs/libfvde.1.dylib ./pyfvde/.libs/pyfvde.so;
fi

make check CHECK_WITH_STDERR=1;
RESULT=$?;

if test ${RESULT} -ne 0 && test -f tests/test-suite.log;
then
	cat tests/test-suite.log;
fi
exit ${RESULT};

