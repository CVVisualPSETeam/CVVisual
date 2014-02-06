#ifndef CVVISUAL_DUAL_FILTER_VIEW_HPP
#define CVVISUAL_DUAL_FILTER_VIEW_HPP

//STD
#include <string>
#include <unordered_map>
//OpenCV
//QT
#include <QString>
#include <QWidget>
//CVV
//#include "../qtutil/signalslot.hpp"
#include "../qtutil/zoomableimage.hpp"
#include "filter_view.hpp"

namespace cvv {namespace view {


enum class DiffFilterType
{
	HUE = 0,
	SATURATION = 1,
	VALUE = 2,
	LUMINANCE=VALUE,
	GRAYSCALE = 3
};


class DualFilterView: public FilterView
{
Q_OBJECT
	private:
		std::array<cv::Mat, 2> rawImages_;
		std::array<cvv::qtutil::ZoomableImage, 3> zoomImages_;
		std::unordered_map<std::string, std::function<void(void)>> filterMap_;

		std::array<cv::Mat, 2> convertToArray(const std::vector<cv::Mat>&) const;
		void applyDiffFilter(DiffFilterType filterType);
		std::pair<bool, QString> checkDiffInput(DiffFilterType filterType) const;
		QStringList extractStringListfromMap() const;
	
	private slots: void updateFilterImg(const QString&);

	public:
		DualFilterView(std::array<cv::Mat, 2> images, QWidget* parent = nullptr);
		DualFilterView(const std::vector<cv::Mat>& images, QWidget* parent = nullptr);
};



}} //namespaces

#endif
