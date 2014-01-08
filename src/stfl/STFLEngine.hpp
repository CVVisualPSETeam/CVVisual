#ifndef CVVISUAL_STFLENGINE_HPP
#define CVVISUAL_STFLENGINE_HPP
#include <vector>
#include <QString>
#include <QMap>
#include <QHash>
#include <QSet>
#include <QRegExp>
#include <map>
#include <set>
#include <iostream>
#include <functional>
#include <algorithm>
#include <utility>
#include "ElementGroup.hpp"

/**
 * Parses and interprets text queries on its inherited elements.
 */
template<typename Element>
class STFLEngine {
public:

	STFLEngine() {
		this->filterFuncs["raw"] = [](QString str, const Element & elem) {
			return elem == str;
		};
		this->filterFuncs["line"] = [](QString str, const Element & elem) {
			return elem == str;
		};
		this->filterFuncs["file"] = [](QString str, const Element & elem) {
			return elem == str;
		};
		this->filterFuncs["description"] = [](QString str, const Element & elem) {
			return elem == str;
		};
		this->filterFuncs["id"] = [](QString str, const Element & elem) {
			return elem == str;
		};
		this->filterPoolFuncs["line"] = [](const Element & elem) {
			return "line";
		};
		this->filterPoolFuncs["file"] = [](const Element elem) {
			return "file";
		};
		this->filterPoolFuncs["description"] = [](const Element & elem) {
			return "description";
		};
		this->filterPoolFuncs["id"] = [](const Element & elem) {
			return "id";
		};
		this->filterPoolFuncs["raw"] = [](const Element & elem) {
			return QString(elem);
		};
		this->initSupportedCommandsList();
	}

	/**
	 * @brief Adds a new element and updates the string pools.
	 * @param element new element
	 */
	void addNewElement(Element element) {
		this->elements.append(element);
		auto it = this->filterPoolFuncs.constBegin();
		while (it != this->filterPoolFuncs.constEnd()) {
			this->filterPool[it.key()].insert(it.value()(element));
			++it;
		}
		auto it2 = this->filterCSPoolFuncs.constBegin();
		while (it2 != this->filterCSPoolFuncs.constEnd()) {
			this->filterCSPool[it2.key()].unite(it2.value()(element));
			++it;
		}
	}

	/**
	 * @brief Returns a number of suggestions for a given query.
	 * E.g. query = "#sourt" => "#sort"
	 * @param query given query
	 * @param number maximum number of suggestions
	 * @return suggestions for the given query
	 */
	QStringList getSuggestions(QString _query, size_t number = 3) {
		QString query(_query);

		if (!query.startsWith("#")) {
			query = "#raw " + query;
		}

		QStringList cmdStrings = query.split("#");
		QStringList _cmdStrings = _query.split("#");
		QString lastCmdString;
		if (cmdStrings.empty()) {
			lastCmdString = "";
		} else {
			lastCmdString = cmdStrings[cmdStrings.size() - 1];
		}
		QStringList suggs = this->getSuggestionsForCmdQuery(lastCmdString, number);
		for (int i = 0; i < suggs.size(); i++) {
			if (_cmdStrings.empty()) {
				suggs[i] = suggs[i].right(suggs[i].size() - 1);
			} else {
				_cmdStrings[_cmdStrings.size() - 1] = suggs[i];
				suggs[i] = _cmdStrings.join(" #");
			}
		}
		return suggs;
	}

	QList<ElementGroup<Element> > query(QString query) {
		if (!query.startsWith("#")) {
			query = "#raw " + query;
		}
		QList<Element> elemList;
		QStringList cmdStrings = query.split("#", QString::SkipEmptyParts);
		elemList = this->executeFilters(this->elements, cmdStrings);
		elemList = this->executeFilterCS(elemList, cmdStrings);
		elemList = this->executeSortCmds(elemList, cmdStrings);
		auto groups = this->executeGroupCmds(elemList, cmdStrings);
		return groups;
	}

