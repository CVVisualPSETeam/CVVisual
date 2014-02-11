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
#include "../qtutil/matinfowidget.hpp"
#include "../qtutil/zoomableimage.hpp"
#include "filter_view.hpp"

namespace cvv {namespace view {

/**
* @brief Enum class for possible types of difference Filters used by
*		applyDiffFilter
*/
enum class DiffFilterType
{
	HUE = 0,
	SATURATION = 1,
	VALUE = 2,
	LUMINANCE=VALUE,
	GRAYSCALE = 3
};

/**
* Original and resulting image are the two images passed with debug call.
* Filtered image refers to an image created by applying a chosen filter to
* both of these images.
*
* @brief Filterview showing original, filtered and result image
*/
class DualFilterView: public FilterView
{
Q_OBJECT
	private:
		std::array<cv::Mat, 2> rawImages_; //< Original and resulting image
		
		std::array<cvv::qtutil::ZoomableImage, 3> zoomImages_;
			//< Original, filtered and resulting image
		
		std::unordered_map<std::string, std::function<cv::Mat(void)>> filterMap_;
			//< Map of all available filters with their names
		
		qtutil::MatInfoWidget* filterImgInfo_;
		
		/**
		* @brief Converts vector of size two to array
		*/
		std::array<cv::Mat, 2> convertToArray(const std::vector<cv::Mat>&) const;
		
		/**
		* @brief Applys given difference filter
		* @return the filtered Image
		*/
		cv::Mat applyDiffFilter(DiffFilterType filterType);
		
		/**
		* @brief Overlays the rawImages
		* @return the overlay image
		*/
		cv::Mat applyOverlayFilter();
		
		/**
		* @brief Checks whether rawImages_ can be processed by this DiffFilter
		*/
		std::pair<bool, QString> checkDiffInput(DiffFilterType filterType) const;
		
		/**
		* @brief Extracts the names of all available filters from filterMap_
		*/
		QStringList extractStringListfromMap() const;
	
	private slots:
		/**
		* @brief Slot invoked when selected Filter changes. Applies newly selected
		*		filter. Updates middle zoomImage and its MatInfoWidget.
		*/
		void updateFilterImg(const QString&);

	public:
		/**
		* Default view is DiffFilter grayscale.
		* @brief Constructs View showing original image, default filtered image and
		*		result image.
		* @param images The original and resulting images as passed by the debug call
		* @param parent The Widget that is to be the parent of the view
		*/
		DualFilterView(std::array<cv::Mat, 2> images, QWidget* parent = nullptr);
		/**
		* Default view is DiffFilter grayscale.
		* @brief Constructs View showing original image, default filtered image and
		*		result image.
		* @param images The original and resulting images as passed by the debug call.
		*		The vector must contain exactly two images.
		* @param parent The Widget that is to be the parent of the view
		*/
		DualFilterView(const std::vector<cv::Mat>& images, QWidget* parent = nullptr);
};



}} //namespaces

#endif
