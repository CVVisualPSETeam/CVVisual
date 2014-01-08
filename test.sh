#! /bin/sh


function run_debug_tests() {
	cd debug
	cmake ../..
	make && make test
	RETVAL=$?
	cd ..
	return $RETVAL
}

function run_release_tests() {
	cd release
	cmake ../..
	make && make test
	RETVAL=$?
	cd ..
	return $RETVAL
}
cd "$(dirname $0)/build"

run_debug_tests && run_release_tests
exit $?
