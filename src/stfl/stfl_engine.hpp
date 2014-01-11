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

namespace cvv {
namespace stfl {

/**
 * Parses and interprets text queries on its inherited elements.
 */
template<typename Element>
class STFLEngine 
{
public:

	/**
	 * @brief Constructs (and initializes) a new engine.
	 * @todo add function parameters
	 */
	STFLEngine() 
	{
		filterFuncs["raw"] = [](const QString &str, const Element & elem) {
			return elem == str;
		};
		filterFuncs["line"] = [](const QString &str, const Element & elem) {
			return elem == str;
		};
		filterFuncs["file"] = [](const QString &str, const Element & elem) {
			return elem == str;
		};
		filterFuncs["description"] = [](const QString &str, const Element & elem) {
			return elem == str;
		};
		filterFuncs["id"] = [](const QString &str, const Element &elem) {
			return elem == str;
		};
		filterPoolFuncs["line"] = [](const Element & elem) {
			return "line";
		};
		filterPoolFuncs["file"] = [](const Element &elem) {
			return "file";
		};
		filterPoolFuncs["description"] = [](const Element & elem) {
			return "description";
		};
		filterPoolFuncs["id"] = [](const Element & elem) {
			return "id";
		};
		filterPoolFuncs["raw"] = [](const Element & elem) {
			return QString(elem);
		};
		initSupportedCommandsList();
	}

