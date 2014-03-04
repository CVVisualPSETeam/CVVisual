#ifndef CVVISUAL_SINGLE_COLOR_KEY_PEN
#define CVVISUAL_SINGLE_COLOR_KEY_PEN

#include <QColorDialog>
#include <QPen>

#include "keypointsettings.hpp"
#include "../../dbg/dbg.hpp"

namespace cvv
{
namespace qtutil
{

/**
 * This KeyPointPen return for all CVVKeyPoints the same Color,
 * the Color can be choosen by an QColorDialog
 */

class SingleColorKeyPen : public KeyPointSettings
{
	Q_OBJECT
      public:
	/**
	 * @brief the consructor
	 * @param parent the parent Widget
	 */
	SingleColorKeyPen(QWidget *parent = nullptr);

	/**
	 * @brief the destructor
	 */
	~SingleColorKeyPen()
	{
		TRACEPOINT;
		colordia_->deleteLater();
		TRACEPOINT;
	}

	/**
	 * @brief this method returns the same PEn for all CVVKeyPoints
	 * @return the same Pen for all CVVKeyPoint
	 */
	virtual void setSettings(CVVKeyPoint &keypoint) override;
      public
slots:

	/**
	 * @brief this method updates the Color of the Pen which will be
	 * returned in getPen()
	 * @param color the new Color
	 */
	void updateColor(const QColor &color);

      private
slots:
	void colorButtonClicked()
	{
		TRACEPOINT;
		colordia_->show();
		TRACEPOINT;
	}

      private:
	QColorDialog *colordia_;
	QPen pen_;
};
}
}
#endif
