#include "../../src/qtutil/util.hpp"

#include <iostream>
#include<typeinfo>
#include <chrono>


#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"

template<int depth>
void test(int w, int h)
{
	using c=std::chrono::high_resolution_clock;

	cv::Mat mat{w,h,depth,cv::Scalar{0,0,0,0}};
	std::cout<<"################## begin test\n";
	std::cout<<"depth "<<depth<<"\n";//typeid();
	std::cout<<"pixel: "<<((w*h)/1000000)<<" M pixels\n";
	auto start=c::now();

	auto res=cvv::qtutil::convertMatToQImage(mat);

	auto end= c::now();
	auto elapsed= end-start;

	std::cout<<"success: "<<(res.first==cvv::qtutil::ImageConversionResult::SUCCESS)<<"\n";

	std::cout<<"time: "<<((elapsed.count()*c::period::num)/(c::period::den/1000))<<"\n";
	std::cout<<"################## end test\n";
}

int main()
{
	{
		//test wheather qimage is allowed to be large
		int w=100000;
		int h=10000;
		QImage i{w,h,QImage::Format_ARGB32};
		i.fill(99);
		std::cout<<"qt allows images with: "<<((w*h)/1000000)<<" M pixels\n";
	}
	//1M
	test<CV_64FC4>(1000,1000);
	test<CV_32SC4>(1000,1000);
	//10M
	test<CV_64FC4>(10000,1000);
	test<CV_32SC4>(10000,1000);
	//100M
	test<CV_64FC4>(10000,10000);
	test<CV_32SC4>(10000,10000);
	//1000M
	test<CV_64FC4>(100000,10000);
	test<CV_32SC4>(100000,10000);
	return 0;
}

