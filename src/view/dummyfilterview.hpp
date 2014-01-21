#ifndef CVVISUAL_DUMMY_FILTER_VIEW
#define CVVISUAL_DUMMY_FILTER_VIEW

#include <QWidget>

#include <opencv2/core/core.hpp>

#include "filter_view.hpp"
namespace cvv{ namespace view
{
class DummyFilterView : public FilterView
{
	Q_OBJECT
public:

	/**
	 * @brief Constructs a DummyFilterView
	 * @param parent the parent of this View
	 **/
	DummyFilterView(QWidget *parent);
	
	/**
	 * This method is used if you cannot use pointer for constructors
	 * @return a new instanz of this class with the given images
	 */
	//static DummyFilterView create(QWidget *parent);
};	
}}//namespaces
#endif
