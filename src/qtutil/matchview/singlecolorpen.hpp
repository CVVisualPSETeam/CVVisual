#ifndef CVVISUAL_SINGLE_COLOR_MATCHPEN
#define CVVISUAL_SINGLE_COLOR_MATCHPEN

#include <QPen>
#include <QColor>
#include <QWidget>
#include <QColorDialog>

#include "matchpen.hpp"
#include "cvvmatch.hpp"
#include "../../dbg/dbg.hpp"

namespace cvv{namespace qtutil{

/**
 * This MatchPen return for all CVVMatches the same Color,
 * the Color can be choosen by an QColorDialog
 */

class SingleColorPen:public MatchPen{
Q_OBJECT
public:
	/**
	 * @brief the constructor
	 * @param parent the parent Widget
	 */
	SingleColorPen(QWidget * parent =nullptr);

	/**
	 * @brief the destructor
	 * the QColorDialog has no parent/layout it must be deleted.
	 */
	~SingleColorPen()
	{
		TRACEPOINT;
		colorDialog_->deleteLater();
		TRACEPOINT;
	}

	/**
	 * @brief return a single Color for all CVVMatch
	 */
	virtual QPen getPen(const CVVMatch&)const override
		{TRACEPOINT;return pen_;}

public slots:

	/**
	 * @brief updates the Color which will be returned in getPen(CVVMAtch&).
	 * @param color a QColor
	 */
	void updateColor(const QColor& color);

protected slots:
	void colorButtonClicked()
		{colorDialog_->show();}
protected:
	QPen pen_;
	QColorDialog *colorDialog_;
};
}}
#endif