	/**
	 * @brief Reexecutes the last query.
	 * If no query had been executed before, a blank string is used.
	 *
	 * @return query result.
	 */
	QList<ElementGroup<Element >> reexecuteLastQuery() {
		return this->query(this->lastQuery);
	}

	/**
	 * @brief Sets the elements inherited by this engine.
	 *
	 * @param newElements new elements, now inherited by this engine
	 */
	void setElements(const QList<Element> &newElements) {
		for (Element elem : newElements) {
			this->addNewElement(elem);
		}
	}

	/**
	 * @brief Sets the elements inherited by this engine.
	 *
	 * @param newElements new elements, now inherited by this engine
	 */
	void setElements(std::vector<Element> &newElements) {
		for (Element elem : newElements) {
			this->addNewElement(elem);
		}
	}

	/**
	 * @brief Calculates the equality of two strings.
	 * If both strings are only single words, a combination of the levenshtein edit distance and a phonetic matching algorithm is used.
	 * If not only the first is used.
	 * Attention: using a phonetic algorithm is much slower, than the simple levenshtein.
	 * @param str1 first string
	 * @param str2 second string
	 * @return equality of both strings, 0 means both string are equal, the greater the number, the more unequal are both strings.
	 */
	int stringEquality(QString str1, QString str2) {
		if (this->isSingleWord(str1) && this->isSingleWord(str2)) {
			return phoneticEquality(str1, str2);
		}
		return this->editDistance(str1, str2);
	}

	

private:
	QList<Element> elements;
	QString lastQuery = "";
	QStringList supportedCmds;

	QMap<QString, std::function<bool(QString, const Element) > > filterFuncs;
	QMap<QString, std::function<QString(const Element)> > filterPoolFuncs;
	QHash<QString, QSet<QString> > filterPool;

	QMap<QString, std::function<bool(QStringList, const Element) > > filterCSFuncs;
	QMap<QString, std::function<QSet<QString>(const Element) > > filterCSPoolFuncs;
	QHash<QString, QSet<QString> > filterCSPool;

	QMap<QString, std::function<int(Element, const Element) >> sortFuncs;
	QMap<QString, std::function<QString(const Element) >> groupFuncs;

	QList<Element> executeFilters(const QList<Element> &elements, const QStringList &cmdStrings) {
		QList<Element> resList;
		for (QString cmdString : cmdStrings) {
			QStringList arr = cmdString.split(" ", QString::SkipEmptyParts);
			QStringList cmd = arr.takeFirst();
			QStringList argument = arr.join(" ");
			if (!arr.empty() && this->isFilterCmd(cmd)) {
				for (Element element : elements) {
					if (this->filterFuncs[cmd](argument, element) && !resList.contains(element)) {
						resList.append(element);
					}
				}
			}
		}
		return resList;
	}

	QList<Element> executeFilterCS(const QList<Element> &elements, const QStringList &cmdStrings) {
		QList<Element> resList;
		for (QString cmdString : cmdStrings) {
			QStringList arr = cmdString.split(" ", QString::SkipEmptyParts);
			QString cmd = arr.takeFirst();
			QString argument = arr.join("");
			if (!arr.empty() && this->isFilterCSCmd(cmd)) {
				QStringList args = argument.split(",", QString::SkipEmptyParts);
				for (Element element : elements) {
					if (this->filterCSFuncs[cmd](args, element) && !resList.contains(element)) {
						resList.append(element);
					}
				}
			}
		}
		return resList;
	}

