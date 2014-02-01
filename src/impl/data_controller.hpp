#ifndef CVVISUAL_DATA_CONTROLLER_HPP
#define CVVISUAL_DATA_CONTROLLER_HPP

#include <memory>
#include <vector>

#include "opencv2/core/core.hpp"

#include "call.hpp"
#include "../controller/view_controller.hpp"

namespace cvv { namespace impl {

class DataController {
public:
	DataController() = default;
	
	/**
	 * Add a new call to the calls-list.
	 */
	void addCall(std::unique_ptr<Call> call);
	
	/**
	 * Remove a call.
	 * @throws std::invalid_argument if no such call exists
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
	 * Get the number of currently managed calls.
	 */
	size_t numCalls() const;
	
	/**
	 * Passes control to the View-controller.
	 * 
	 * Returns when the ViewController signals that normal program-execution shall continue.
	 * 
	 */
	void callUI();
	
	/**
	 * @brief Replace the continue-buttons with close-buttons and show the UI.
	 * 
	 * This function is intended to be called directly before main returns after all the actual
	 * work is done.
	 */
	void lastCall();
private:
	controller::ViewController viewController;
	std::vector<std::unique_ptr<Call>> calls;
};

DataController& dataController();

}} // namespaces cvv::impl

#endif
