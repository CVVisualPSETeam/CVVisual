#ifndef CVVISUAL_KEY_POINT_VALUE_CHOOSER
#define CVVISUAL_KEY_POINT_VALUE_CHOOSER

#include <QWidget>
#include <QComboBox>

#include "opencv2/features2d/features2d.hpp"


namespace cvv{ namespace qtutil{

class KeyPointValueChooser:public QWidget{

	Q_OBJECT

public:
	KeyPointValueChooser(QWidget *parent=nullptr);

	double getChoosenValue(cv::KeyPoint keypoint);

signals:

	void valueChanged();

private:
	QComboBox *combBox_;

};

}}

#endif