	QList<Element> executeSortCmds(const QList<Element> &elements, const QStringList &cmdStrings) {
		QList < std::pair<QString, bool> > sortCmds;
		for (QString cmdString : cmdStrings) {
			QStringList arr = cmdString.split(" ", QString::SkipEmptyParts);
			if (arr.size() < 2)
				continue;
			QString cmd = arr.takeFirst();
			if (arr[0] == "by")
				arr.removeFirst();
			arr = arr.join(" ").split(",", QString::SkipEmptyParts);
			for (QString cmdPart : arr) {
				cmdPart = cmdPart.trimmed();
				QStringList cmdPartList = cmdPart.split(" ");
				if (cmdPartList.empty())
					continue;
				cmdPart = cmdPartList[0];
				bool asc = true;
				if (cmdPartList.size() >= 2) {
					asc = cmdPartList[1] == "asc";
				}
				if (this->isSortCmd(cmdPart))
					sortCmds.append(std::make_pair(cmdPart, asc));
			}
		}
		QList<Element> resList(elements);
		for (auto sortCmd : sortCmds) {
			if (sortCmd.second) {
				qStableSort(resList.begin(), resList.end(), this->sortFuncs[sortCmd.first]);
			} else {
				auto sortFunc = this->sortFuncs[sortCmd.first];
				qStableSort(resList.begin(), resList.end(), [&](const Element &elem1, const Element & elem2) {
					return !sortFunc(elem1, elem2);
				});
			}
		}
		return resList;
	}

	QList<ElementGroup<Element> > executeGroupCmds(const QList<Element> &elements, const QStringList &cmdStrings) {
		QStringList groupCmds;
		for (QString cmdString : cmdStrings) {
			QStringList arr = cmdString.split(" ", QString::SkipEmptyParts);
			if (arr.size() < 2)
				continue;
			QString cmd = arr.takeFirst();
			if (arr[0] == "by")
				arr.removeFirst();
			arr = arr.join("").split(",", QString::SkipEmptyParts);
			for (QString cmdPart : arr) {
				QStringList cmdPartList = cmdPart.split(" ");
				if (cmdPartList.empty())
					continue;
				groupCmds.append(cmdPartList[0]);
			}
		}
		int id = 0;
		QHash<QStringList, std::pair<QList<Element>, int> > groupHash;
		for (Element element : elements) {
			QStringList groupTitles;
			if (!groupHash.contains(groupTitles)) {
				groupHash[groupTitles] = std::make_pair(QList<Element>(), id);
				id++;
			}
			groupHash[groupTitles].first.append(element);
		}
		QList<ElementGroup<Element> > groupList;
		ElementGroup<Element> dummy(*(new QStringList()), *(new QList<Element>()));
		for (int i = 0; i < groupHash.size(); i++)
			groupList.append(dummy);
		auto it = groupHash.constBegin();
		while (it != groupHash.constEnd()) {
			groupList[it.value().second] = *(new ElementGroup<Element>(it.key(), it.value().first));
			it++;
		}
		return groupList;
	}

	QStringList getSuggestionsForCmdQuery(QString cmdQuery, size_t number) {
		QStringList tokens = cmdQuery.split(" ");
		QStringList suggs;
		if (tokens.empty())
			return suggs;
		bool hasByString = tokens.size() >= 2 && tokens[1] == "by";

		QString cmd = tokens[0];
		if (isSortCmd(cmd) || isGroupCmd(cmd)) {
			int frontCut = std::min(1 + (hasByString ? 1 : 0), tokens.size());
			tokens = tokens.mid(frontCut, tokens.size());
			QStringList args = tokens.join(" ").replace("\\s+", "\\s").split(",", QString::SkipEmptyParts);
			args.removeDuplicates();
			if (isSortCmd(cmd)) {
				suggs = getSuggestionsForSortCmd(args);
			} else {
				suggs = getSuggestionsForGroupCmd(args);
			}
		} else if (isFilterCmd(cmd) || isFilterCSCmd(cmd)) {
			tokens = tokens.mid(1, tokens.size());
			QString rejoined = tokens.join(" ").replace("\\s+", "\\s");
			if (isFilterCmd(cmd)) {
				suggs = getSuggestionsForFilterCmd(cmd, rejoined);
			} else {
				QStringList args = rejoined.split(",", QString::SkipEmptyParts);
				suggs = getSuggestionsForFilterCSCmd(cmd, args);
			}
		} else {
			suggs = getSuggestionsForCmd(cmd);
		}
		return suggs.mid(0, number);
	}

