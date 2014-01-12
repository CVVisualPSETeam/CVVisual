#! /bin/sh

PARALLEL_JOBS=$(nproc)

if [[ "$#" == 1 ]]; then
	PARALLEL_JOBS=$1
fi

function run_debug_tests() {
	cd debug
	cmake ../..
	make -j$PARALLEL_JOBS && make test
	RETVAL=$?
	cd ..
	return $RETVAL
}

function run_release_tests() {
	cd release
	cmake ../..
	make -j$PARALLEL_JOBS && make test
	RETVAL=$?
	cd ..
	return $RETVAL
}
cd "$(dirname $0)/build"

run_debug_tests && run_release_tests
exit $?
