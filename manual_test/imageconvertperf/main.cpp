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
	std::cout<<"##test: \n";
	std::cout<<"depth "<<depth<<"\t";//typeid();
	std::cout<<"pixel: "<<((w*h)/1000000)<<" M pixels\t";
	auto start=c::now();

	auto res=cvv::qtutil::convertMatToQImage(mat);

	auto end= c::now();
	auto elapsed= end-start;

	std::cout<<"success: "<<(res.first==cvv::qtutil::ImageConversionResult::SUCCESS)<<"\t";

	std::cout<<"time: "<<((elapsed.count()*c::period::num)/(c::period::den/1000))<<"\n";
}

int main()
{
	try{
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
		char c;
		std::cout<<"continue with 100M pixel test? will need about 3,2 gig mem (y)\n";
		std::cin>>c;
		if(c!='y'){return 0;};
		test<CV_64FC4>(10000,10000);
		test<CV_32SC4>(10000,10000);
		//1000M
		//test<CV_64FC4>(100000,10000);
		//test<CV_32SC4>(100000,10000);
		return 0;
	}catch(const std::bad_alloc&){std::cout<<"Out of mem\n";};
	return 0;
}

