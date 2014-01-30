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
try{
	QApplication a(argc, argv);

	int w=10;
	int h=1000;

	std::cout<<"will now create mats \n";
	cv::Mat mGrey{h+1,w+1,	CV_8UC1,cv::Scalar{212}};
	cv::Mat mR{   h+2,w+2,	CV_8SC3,cv::Scalar{0,0,255}};//CV_8SC3
	cv::Mat mG{   h+3,w+3,	CV_16UC3,cv::Scalar{0,65255,0}};
	cv::Mat mB{   w+4,h+4,	CV_16SC3,cv::Scalar{32255,0,0}};
	cv::Mat mBGRA{h+5,w+5,	CV_32SC4,cv::Scalar{5000,-10000,25500,12500}};
	cv::Mat mBG{  w+6,h+6,	CV_32FC2,cv::Scalar{0.41,0.55}};
	cv::Mat mRGB{ h+7,w+7,	CV_64FC3,cv::Scalar{0.123,0.8123,0.7123}};

	std::cout<<"done all\n";
	std::cout<<"will now convert \n";

	auto cGrey = cvv::qtutil::convertMatToQImage(mGrey);
	std::cout<<"done 1";
	auto cR    = cvv::qtutil::convertMatToQImage(mR);
	std::cout<<"2";
	auto cG    = cvv::qtutil::convertMatToQImage(mG);
	std::cout<<"3";
	auto cB    = cvv::qtutil::convertMatToQImage(mB);
	std::cout<<"4";
	auto cBGRA = cvv::qtutil::convertMatToQImage(mBGRA);
	std::cout<<"5";
	auto cBG   = cvv::qtutil::convertMatToQImage(mBG);
	std::cout<<"6";
	auto cRGB  = cvv::qtutil::convertMatToQImage(mRGB);
	std::cout<<"7\n";

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

	std::cout<<"create widget \n";
	QWidget wid{};
	std::cout<<"create vbox \n";
	QVBoxLayout* lb{new QVBoxLayout{}};
	std::cout<<"create hbox \n";
	QHBoxLayout* l{new QHBoxLayout{}};

	std::cout<<"set widget layout to vbox\n";
	wid.setLayout(lb);

	std::cout<<"add hbox to vbox \n";
	lb->addLayout(l);

	//std::cout.flush();
	std::cerr<<"will create labels........\n";

	std::cout<<"create labels\n ";
	//std::cout.flush();
	QLabel* l1=new QLabel{};
	std::cout<<"1";
	QLabel* l2=new QLabel{};
	std::cout<<"2";
	QLabel* l3=new QLabel{};
	std::cout<<"3";
	QLabel* l4=new QLabel{};
	std::cout<<"4";
	QLabel* l5{new QLabel{}};
	std::cout<<"5";
	QLabel* l6{new QLabel{}};
	std::cout<<"6";
	QLabel* l7{new QLabel{}};
	std::cout<<"7 => all\n";


	std::cout<<"convert to pixmaps start\n";
	l1->setPixmap(QPixmap::fromImage(cGrey.second));
	std::cout<<"done 1";
	l2->setPixmap(QPixmap::fromImage(cR.second));
	std::cout<<"2";
	l3->setPixmap(QPixmap::fromImage(cG.second));
	std::cout<<"3";
	l4->setPixmap(QPixmap::fromImage(cB.second));
	std::cout<<"4";
	l5->setPixmap(QPixmap::fromImage(cBGRA.second));
	std::cout<<"5";
	l6->setPixmap(QPixmap::fromImage(cBG.second));
	std::cout<<"6";
	l7->setPixmap(QPixmap::fromImage(cRGB.second));
	std::cout<<"7 => all\n";
	std::cout<<"convert to pixmaps end\n";

	l->addWidget(l1);
	l->addWidget(l2);
	l->addWidget(l3);
	l->addWidget(l4);
	l->addWidget(l5);
	l->addWidget(l6);
	l->addWidget(l7);

	std::cout<<"now everything with pixmaps \n";
	l=new QHBoxLayout{};
	lb->addLayout(l);

	l1=new QLabel{};
	l2=new QLabel{};
	l3=new QLabel{};
	l4=new QLabel{};
	l5=new QLabel{};
	l6=new QLabel{};
	l7=new QLabel{};

	std::cout<<"convert to pixmaps start\n";
	l1->setPixmap(cvv::qtutil::convertMatToQPixmap(mGrey).second);
	l2->setPixmap(cvv::qtutil::convertMatToQPixmap(mR).second);
	l3->setPixmap(cvv::qtutil::convertMatToQPixmap(mG).second);
	l4->setPixmap(cvv::qtutil::convertMatToQPixmap(mB).second);
	l5->setPixmap(cvv::qtutil::convertMatToQPixmap(mBGRA).second);
	l6->setPixmap(cvv::qtutil::convertMatToQPixmap(mBG).second);
	l7->setPixmap(cvv::qtutil::convertMatToQPixmap(mRGB).second);
	std::cout<<"convert to pixmaps end\n";

	l->addWidget(l1);
	l->addWidget(l2);
	l->addWidget(l3);
	l->addWidget(l4);
	l->addWidget(l5);
	l->addWidget(l6);
	l->addWidget(l7);

	wid.show();
	return a.exec();
}catch(const std::bad_alloc&){std::cout<<"Out of mem\n";};
return 0;
}