	QStringList getSuggestionsForSortCmd(QStringList args) {
		QString last;
		if (args.empty()) {
			last = "";
		} else {
			last = args[args.size() - 1];
		}
		QStringList pool(this->groupFuncs.keys());
		QStringList list;
		QStringList arr = last.split(" ");
		if (pool.contains(arr[0])) {
			list.append("asc");
			list.append("desc");
			if (arr.size() > 1) {
				list = this->sortStringsByStringEquality(list, arr[1]);
			}
			for (int i = 0; i < list.size(); i++)
				list[i] = arr[i] + " " + list[i];
		} else {
			list = this->sortStringsByStringEquality(pool, last);
		}
		for (int i = 0; i < list.size(); i++) {
			if (args.empty()) {
				list[i] = "sort by " + list[i];
			} else {
				args[args.size() - 1] = list[i];
				list[i] = "sort by " + args.join(", ");
			}
		}
		return list;
	}

	QStringList getSuggestionsForGroupCmd(QStringList args) {
		QString last;
		if (args.empty()) {
			last = "";
		} else {
			last = args[args.size() - 1];
		}
		QStringList pool(this->groupFuncs.keys());
		QStringList list = this->sortStringsByStringEquality(pool, last);
		for (int i = 0; i < list.size(); i++) {
			if (args.empty()) {
				list[i] = "group by " + list[i];
			} else {
				args[args.size() - 1] = list[i];
				list[i] = "group by " + args.join(", ");
			}
		}
		return list;
	}

	QStringList getSuggestionsForFilterCmd(QString cmd, QString argument) {
		QStringList pool(this->filterPool[cmd].toList());
		return this->sortStringsByStringEquality(pool, argument);
	}

	QStringList getSuggestionsForFilterCSCmd(QString cmd, QStringList args) {
		QString last;
		if (args.empty()) {
			last = "";
		} else {
			last = args[args.size() - 1];
		}
		QStringList pool(this->filterCSPool[cmd].toList());
		QStringList list = this->sortStringsByStringEquality(pool, last);
		for (int i = 0; i < list.size(); i++) {
			if (args.empty()) {
				list[i] = cmd + " " + list[i];
			} else {
				args[args.size() - 1] = list[i];
				list[i] = cmd + " " + args.join(", ");
			}
		}
		return list;
	}

	QStringList getSuggestionsForCmd(QString cmd) {
		return sortStringsByStringEquality(this->supportedCmds, cmd);
	}

	/**
	 * @brief Returns a list of supported commands.
	 * E.g. "#sort by", "#group by", "#[filter name]"
	 * @return a string list of commands
	 */
	void initSupportedCommandsList() {
		QStringList list;
		list.append(this->filterFuncs.keys());
		list.append(this->filterCSFuncs.keys());
		auto it = this->groupFuncs.constBegin();
		while (it != this->groupFuncs.constEnd()) {
			list.append(it.key() + " by");
			++it;
		}
		auto it2 = this->sortFuncs.constBegin();
		while (it2 != this->sortFuncs.constEnd()) {
			list.append(it2.key() + " by ");
			++it2;
		}
		this->supportedCmds = list;
	}

	/**
	 * @brief It sorts the strings by their edit distance to the compareWith string in ascending order.
	 * @param strings strings to sort
	 * @param compareWith compare them with this string
	 * @return the sorted list
	 */
	QStringList sortStringsByStringEquality(QStringList strings, QString compareWith) {
		QMap<int, QString> weightedStrings;
		for (auto it = strings.begin(); it != strings.end(); it++) {
			int strEqu = this->stringEquality(compareWith, *it);
			weightedStrings[strEqu] = *it;
		}
		QStringList retList;
		for (QString string : weightedStrings) {
			retList.append(string + "[" + QString::number(this->stringEquality(string, compareWith)) + "]");
		}
		return retList;
	}

	bool isSortCmd(QString cmd) {
		return sortFuncs.count(cmd) > 0;
	}

	bool isGroupCmd(QString cmd) {
		return groupFuncs.count(cmd) > 0;
	}

	bool isFilterCmd(QString cmd) {
		return filterFuncs.count(cmd) > 0;
	}

	bool isFilterCSCmd(QString cmd) {
		return filterCSFuncs.count(cmd) > 0;
	}
};

#endif // BERRYENGINE_H
