#include "../../src/qtutil/filterselectorwidget.hpp"

#include <QLabel>
#include <QVBoxLayout>
#include <QApplication>

#include <iostream>

#include "../../src/util/util.hpp"

class A: public cvv::qtutil::FilterFunctionWidget<1,1>
{
public:
	A(QWidget* parent = nullptr):QWidget{parent}
	{
		QVBoxLayout* lay = new QVBoxLayout{};
		lay->addWidget(new QLabel{"A"});
		setLayout(lay);
	}

	~A(){}

	const std::array<cv::Mat,1>& applyFilter(const std::array<cv::Mat,1>& ,
					std::array<cv::Mat,1>& ) const override
		{throw std::invalid_argument{"cant use a"};}

	std::pair<bool, QString> checkInput(const std::array<cv::Mat,1>& ) const override
		{return {false, "testfilter A."};}
};


class B: public cvv::qtutil::FilterFunctionWidget<1,1>
{
public:
	B(QWidget* parent = nullptr):QWidget{parent}
	{
		QVBoxLayout* lay = new QVBoxLayout{};
		lay->addWidget(new QLabel{"B"});
		setLayout(lay);
	}

	~B(){}

	const std::array<cv::Mat,1>& applyFilter(const std::array<cv::Mat,1>& ,
					std::array<cv::Mat,1>& ) const override
		{throw std::invalid_argument{"cant use B"};}

	std::pair<bool, QString> checkInput(const std::array<cv::Mat,1>& ) const override
		{return {false, "testfilter B."};}
};


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	std::cout<<"add A: "
		<<cvv::qtutil::FilterSelectorWidget<1,1>::registerElement("A",
		[](QWidget* parent)
		{return std::unique_ptr<cvv::qtutil::FilterFunctionWidget<1,1>>{new A{parent}};})
		<<std::endl;
	std::cout<<"add B: "
		<<cvv::qtutil::FilterSelectorWidget<1,1>::registerElement("B",
		[](QWidget* parent)
		{return std::unique_ptr<cvv::qtutil::FilterFunctionWidget<1,1>>{new B{parent}};})
		<<std::endl;

	std::cout<<"elems in cvv::qtutil::FilterSelectorWidget<1,1>:\t"
			<<cvv::qtutil::FilterSelectorWidget<1,1>::registeredElements().size()
			<<std::endl;

	cvv::qtutil::FilterSelectorWidget<1,1> w{};

	w.show();
	return a.exec();
}
