#ifndef CVVISUAL_FALSE_COLOR_MATCH_PEN
#define CVVISUAL_FALSE_COLOR_MATCH_PEN

#include "matchpen.hpp"

namespace cvv{namespace qtutil{

class FalseColorMatchPen: public MatchPen{
public:
	FalseColorMatchPen(double max,double min,QWidget * parent=nullptr);

	virtual QPen getPen(const CVVMatch& match) const override;

public slots:
	void updateMin(double newMin)
		{TRACEPOINT;min_=newMin;TRACEPOINT;}

	void updateMax(double newMax)
		{TRACEPOINT;max_=newMax;TRACEPOINT;}

private slots:
	void emitSignal()
		{TRACEPOINT;emit settingsChanged(*this);}
private:
	double max_;
	double min_;
};
}}
#endif
