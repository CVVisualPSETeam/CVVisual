#ifndef CVVISUAL_MATCH_PEN
#define CVVISUAL_MATCH_PEN

#include <QFrame>
#include <QPen>

#include "../../dbg/dbg.hpp"

namespace cvv
{
namespace qtutil
{

class CVVMatch;

/**
 * @brief this abstract class returns an individual QPen for a CVVMatch.
 */

class MatchSettings : public QFrame
{
	Q_OBJECT

      public:
	/**
	 * @brief MatchPen
	 * @param parent the parent Widget
	 */
	MatchSettings(QWidget *parent) : QFrame(parent){}

	/**
	 * @brief getSettings
	 * @param match a CVVMatch
	 */
	virtual void setSettings(CVVMatch &match) = 0;

public slots:
	/**
	 * @brief this method emits the signal settingsChanged();
	 */
	void updateAll()
		{TRACEPOINT;emit settingsChanged(*this);}

signals:
	/**
	 * @brief this signal will be emitted if the settings changed
	 * and the CVVMatch must update their Pens
	 * @param the setting object which has changed
	 */
	void settingsChanged(MatchSettings &settings);


};
}
}
#endif