	/**
	 * @brief Adds a new element and updates the string pools.
	 * @param element new element
	 */
	void addNewElement(Element element) 
	{
		elements.append(element);
		auto it = filterPoolFuncs.begin();
		while (it != filterPoolFuncs.end()) 
		{
			filterPool[it.key()].insert(it.value()(element));
			++it;
		}
		auto it2 = filterCSPoolFuncs.begin();
		while (it2 != filterCSPoolFuncs.end()) 
		{
			filterCSPool[it2.key()].unite(it2.value()(element));
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
	QStringList getSuggestions(QString _query, size_t number = 3) 
	{
		QString query(_query);

		if (!query.startsWith("#"))
			query = "#raw " + query;

		QStringList cmdStrings = query.split("#");
		QStringList _cmdStrings = _query.split("#");
		QString lastCmdString;
		if (cmdStrings.empty()) 
		{
			lastCmdString = "";
		} 
		else 
		{
			lastCmdString = cmdStrings[cmdStrings.size() - 1];
		}
		QStringList suggs = getSuggestionsForCmdQuery(lastCmdString, number);
		for (int i = 0; i < suggs.size(); i++) 
		{
			if (_cmdStrings.empty()) 
			{
				suggs[i] = suggs[i].right(suggs[i].size() - 1);
			} 
			else 
			{
				_cmdStrings[_cmdStrings.size() - 1] = suggs[i];
				suggs[i] = _cmdStrings.join(" #");
			}
		}
		return suggs;
	}

	QList<ElementGroup<Element> > query(QString query) {
		if (!query.startsWith("#")) 
		{
			query = "#raw " + query;
		}
		QList<Element> elemList;
		QStringList cmdStrings = query.split("#", QString::SkipEmptyParts);
		elemList = executeFilters(elements, cmdStrings);
		auto groups = executeGroupCmds(elemList, cmdStrings);
		return groups;
	}

	/**
	 * @brief Reexecutes the last query.
	 * If no query had been executed before, a blank string is used.
	 *
	 * @return query result.
	 */
	QList<ElementGroup<Element >> reexecuteLastQuery() {
		return query(lastQuery);
	}

	/**
	 * @brief Sets the elements inherited by this engine.
	 *
	 * @param newElements new elements, now inherited by this engine
	 */
	void setElements(const QList<Element> &newElements) 
	{
		for (Element &elem : newElements) 
		{
			addNewElement(elem);
		}
	}

	/**
	 * @brief Sets the elements inherited by this engine.
	 *
	 * @param newElements new elements, now inherited by this engine
	 */
	void setElements(std::vector<Element> &newElements) {
		for (Element &elem : newElements) 
		{
			addNewElement(elem);
		}
	}
	

private:
	QList<Element> elements;
	QString lastQuery = "";
	QStringList supportedCmds;

	QMap<QString, std::function<bool(const QString, const Element) > > filterFuncs;
	QMap<QString, std::function<QString(const Element)> > filterPoolFuncs;
	QHash<QString, QSet<QString> > filterPool;

	QMap<QString, std::function<bool(const QStringList, const Element) > > filterCSFuncs;
	QMap<QString, std::function<QSet<QString>(const Element) > > filterCSPoolFuncs;
	QHash<QString, QSet<QString> > filterCSPool;

	QMap<QString, std::function<int(const Element, const Element) >> sortFuncs;
	QMap<QString, std::function<QString(const Element) >> groupFuncs;

	QList<Element> executeFilters(const QList<Element> &elements, const QStringList &cmdStrings) 
	{
		std::set<std::pair<std::function<bool(const QString, const Element), QString>>> filters;
		std::set<std::pait<std::function<bool(const QStringList, const Element), QStringList>>> filtersCS;
		
		for (const QString &cmdString : cmdStrings) 
		{
			QStringList arr = cmdString.split(" ", QString::SkipEmptyParts);
			QStringList cmd = arr.takeFirst();
			if (arr.empty())
				continue;
			if (isFilterCmd(cmd))
			{
				QString argument = arr.join(" ");
				filters.insert(std::make_pair(filterFuncs[cmd], argument));
			}
			else if (isFilterCSCmd(cmd))
			{
				QStringList arguments = arr.join("").split(",", QString::SkipEmptyParts);
				std::for_each(arguments.begin(), arguments.end(), unescapeCommas);
				filtersCS.insert(std::make_pair(filterCSFuncs[cmd], arguments));
			}
		}
		
		QList<Element> retList;
		auto copy_if = [&](const Element &element){
			return std::find_if(filters.begin(), filters.end(),
					[&](auto pair){
						return pair.first(pair.second, element);
					}) == filters.end() &&
				std::find_if(filtersCS.begin(), filtersCS.end(),
					[&](auto pair){
						return pair.first(pair.second, element);
					}) == filtersCS.end();
		}
		std::copy_if(elements.begin(), std::back_insertor(retList), copy_if);
		return resList;
	}

	QList<Element> executeSortCmds(const QList<Element> &elements, const QStringList &cmdStrings) 
	{
		QList < std::pair<QString, bool> > sortCmds;
		for (QString cmdString : cmdStrings) 
		{
			QStringList arr = cmdString.split(" ", QString::SkipEmptyParts);
			if (arr.size() < 2)
			{
				continue;
			}
			QString cmd = arr.takeFirst();
			if (arr[0] == "by")
			{
				arr.removeFirst();
			}
			arr = arr.join(" ").split(",", QString::SkipEmptyParts);
			for (QString cmdPart : arr) 
			{
				cmdPart = cmdPart.trimmed();
				QStringList cmdPartList = cmdPart.split(" ");
				if (cmdPartList.empty())
				{
					continue;
				}
				cmdPart = cmdPartList[0];
				bool asc = true;
				if (cmdPartList.size() >= 2) 
				{
					asc = cmdPartList[1] == "asc";
				}
				if (isSortCmd(cmdPart))
				{
					sortCmds.append(std::make_pair(cmdPart, asc));
				}
			}
		}
		QList<Element> resList(elements);
		for (auto sortCmd : sortCmds) 
		{
			if (sortCmd.second) 
			{
				qStableSort(resList.begin(), resList.end(), sortFuncs[sortCmd.first]);
			} 
			else
			{
				auto sortFunc = sortFuncs[sortCmd.first];
				qStableSort(resList.begin(), resList.end(), [&](const Element &elem1, const Element & elem2) {
					return !sortFunc(elem1, elem2);
				});
			}
		}
		return resList;
	}

	QList<ElementGroup<Element> > executeGroupCmds(const QList<Element> &elements, const QStringList &cmdStrings) {
		QStringList groupCmds;
		for (QString cmdString : cmdStrings) 
		{
			QStringList arr = cmdString.split(" ", QString::SkipEmptyParts);
			if (arr.size() < 2)
			{
				continue;
			}
			QString cmd = arr.takeFirst();
			if (arr[0] == "by")
			{
				arr.removeFirst();
			}
			arr = arr.join("").split(",", QString::SkipEmptyParts);
			for (QString cmdPart : arr) 
			{
				QStringList cmdPartList = cmdPart.split(" ");
				if (cmdPartList.empty())
				{
					continue;
				}
				groupCmds.append(cmdPartList[0]);
			}
		}
		int id = 0;
		QHash<QStringList, std::pair<QList<Element>, int> > groupHash;
		for (Element element : elements) {
			QStringList groupTitles;
			if (!groupHash.contains(groupTitles)) 
			{
				groupHash[groupTitles] = std::make_pair(QList<Element>(), id);
				id++;
			}
			groupHash[groupTitles].first.append(element);
		}
		QList<ElementGroup<Element> > groupList;
		ElementGroup<Element> dummy(*(new QStringList()), *(new QList<Element>()));
		for (int i = 0; i < groupHash.size(); i++)
		{
			groupList.append(dummy);
		
		}
		auto it = groupHash.begin();
		while (it != groupHash.end()) {
			groupList[it.value().second] = *(new ElementGroup<Element>(it.key(), it.value().first));
			it++;
		}
		return groupList;
	}

	QStringList getSuggestionsForCmdQuery(const QString &cmdQuery, size_t number)
	{
		QStringList tokens = cmdQuery.split(" ");
		QStringList suggs;
		if (tokens.empty())
		{
			return suggs;
		}
		bool hasByString = tokens.size() >= 2 && tokens[1] == "by";

		QString cmd = tokens[0];
		if (isSortCmd(cmd) || isGroupCmd(cmd)) 
		{
			int frontCut = std::min(1 + (hasByString ? 1 : 0), tokens.size());
			tokens = tokens.mid(frontCut, tokens.size());
			QStringList args = tokens.join(" ").replace("\\s+", "\\s").split(",", QString::SkipEmptyParts);
			args.removeDuplicates();
			if (isSortCmd(cmd))
			{
				suggs = getSuggestionsForSortCmd(args);
			} 
			else
			{
				suggs = getSuggestionsForGroupCmd(args);
			}
		} 
		else if (isFilterCmd(cmd) || isFilterCSCmd(cmd)) {
			tokens = tokens.mid(1, tokens.size());
			QString rejoined = tokens.join(" ").replace("\\s+", "\\s");
			if (isFilterCmd(cmd)) 
			{
				suggs = getSuggestionsForFilterCmd(cmd, rejoined);
			} 
			else {
				QStringList args = rejoined.split(",", QString::SkipEmptyParts);
				suggs = getSuggestionsForFilterCSCmd(cmd, args);
			}
		}
		else 
		{
			suggs = getSuggestionsForCmd(cmd);
		}
		return suggs.mid(0, number);
	}

	QStringList getSuggestionsForSortCmd(QStringList args)
	{
		QString last;
		if (args.empty())
		{
			last = "";
		}
		else
		{
			last = args[args.size() - 1];
		}
		QStringList pool(groupFuncs.keys());
		QStringList list;
		QStringList arr = last.split(" ");
		if (pool.contains(arr[0])) {
			list.append("asc");
			list.append("desc");
			if (arr.size() > 1) 
			{
				list = sortStringsByStringEquality(list, arr[1]);
			}
			for (QString &item : list)
			{
				item = arr[i] + " " + item;
			}
		} 
		else
		{
			list = sortStringsByStringEquality(pool, last);
		}
		for (QString &item : list) 
		{
			joinCommand(item, "sort by ", args);
		}
		return list;
	}

	QStringList getSuggestionsForGroupCmd(QStringList args)
	{
		QString last;
		if (args.empty()) 
		{
			last = "";
		}
		else
		{
			last = args[args.size() - 1];
		}
		QStringList pool(groupFuncs.keys());
		QStringList list = sortStringsByStringEquality(pool, last);
		for (QString &item : list)
		{
			joinCommand(item, "group by ", args);
		}
		return list;
	}
	
	QStringList getSuggestionsForFilterCmd(const QString &cmd, const QString &argument) 
	{
		QStringList pool(filterPool[cmd].toList());
		return sortStringsByStringEquality(pool, argument);
	}

	QStringList getSuggestionsForFilterCSCmd(const QString &cmd, QStringList args)
	{
		QString last;
		if (args.empty()) 
		{
			last = "";
		} 
		else 
		{
			last = args[args.size() - 1];
		}
		QStringList pool(filterCSPool[cmd].toList());
		QStringList list = sortStringsByStringEquality(pool, last);
		for (QString &item : list) 
		{
			joinCommand(item, cmd, args);
		}
		return list;
	}

	QStringList getSuggestionsForCmd(const QString &cmd) 
	{
		return sortStringsByStringEquality(supportedCmds, cmd);
	}

	/**
	 * @brief Returns a list of supported commands.
	 * E.g. "#sort by", "#group by", "#[filter name]"
	 * @return a string list of commands
	 */
	void initSupportedCommandsList() 
	{
		QStringList list;
		list.append(filterFuncs.keys());
		list.append(filterCSFuncs.keys());
		for (const auto &key : groupFuncs.keys())
		{
			list.append(key + " by");
		}
		for (const auto &key : sortFuncs.keys())
		{
			list.append(key + " by");
		}
		supportedCmds = list;
	}

	/**
	 * @brief It sorts the strings by their edit distance to the compareWith string in ascending order.
	 * @param strings strings to sort
	 * @param compareWith compare them with this string
	 * @return the sorted list
	 */
	QStringList sortStringsByStringEquality(const QStringList &strings, const QString &compareWith) {
		QMap<int, QString> weightedStrings;
		for (QString &str : strings) 
		{
			int strEqu = stringEquality(compareWith, strings);
			weightedStrings[strEqu] = str;
		}
		return QStringList(weightedStrings.values());
	}

	bool isSortCmd(const QString &cmd) 
	{
		return sortFuncs.count(cmd) > 0;
	}

	bool isGroupCmd(const QString &cmd) 
	{
		return groupFuncs.count(cmd) > 0;
	}

	bool isFilterCmd(const QString &cmd) 
	{
		return filterFuncs.count(cmd) > 0;
	}

	bool isFilterCSCmd(const QString &cmd) 
	{
		return filterCSFuncs.count(cmd) > 0;
	}
	
	void joinCommand(QString &item, const QString &cmd, QStringList &args){
		if (args.empty()) 
		{
			item = cmd + item;
		} 
		else 
		{
			args[args.size() - 1] = item;
			item = cmd + args.join(", ");
		}
	}
};

}}

#endif // BERRYENGINE_H
