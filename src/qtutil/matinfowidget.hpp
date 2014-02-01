#ifndef CVVISUAL_MAT_INFO_WIDGET
#define CVVISUAL_MAT_INFO_WIDGET

#include <QLabel>
#include <QString>
#include <QWidget>


#include "opencv2/core/core.hpp"

namespace cvv { namespace qtutil{

class MatInfoWidget : public QWidget{
Q_OBJECT
public:
	MatInfoWidget(cv::Mat,QWidget *parent=nullptr);

public slots:
	void updateMat(cv::Mat);
	void updateConvertStatus(QString);
private:
	QLabel *labelConvert;
	QLabel *labelDim;
	QLabel *labelType;
	QLabel *labelChannel;
	QLabel *labelSize;
	QLabel *labelDepth;

};

}}

#endif
