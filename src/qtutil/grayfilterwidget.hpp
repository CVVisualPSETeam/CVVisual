#ifndef CVVISUAL_GRAYFILTERWIDGET_HPP
#define CVVISUAL_GRAYFILTERWIDGET_HPP



#include <vector>

#include <QVBoxLayout>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QWidget>
#include <QObject>
#include <QString>

#include "opencv2/core/core.hpp"

#include "filterfunctionwidget.hpp"
#include "../dbg/dbg.hpp"

namespace cvv { namespace qtutil {

class GrayFilterWidget: public FilterFunctionWidget<1,1>
{
	Q_OBJECT
public:
	using InputArray  = FilterFunctionWidget<1,1>::InputArray;
	using OutputArray = FilterFunctionWidget<1,1>::OutputArray;

	GrayFilterWidget(QWidget* parent = nullptr);

	virtual void applyFilter(InputArray in,OutputArray out) const override;

	virtual std::pair<bool, QString> checkInput(InputArray) const override;

private slots:
	void setChannel(int n)
	{
		TRACEPOINT;
		setChannel(static_cast<std::size_t>(n));
		TRACEPOINT;
	}

	void setChannel(std::size_t n);

	void setStd();

private:
	QVBoxLayout* layout_;
	QSpinBox* channel_;
	std::vector<QDoubleSpinBox*> chanValues_;
};

void registerGray();

}}

#endif // CVVISUAL_GRAYFILTERWIDGET_HPP
