#ifndef CVVISUAL_CHANGED_PIXELS_WIDGET_HPP
#define CVVISUAL_CHANGED_PIXELS_WIDGET_HPP

#include "../filterfunctionwidget.hpp"

namespace cvv
{
namespace qtutil
{

/**
 * @brief A Comparator that will create a Mat that highlights exactly the changed pixels.
 */
class ChangedPixelsWidget : public FilterFunctionWidget<2, 1>
{
	Q_OBJECT
public:

	ChangedPixelsWidget(QWidget* parent = nullptr);

	void applyFilter(InputArray in, OutputArray out) const override;

	std::pair<bool, QString> checkInput(InputArray in) const override;

};

}
}


#endif
