#ifndef CVVISUAL_KEYPOINT_PEN
#define CVVISUAL_KEYPOINT_PEN

#include <QWidget>
#include <QPen>

#include "cvvkeypoint.hpp"
#include "../../dbg/dbg.hpp"

namespace cvv
{
namespace qtutil
{

class CVVKeyPoint;

/**
 * @brief this abstract class returns an individual QPen for a CVVKeyPoint.
 */
class KeyPointSettings : public QWidget
{
	Q_OBJECT

      public:
	/**
	 * @brief KeyPointPen
	 * @param parent the parent Widget
	 */
	KeyPointSettings(QWidget *parent) : QWidget(parent){}

	/**
	 * @brief getPen
	 * @param key a CVVKeyPoint
	 * @return an indivudual QPen for the given CVVKeyPoint
	 */
	virtual void setSettings(CVVKeyPoint &key) = 0;

public slots:
	/**
	 * @brief this method emits the signal settingsChanged();
	 */
	void updateAll()
		{ emit settingsChanged(*this); }
signals:
	/**
	 * @brief this signal will be emitted if the settings changed
	 * and the CVVKeyPoint must update their Pens
	 */
	void settingsChanged(KeyPointSettings &);
};
}
}
#endif
