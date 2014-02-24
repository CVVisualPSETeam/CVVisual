#include "final_show.hpp"

#include "../../src/dbg/dbg.hpp"


int main() {
	TRACEPOINT;
	cvv::finalShow();
	TRACEPOINT;
	cvv::impl::deleteDataController();
	TRACEPOINT;
}
