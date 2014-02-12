#include "sobelfilterwidget.hpp"

#include "opencv2/imgproc/imgproc.hpp"

#include <QVBoxLayout>
#include <QLabel>

#include "../dbg/dbg.hpp"
#include "../util/util.hpp"
#include "filterfunctionwidget.hpp"
#include "filterselectorwidget.hpp"

namespace cvv { namespace qtutil{


SobelFilterWidget::SobelFilterWidget(QWidget* parent):
	QWidget{parent},
	dx_{new QSpinBox{}},
	dy_{new QSpinBox{}},
	ksize_{new QComboBox{}},
	borderType_{new QComboBox{}},
	label_{new QLabel{}}
{
	TRACEPOINT;
	//set up elements
	dx_->setRange(0,7);
	dy_->setRange(0,7);
	ksize_->addItem("1");
	ksize_->addItem("3");
	ksize_->addItem("5");
	ksize_->addItem("7");
	ksize_->addItem("CV_SCHARR(-1)");
	ksize_->setCurrentIndex(1);

	borderType_->addItem("BORDER_DEFAULT");
	borderType_->addItem("BORDER_CONSTANT");
	borderType_->addItem("BORDER_REPLICATE");
	borderType_->addItem("BORDER_REFLECT");
	borderType_->addItem("BORDER_REFLECT_101");
	TRACEPOINT;

	//connect
	QObject::connect(dx_,SIGNAL(valueChanged(int)),
			 &(this->signFilterSettingsChanged_),SIGNAL(signal()));
	QObject::connect(dy_,SIGNAL(valueChanged(int)),
			 &(this->signFilterSettingsChanged_),SIGNAL(signal()));
	QObject::connect(ksize_,SIGNAL(currentIndexChanged(int)),
			 &(this->signFilterSettingsChanged_),SIGNAL(signal()));
	QObject::connect(borderType_,SIGNAL(currentIndexChanged(int)),
			 &(this->signFilterSettingsChanged_),SIGNAL(signal()));
	TRACEPOINT;

	//build ui
	auto lay=util::make_unique<QVBoxLayout>();
	lay->addWidget(util::make_unique<QLabel>("dx").release());
	lay->addWidget(dx_);
	lay->addWidget(util::make_unique<QLabel>("dy").release());
	lay->addWidget(dy_);
	lay->addWidget(util::make_unique<QLabel>("ksize").release());
	lay->addWidget(ksize_);
	lay->addWidget(util::make_unique<QLabel>("borderType").release());
	lay->addWidget(borderType_);
	lay->addWidget(label_);
	setLayout(lay.release());
	TRACEPOINT;
	//emit first update
	this->signFilterSettingsChanged_.emitSignal();
	TRACEPOINT;
}


void SobelFilterWidget::applyFilter(InputArray in,OutputArray out) const
{
	TRACEPOINT;
	int ksize=3;
	switch(ksize_->currentIndex()){
		case 0: ksize=1;break;
		case 1: ksize=3;break;
		case 2: ksize=5;break;
		case 3: ksize=7;break;
		case 4: ksize=CV_SCHARR;break;
	}

	TRACEPOINT;
	int borderType=cv::BORDER_DEFAULT;
	switch(borderType_->currentIndex()){
		case 0: borderType=cv::BORDER_DEFAULT;    break;
		case 1: borderType=cv::BORDER_CONSTANT;   break;
		case 2: borderType=cv::BORDER_REPLICATE;  break;
		case 3: borderType=cv::BORDER_REFLECT;    break;
		case 4: borderType=cv::BORDER_REFLECT_101;break;
	}

	TRACEPOINT;
	const cv::Mat& iar=in.at(0).get();
	cv::Mat& oar=out.at(0).get();
	int dx=dx_->value();
	int dy=dy_->value();

	DEBUGF(400,"\n%s    %s    %s    %s\n",dx,dy,ksize,borderType);

	TRACEPOINT;
	try
	{
		Sobel(iar,oar,-1,dx,dy,ksize,1,0,borderType);
		label_->setText("");
		TRACEPOINT;
	}catch(...)
	{
		label_->setText("The filter had an error.");
		TRACEPOINT;
	}
	TRACEPOINT;
}

std::pair<bool, QString> SobelFilterWidget::checkInput(InputArray) const
{
	TRACEPOINT;
	int dx=dx_->value();
	int dy=dy_->value();
	if(dx==0 && dy==0)
	{
		TRACEPOINT;
		return {false,"dx=0 and dy=0"};
	}
	//dx,dy<ksize, if sharr:  dx XOR dy
	if(dx_->value()==0 && dy_->value()==0)
	{
		TRACEPOINT;
		return {false,"dx=0 and dy=0"};
	}

	int ksize=3;
	switch(ksize_->currentIndex()){
		case 0: ksize=1;break;
		case 1: ksize=3;break;
		case 2: ksize=5;break;
		case 3: ksize=7;break;
		case 4: ksize=CV_SCHARR;break;
	}

	if(ksize==CV_SCHARR){
		if(dx+dy!=1)
		{
			TRACEPOINT;
			return {false,"ksize=CV_SCHARR but dx+dy != 1"};
		}
	}else{
		if((dx>=3 || dy>=3)&&(dx>=ksize || dy>=ksize))
		{
			TRACEPOINT;
			return {false,"dx or dy is to big"};
		}
	}

	TRACEPOINT;
	return {true,""};
}

 void registerSobel()
{
	FilterSelectorWidget<1,1>::registerElement(
		"sobel",
		[](QWidget* parent){
		return std::unique_ptr<FilterFunctionWidget<1,1>>{new SobelFilterWidget{parent}};}
	);
}

}}
