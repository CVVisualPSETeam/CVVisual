#ifndef CVVISUAL_MATCH_PEN
#define CVVISUAL_MATCH_PEN

#include <QWidget>
#include <QPen>


#include "../../dbg/dbg.hpp"

namespace cvv{ namespace qtutil{

class CVVMatch;

/**
 * @brief this abstract class returns an individual QPen for a CVVMatch.
 */

class MatchPen:public QWidget{
Q_OBJECT

public:
	/**
	 * @brief MatchPen
	 * @param parent the parent Widget
	 */
	MatchPen(QWidget* parent):QWidget(parent){TRACEPOINT;}

	/**
	 * @brief getPen
	 * @param match a CVVMatch
	 * @return an indivudual QPen for the given CVVMatch
	 */
	virtual QPen getPen(const CVVMatch& match)const =0;

	/**
	 * @brief this method emits the signal settingsChanged();
	 */
	void updateAll()
		{TRACEPOINT;
		emit settingsChanged(*this);
		TRACEPOINT;}

signals:
	/**
	 * @brief this signal will be emitted if the settings changed
	 * and the CVVMatch must update their Pens
	 */
	void settingsChanged(const MatchPen&);
};

}}
#endif
