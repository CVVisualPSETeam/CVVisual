#include "../../src/qtutil/util.hpp"

#include <iostream>
#include<typeinfo>

#include<QHBoxLayout>
#include<QWidget>
#include<QApplication>
#include<QLabel>

#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	int dim=100;

	std::cout<<"will now create mats \n";
	cv::Mat mGrey{dim,dim,	CV_8UC1,cv::Scalar{212}};

	cv::Mat mR{dim,dim,	CV_8SC3,cv::Scalar{0,0,255}};
	cv::Mat mG{dim,dim,	CV_16UC3,cv::Scalar{0,65255,0}};
	cv::Mat mB{dim,dim,	CV_16SC3,cv::Scalar{32255,0,0}};
	cv::Mat mBGRA{dim,dim,	CV_32SC4,cv::Scalar{5000,-10000,25500,12500}};
	cv::Mat mBG{dim,dim,	CV_32FC2,cv::Scalar{0.41,0.55}};
	cv::Mat mRGB{dim,dim,	CV_64FC3,cv::Scalar{0.123,0.8123,0.7123}};

	std::cout<<"done all\n";
	std::cout<<"will now convert \n";

	auto cGrey = cvv::qtutil::convertMatToQImage(mGrey);
	std::cout<<"done 1\n";

	auto cR    = cvv::qtutil::convertMatToQImage(mR);
	auto cG    = cvv::qtutil::convertMatToQImage(mG);
	auto cB    = cvv::qtutil::convertMatToQImage(mB);
	auto cBGRA = cvv::qtutil::convertMatToQImage(mBGRA);
	auto cBG   = cvv::qtutil::convertMatToQImage(mBG);
	auto cRGB  = cvv::qtutil::convertMatToQImage(mRGB);

	std::cout<<"done all\n";
	std::cout<<"will now test result \n";

	std::cout<<(cGrey.first==cvv::qtutil::ImageConversionResult::SUCCESS)<<"\n";
	std::cout<<   (cR.first==cvv::qtutil::ImageConversionResult::SUCCESS)<<"\n";
	std::cout<<   (cG.first==cvv::qtutil::ImageConversionResult::SUCCESS)<<"\n";
	std::cout<<   (cB.first==cvv::qtutil::ImageConversionResult::SUCCESS)<<"\n";
	std::cout<<(cBGRA.first==cvv::qtutil::ImageConversionResult::SUCCESS)<<"\n";
	std::cout<<  (cBG.first==cvv::qtutil::ImageConversionResult::SUCCESS)<<"\n";
	std::cout<< (cRGB.first==cvv::qtutil::ImageConversionResult::SUCCESS)<<"\n";

	std::cout<<"done all\n";
	std::cout<<"will now print formats 0=Invalid, 3=Indexed8, 5=ARGB32, 13=RGB888 \n";

	std::cout<<cGrey.second.format()<<"\n";
	std::cout<<   cR.second.format()<<"\n";
	std::cout<<   cG.second.format()<<"\n";
	std::cout<<   cB.second.format()<<"\n";
	std::cout<<cBGRA.second.format()<<"\n";
	std::cout<<  cBG.second.format()<<"\n";
	std::cout<< cRGB.second.format()<<"\n";

	QWidget w{};
	QVBoxLayout* lb{new QVBoxLayout{}};
	w.setLayout(lb);
	QHBoxLayout* l{new QHBoxLayout{}};
	lb->addLayout(l);

	QLabel* l1{new QLabel{}};
	QLabel* l2{new QLabel{}};
	QLabel* l3{new QLabel{}};
	QLabel* l4{new QLabel{}};
	QLabel* l5{new QLabel{}};
	QLabel* l6{new QLabel{}};
	QLabel* l7{new QLabel{}};

	l1->setPixmap(QPixmap::fromImage(cGrey.second));
	l2->setPixmap(QPixmap::fromImage(cR.second));
	l3->setPixmap(QPixmap::fromImage(cG.second));
	l4->setPixmap(QPixmap::fromImage(cB.second));
	l5->setPixmap(QPixmap::fromImage(cBGRA.second));
	l6->setPixmap(QPixmap::fromImage(cBG.second));
	l7->setPixmap(QPixmap::fromImage(cRGB.second));

	l->addWidget(l1);
	l->addWidget(l2);
	l->addWidget(l3);
	l->addWidget(l4);
	l->addWidget(l5);
	l->addWidget(l6);
	l->addWidget(l7);

	//now everything with pixmaps
	l=new QHBoxLayout{};
	lb->addLayout(l);

	l1=new QLabel{};
	l2=new QLabel{};
	l3=new QLabel{};
	l4=new QLabel{};
	l5=new QLabel{};
	l6=new QLabel{};
	l7=new QLabel{};

	l1->setPixmap(cvv::qtutil::convertMatToQPixmap(mGrey).second);
	l2->setPixmap(cvv::qtutil::convertMatToQPixmap(mR).second);
	l3->setPixmap(cvv::qtutil::convertMatToQPixmap(mG).second);
	l4->setPixmap(cvv::qtutil::convertMatToQPixmap(mB).second);
	l5->setPixmap(cvv::qtutil::convertMatToQPixmap(mBGRA).second);
	l6->setPixmap(cvv::qtutil::convertMatToQPixmap(mBG).second);
	l7->setPixmap(cvv::qtutil::convertMatToQPixmap(mRGB).second);

	l->addWidget(l1);
	l->addWidget(l2);
	l->addWidget(l3);
	l->addWidget(l4);
	l->addWidget(l5);
	l->addWidget(l6);
	l->addWidget(l7);

	w.show();
	return a.exec();
}

