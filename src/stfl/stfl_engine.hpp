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
#include <iterator>

#include "stringutils.hpp"
#include "element_group.hpp"
#include "../qtutil/util.hpp"

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
     * @brief Constructs a new Engine.
     * @note use the appropriate setters to add filters, etc.
     */
    STFLEngine(){}

    /**
     * @brief Constructs (and initializes) a new engine.
     * @todo add function parameters
     */
    STFLEngine(QMap<QString, std::function<bool(const QString&, const Element&) > > filterFuncs,
               QMap<QString, std::function<QString(const Element&)> > filterPoolFuncs,
               QMap<QString, std::function<bool(const QStringList&, const Element&) > > filterCSFuncs,
               QMap<QString, std::function<QSet<QString>(const Element&) > > filterCSPoolFuncs,
               QMap<QString, std::function<int(const Element&, const Element&) >> sortFuncs,
               QMap<QString, std::function<QString(const Element&) >> groupFuncs):
        filterFuncs{filterFuncs}, filterPoolFuncs{filterPoolFuncs}, filterCSFuncs{filterCSFuncs},
        filterCSPoolFuncs{filterCSPoolFuncs}, sortFuncs{sortFuncs}, groupFuncs{groupFuncs}
    {
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
            ++it2;
        }
    }

    /**
     * @brief Returns a number of suggestions for a given query.
     * E.g. query = "#sourt" => "#sort"
     * @param query given query
     * @param number maximum number of suggestions
     * @return suggestions for the given query
     */
    QStringList getSuggestions(QString _query, size_t number = 5)
    {
        QString query(_query);
        bool addedRaw = false;
        if (!query.startsWith("#")){
            query = "#raw " + query;
            addedRaw = true;
        }

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
        for (auto &sugg : suggs)
            sugg = sugg.trimmed();
        for (auto &str : _cmdStrings)
            str = str.trimmed();
        for (int i = 0; i < suggs.size(); i++)
        {
            if (_cmdStrings.empty())
            {
                suggs[i] = suggs[i].right(suggs[i].size() - 1);
            }
            else
            {
                _cmdStrings[_cmdStrings.size() - 1] = suggs[i];
                suggs[i] = _cmdStrings.join(" #").trimmed();
            }
            if (addedRaw)
            {
                replaceIfStartsWith(suggs[i], "raw ", "");
            }
        }
        return suggs;
    }

    std::vector<ElementGroup<Element> > query(QString query)

    {
        lastQuery = query;
        if (!query.startsWith("#"))
        {
            query = "#raw " + query;
        }
        QList<Element> elemList;
        QStringList cmdStrings = query.split("#", QString::SkipEmptyParts);
        elemList = executeFilters(elements, cmdStrings);
        elemList = executeSortCmds(elements, cmdStrings);
        auto groups = executeGroupCmds(elemList, cmdStrings);
        return groups;
    }

    /**
     * @brief Reexecutes the last query.
     * If no query had been executed before, a blank string is used.
     *
     * @return query result.
     */
    std::vector<ElementGroup<Element >> reexecuteLastQuery()
    {
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
    void setElements(std::vector<Element> &newElements)
    {
        for (Element &elem : newElements)
        {
            addNewElement(elem);
        }
    }

    void setFilterFunc(QString key, std::function<bool(const QString&, const Element&)> func){
        filterFuncs[key] = func;
        initSupportedCommandsList();
    }

    void setFilterPoolFunc(QString key, std::function<QString(const Element&)> func){
        filterPoolFuncs[key] = func;
        initSupportedCommandsList();
    }

    void setFilterCSFunc(QString key, std::function<bool(const QStringList&, const Element&) >  func){
        filterCSFuncs[key] = func;
        initSupportedCommandsList();
    }

    void setFilterCSPoolFunc(QString key, std::function<QSet<QString>(const Element&) > func){
        filterCSPoolFuncs[key] = func;
        initSupportedCommandsList();
    }

    /**
     * @brief Derives a basic filter, group and sort function from the given function.
     * Slightly slower than just creating the methods on your on and adding them
     * with the appropriate setters.
     * Please call this method only during the initialization of the STFLEngine object
     * with in your code.
     */
    void addStringCmdFunc(QString key, std::function<QString(const Element&)> func, bool withFilterCS = true)
    {
        if (withFilterCS)
        {
            filterCSFuncs[key] = [func](const QStringList& args, const Element &elem)
            {
                    return args.contains(func(elem));
            };
            filterCSPoolFuncs[key] = [func](const Element &elem)
            {
                return qtutil::createStringSet(func(elem));
            };
        }
        else
        {
            filterFuncs[key] = [func](const QString& query, const Element& elem)
            {
                return query == func(elem);
            };
            filterPoolFuncs[key] = [func](const Element& elem)
            {
                return func(elem);
            };
        }
        sortFuncs[key] = [func](const Element &elem1, const Element& elem2)
        {
            return func(elem1) < func(elem2);
        };
        groupFuncs[key] = [func](const Element& elem)
        {
            return func(elem);
        };
        initSupportedCommandsList();
    }

    /**
     * @brief Derives a basic filter, group and sort function from the given function.
     * Slightly slower than just creating the methods on your on and adding them
     * with the appropriate setters.
     * Please call this method only during the initialization of the STFLEngine object
     * with in your code.
     */
    void addIntegerCmdFunc(QString key, std::function<int(const Element&)> func, bool withFilterCS = true)
    {
        if (withFilterCS)
        {
            filterCSFuncs[key] = [func](const QStringList& args, const Element &elem)
            {
                return args.contains(QString::number(func(elem)));
            };
            filterCSPoolFuncs[key] = [func](const Element &elem)
            {
                return qtutil::createStringSet(QString::number(func(elem)));
            };
        }
        else
        {
            filterFuncs[key] = [func](const QString& query, const Element& elem)
            {
                return query.toInt() == func(elem);
            };
            filterPoolFuncs[key] = [func](const Element& elem)
            {
                return QString::number(func(elem));
            };
        }
        sortFuncs[key] = [func](const Element &elem1, const Element& elem2)
        {
            return func(elem1) < func(elem2);
        };
        groupFuncs[key] = [func, key](const Element& elem)
        {
            return QString(key + " %1").arg(func(elem));
        };
        initSupportedCommandsList();
    }

    /**
     * @brief Removes the elements that match the filter function.
     * @brief filterFunc filter function
     * @param deleteIt if true, also deletes the elements
     */
    void removeElements(std::function<bool(const Element&)> filterFunc, bool deleteIt = true)
    {
        QList<Element> newList;
        std::vector<Element> remList;
        for (auto &elem : elements)
        {
            if (filterFunc(elem))
            {
                remList.push_back(elem);
            }
            else
            {
                newList.push_back(elem);
            }
        }
        for (auto &elem : remList)
        {
            if (deleteIt)
                delete &elem;
        }
        elements = newList;
    }

private:
    QList<Element> elements;
    QString lastQuery = "";
    QStringList supportedCmds;
    QMap<QString, std::function<bool(const QString&, const Element&) > > filterFuncs;
    QMap<QString, std::function<QString(const Element&)> > filterPoolFuncs;
    QHash<QString, QSet<QString> > filterPool;

    QMap<QString, std::function<bool(const QStringList&, const Element&) > > filterCSFuncs;
    QMap<QString, std::function<QSet<QString>(const Element&) > > filterCSPoolFuncs;
    QHash<QString, QSet<QString> > filterCSPool;

    QMap<QString, std::function<int(const Element&, const Element&) >> sortFuncs;
    QMap<QString, std::function<QString(const Element&) >> groupFuncs;
    QList<Element> executeFilters(const QList<Element> &elements, const QStringList &cmdStrings)
    {
        std::vector< std::pair< std::function<bool(const QString&, const Element&)>, QString > > filters;
        std::vector< std::pair< std::function<bool(const QStringList&, const Element&)>, QStringList > > filtersCS;

        for (const QString &cmdString : cmdStrings)
        {
            QStringList arr = cmdString.split(" ", QString::SkipEmptyParts);
            QString cmd = arr.takeFirst();
            if (arr.empty())
                continue;
            if (isFilterCmd(cmd))
            {
                QString argument = arr.join(" ");
                filters.push_back(std::make_pair(filterFuncs[cmd], argument));
            }
            else if (isFilterCSCmd(cmd))
            {
                QStringList arguments = arr.join("").split(",", QString::SkipEmptyParts);
                std::for_each(arguments.begin(), arguments.end(), [](QString &str){
                    str.replace("\\,", ",");
                });
                filtersCS.push_back(std::make_pair(filterCSFuncs[cmd], arguments));
            }
        }
        QList<Element> retList;
        //copy if all filters match
        auto copy_if = [&](const Element & element)
        {
            (void)element;
            //find in filters
           auto find_if1 = [&](std::pair< std::function<bool(const QString&, const Element&)>, QString> pair)
            {
                return pair.first(pair.second, element);
            };
            //find in filtersCS
            auto find_if2 = [&](std::pair< std::function<bool(const QStringList&, const Element&)>, QStringList > pair)
            {
                return pair.first(pair.second, element);
            };
            return (filters.size() == 0 || std::find_if(filters.begin(), filters.end(), find_if1) != filters.end())  &&
                (filtersCS.size() == 0 || std::find_if(filtersCS.begin(), filtersCS.end(), find_if2) != filtersCS.end());
        };
        std::copy_if(elements.begin(), elements.end(), std::back_inserter(retList), copy_if);
        return retList;
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
                auto sortFunc = sortFuncs[sortCmd.first];
                qStableSort(resList.begin(), resList.end(), [&](const Element &elem1, const Element & elem2)
                {
                    return sortFunc(elem1, elem2);
                });
            }
            else
            {
                auto sortFunc = sortFuncs[sortCmd.first];
                qStableSort(resList.begin(), resList.end(), [&](const Element &elem1, const Element & elem2)
                {
                    return sortFunc(elem2, elem1);
                });
            }
        }
        return resList;
    }

    /**
     * @note I use std::vector here, as QList does strange things...
     */
    std::vector<ElementGroup<Element> > executeGroupCmds(const QList<Element> &elements, const QStringList &cmdStrings)
    {
        (void)cmdStrings;
        (void)elements;
        QStringList groupCmds;
        for (QString cmdString : cmdStrings)
        {
            QStringList arr = cmdString.split(" ", QString::SkipEmptyParts);
            if (arr.size() < 2)
            {
                continue;
            }
            QString cmd = arr.takeFirst();
            if (cmd != "group")
            {
                continue;
            }
            if (arr[0] == "by")
            {
                arr.removeFirst();
            }
            arr = arr.join("").split(",", QString::SkipEmptyParts);
            for (QString cmdPart : arr)
            {
                QStringList cmdPartList = cmdPart.split(" ");
                if (cmdPartList.empty() || !isGroupCmd(cmdPartList[0]))
                {
                    continue;
                }
                groupCmds.append(cmdPartList[0]);
            }
        }
        std::vector<ElementGroup<Element> > groupList;
        std::map<QString, QList<Element>> groups{};
        for (auto &element : elements)
        {
            QString name = "";
            for (auto &groupCmd : groupCmds)
            {
                name.append("\\|" + groupFuncs[groupCmd](element));
            }
            if (groups.count(name) == 0)
            {
                groups[name] = QList<Element>();
            }
            groups[name].push_back(element);
        }
        for (auto it = groups.begin(); it != groups.end(); ++it)
        {
            ElementGroup<Element> elementGroup(it->first.split("\\|" , QString::SkipEmptyParts), it->second);
            groupList.push_back(elementGroup);
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
        /*bool cmdFollowedBySpace = false;
        if (hasByString && tokens.size() >= 3)
            cmdFollowedBySpace = tokens[2] == "";
        else if (tokens.size() >= 2)
            cmdFollowedBySpace = tokens[1];*/

        QString cmd = tokens[0];
        if (cmd == "group" || cmd == "sort")
        {
            int frontCut = std::min(1 + (hasByString ? 1 : 0), tokens.size());
            tokens = tokens.mid(frontCut, tokens.size());
            QStringList args = tokens.join(" ").replace("\\s+", "\\s").split(",", QString::SkipEmptyParts);
            args.removeDuplicates();
            if (args.empty())
            {
                args.push_back(" ");
            }
            if (cmd == "sort")
            {
                suggs = getSuggestionsForSortCmd(args);
            }
            else
            {
                suggs = getSuggestionsForGroupCmd(args);
            }
        }
        else if (isFilterCmd(cmd) || isFilterCSCmd(cmd))
        {
            tokens = tokens.mid(1, tokens.size());
            QString rejoined = tokens.join(" ");
            if (tokens.empty())
            {
                rejoined = " ";
            }
            if (isFilterCmd(cmd))
            {
                suggs = getSuggestionsForFilterCmd(cmd, rejoined);
            }
            else
            {
                QStringList args = rejoined.split(",", QString::SkipEmptyParts);
                suggs = getSuggestionsForFilterCSCmd(cmd, args);
            }
        }
        else
        {
            suggs = getSuggestionsForCmd(cmd);
        }
        if (isFilterCmd(cmd) || isFilterCSCmd(cmd) || isSortCmd(cmd) || isGroupCmd(cmd))
        {
            for (auto &sugg : suggs)
            {
                sugg = cmd + " " + sugg;
            }
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
        QStringList pool(sortFuncs.keys());
        QStringList list;
        QStringList arr = last.split(" ");
        if (pool.contains(arr[0])) //TODO doesn't work well
        {
            list.append("asc");
            list.append("desc");
            if (arr.size() > 1)
            {
                list = sortStringsByStringEquality(list, arr[1]);
            }
            for (auto &str : list)
            {
                str = arr[0] + " " + str;
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
            joinCommand(item, cmd, args, true);
        }
        return list;
    }

    QStringList getSuggestionsForCmd(const QString &cmd)
    {
        return sortStringsByStringEquality(supportedCmds, cmd);
    }

    /**
     * @brief Init the inherited list of supported commands.
     * E.g. "#sort by", "#group by", "#[filter name]"
     */
    void initSupportedCommandsList()
    {
        QStringList list;
        list.append(filterFuncs.keys());
        list.append(filterCSFuncs.keys());
        for (const auto &key : groupFuncs.keys())
        {
            list.append("group by " + key);
        }
        for (const auto &key : sortFuncs.keys())
        {
            list.append("sort by " + key);
        }
        supportedCmds = list;
    }

    /**
     * @brief It sorts the strings by their edit distance to the compareWith string in ascending order.
     * @param strings strings to sort
     * @param compareWith compare them with this string
     * @return the sorted list
     */
    QStringList sortStringsByStringEquality(const QStringList &strings, QString compareWith)
    {
        QMap<int, QStringList> weightedStrings;
        for (const QString &str : strings)
        {
            int strEqu;
            if (str.startsWith(compareWith) || str.endsWith(compareWith) ||
                    compareWith.startsWith(str) || compareWith.endsWith(str))
            {
                strEqu = editDistance(compareWith, str) / 2;
            }
            else
            {
                strEqu = editDistance(compareWith, str);
            }
            if (!weightedStrings.contains(strEqu))
            {
                weightedStrings[strEqu] = QStringList();
            }
            weightedStrings[strEqu].push_back(str);
        }
        QStringList retList;
        for (auto &list : weightedStrings.values())
        {
            retList.append(list);
        }
        return retList;
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

    void joinCommand(QString &item, const QString &cmd, QStringList args, bool omitCmd = false)
    {
        if (args.size() == 0)
        {
            item = "";
        }
        else
        {
            for (auto &arg : args)
                arg = arg.trimmed();
            args[args.size() - 1] = item;
            item = args.join(", ");
        }
        if (!omitCmd)
        {
            item = cmd + item;
        }
    }
};

}
}

#endif // BERRYENGINE_H
