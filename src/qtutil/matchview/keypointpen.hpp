#ifndef CVVISUAL_KEYPOINT_PEN
#define CVVISUAL_KEYPOINT_PEN

#include <QWidget>
#include <QPen>


#include "../../dbg/dbg.hpp"

namespace cvv{ namespace qtutil{

class CVVKeyPoint;

/**
 * @brief this abstract class returns an individual QPen for a CVVKeyPoint.
 */
class KeyPointPen:public QWidget{
Q_OBJECT

public:
	/**
	 * @brief KeyPointPen
	 * @param parent the parent Widget
	 */
	KeyPointPen(QWidget* parent):QWidget(parent){TRACEPOINT;}

	/**
	 * @brief getPen
	 * @param key a CVVKeyPoint
	 * @return an indivudual QPen for the given CVVKeyPoint
	 */
	virtual QPen getPen(const CVVKeyPoint& key)const =0;

signals:
	/**
	 * @brief this signal will be emitted if the settings changed
	 * and the CVVKeyPoint must update their Pens
	 */
	void settingsChanged(const KeyPointPen&);
};

}}
#endif
