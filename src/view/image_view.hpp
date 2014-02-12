#ifndef CVVISUAL_IMAGE_VIEW_HPP
#define CVVISUAL_IMAGE_VIEW_HPP

#include <memory>

#include <QString>
#include <QWidget>
#include <QHBoxLayout>

#include <opencv2/core/core.hpp>

#include "../qtutil/accordion.hpp"
#include "../qtutil/matinfowidget.hpp"
#include "../qtutil/zoomableimage.hpp"

namespace cvv {
namespace view {

/**
 * @brief shows one image.
 */
class ImageView: public QWidget
{
Q_OBJECT

signals:

	/**
	 * @brief update left Footer
	 * Signal to update the left side of the footer with newText.
	 * @param newText
	 */
	void updateLeftFooter(const QString &newText);

	/**
	 * @brief update right Footer
	 * Signal to update the right side of the footer with newText.
	 * @param newText
	 */
	void updateRightFoooter(const QString& newText);

public:
	/**
	 * @brief default constructor
	 **/
	ImageView(const cv::Mat& image, QWidget* parent = nullptr): QWidget{parent}
	{
		layout_ = new QHBoxLayout{this};
		accor_ = new qtutil::Accordion{};
		accor_->setMinimumWidth(250);
		accor_->setMaximumWidth(250);

		layout_->addWidget(accor_);

		zoomim_ =new qtutil::ZoomableImage{};
		auto info = util::make_unique<qtutil::MatInfoWidget>(image);

		connect(zoomim_,SIGNAL(updateConversionResult(ImageConversionResult)),info.get(),
			SLOT(updateConvertStatus(ImageConversionResult)));


		connect(info.get(),SIGNAL(getZoom(qreal)),zoomim_,
			SLOT(setZoom(qreal)));


		zoomim_->setMat(image);

		layout_->addWidget(zoomim_);
		accor_->insert("ImageInformation", std::move(info));

		setLayout(layout_);
	}
	
private:
	QHBoxLayout* layout_;
	qtutil::Accordion* accor_;
	qtutil::ZoomableImage* zoomim_;
};

}} //namespaces

#endif
