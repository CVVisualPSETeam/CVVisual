#ifndef CVVISUAL_SYNC_ZOOM_WIDGET
#define CVVISUAL_SYNC_ZOOM_WIDGET

#include <QWidget>
#include <QButtonGroup>

#include "zoomableimage.hpp"

namespace cvv
{
namespace qtutil
{

class SyncZoomWidget : public QWidget
{

	Q_OBJECT

      public:
	SyncZoomWidget(std::vector<ZoomableImage *> images,
	               QWidget *parent = nullptr);

	~SyncZoomWidget()
	{
		buttonGroup_->deleteLater();
	}
      public
slots:

	void selectMaster(int id);
signals:

	void updateArea(QRectF, qreal) const;

      private:
	void disconnectMaster();

	std::vector<ZoomableImage *> images_;
	size_t currentIdx_;
	QButtonGroup *buttonGroup_;
};
}
}

#endif
