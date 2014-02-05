#! /bin/bash

PARALLEL_JOBS=$(nproc)

if [[ "$#" == 1 ]]; then
	PARALLEL_JOBS=$1
fi

function run_tests() {
	cd $1
	cmake ../..
	make -j$PARALLEL_JOBS && make test
	RETVAL=$?
	cd ..
	return $RETVAL
}

cd "$(dirname $0)/build"
   	run_tests debug
exit $?
