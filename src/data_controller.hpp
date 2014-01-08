#ifndef CVVISUAL_DATA_CONTROLLER_HPP
#define CVVISUAL_DATA_CONTROLLER_HPP

#include <memory>
#include <vector>

#include "opencv2/core/core.hpp"

#include "call.hpp"

namespace cvv { namespace impl {

// 'forward declaration':
class WindowController{};

class DataController {
public:
	DataController();
	
	/**
	 * Add a new call to the calls-list.
	 */
	void addCall(std::unique_ptr<Call> call);
	
	/**
	 * Remove a call.
	 */
	void removeCall(size_t Id);
	
	/**
	 * Get read-access to a certain call.
	 */
	const Call& getCall(size_t Id) const;
	
	/**
	 * Get read/write-access to a certain call.
	 */
	Call& getCall(size_t Id);
	
	/**
	 * Passes control to the View-controller.
	 * 
	 * Returns when the ViewController signals that normal program-execution shall continue.
	 * 
	 */
	void callUI();
	
private:
	WindowController windowController;
	std::vector<std::unique_ptr<Call>> calls;
};

DataController& dataController();

}} // namespaces cvv::impl

#endif
