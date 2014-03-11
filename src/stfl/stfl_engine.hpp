#ifndef CVVISUAL_STFLENGINE_HPP
#define CVVISUAL_STFLENGINE_HPP

#include <math.h>
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
#include "../dbg/dbg.hpp"

namespace cvv
{
namespace stfl
{

/**
 * @brief Parses and interprets text queries on its inherited elements.
 * The queries are written in a simple filter language.
 * @see http://cvv.mostlynerdless.de/ref/filterquery-ref.html
 */
template <typename Element> class STFLEngine
{
      public:
	/**
	 * @brief Constructs a new Engine.
	 * @note use the appropriate setters to add filters, etc.
	 */
	STFLEngine()
	{
	}

	/**
	 * @brief Constructs (and initializes) a new engine.
	 *
	 * @param filterFuncs map mapping a filter command to a filter function
	 * @param filterPoolFuncs map mapping a filter command to a filter pool
	 *function
	 * (that returns a value the filter command filters on for a given
	 *element)
	 * @param filterCSFuncs map mapping a filter cs command (it allows comma
	 *separated arguments
	 * to a filter function
	 * @param filterCSPoolFuncs map mapping a filter cs command to a filter
	 *pool function
	 * @param sortFuncs map mapping a sort command to sort function
	 * @param groupFuncs map mapping a group command to a grouping function
	 *(a function returning a grooup name
	 * for a given element)
	 */
	STFLEngine(
	    QMap<QString, std::function<bool(const QString &, const Element &)>>
	        filterFuncs,
	    QMap<QString, std::function<QString(const Element &)>>
	        filterPoolFuncs,
	    QMap<QString, std::function<bool(const QStringList &,
	                                     const Element &)>> filterCSFuncs,
	    QMap<QString, std::function<QSet<QString>(const Element &)>>
	        filterCSPoolFuncs,
	    QMap<QString, std::function<int(const Element &, const Element &)>>
	        sortFuncs,
	    QMap<QString, std::function<QString(const Element &)>> groupFuncs)
	    : filterFuncs{ filterFuncs }, filterPoolFuncs{ filterPoolFuncs },
	      filterCSFuncs{ filterCSFuncs },
	      filterCSPoolFuncs{ filterCSPoolFuncs }, sortFuncs{ sortFuncs },
	      groupFuncs{ groupFuncs }
	{
		TRACEPOINT;
		initSupportedCommandsList();
		TRACEPOINT;
	}

	/**
	 * @brief Adds a new element and updates the string pools.
	 * @param element new element
	 */
	void addNewElement(Element element)
	{
		TRACEPOINT;
		elements.append(element);
		updateFilterPools(element);
		TRACEPOINT;
	}

	/**
	 * @brief Returns a number of suggestions for a given query.
	 * E.g. query = "#sourt" => "#sort"
	 * @param query given query
	 * @param number maximum number of suggestions
	 * @return suggestions for the given query
	 */
	QStringList getSuggestions(QString _query, size_t number = 100)
	{
		TRACEPOINT;
		QString query(_query);
		bool addedRaw = false;
		if (!query.startsWith("#"))
		{
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
		QStringList suggs =
		    getSuggestionsForCmdQuery(lastCmdString, number);
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
		TRACEPOINT;
		return suggs;
	}

	/**
	 * @brief Executes the given query on its elements.
	 * @param query given query
	 * @return the resulting element groups
	 */
	std::vector<ElementGroup<Element>> query(QString query)
	{
		TRACEPOINT;
		lastQuery = query;
		if (!query.startsWith("#"))
		{
			query = "#raw " + query;
		}
		QList<Element> elemList;
		QStringList cmdStrings =
		    query.split("#", QString::SkipEmptyParts);
		elemList = executeFilters(elements, cmdStrings);
		elemList = executeSortCmds(elemList, cmdStrings);
		auto groups = executeGroupCmds(elemList, cmdStrings);
		TRACEPOINT;
		return groups;
	}

	/**
	 * @brief Reexecutes the last query.
	 * @note If no query had been executed before, a blank string is used.
	 *
	 * @return query result.
	 */
	std::vector<ElementGroup<Element>> reexecuteLastQuery()
	{
		return query(lastQuery);
	}

	/**
	 * @brief Adds the new elements to the elements already inherited by
	 *this engine.
	 *
	 * @param newElements new elements
	 */
	void addElements(QList<Element> newElements)
	{
		TRACEPOINT;
		for (Element &elem : newElements)
		{
			addNewElement(elem);
		}
		TRACEPOINT;
	}

	/**
	 * @brief Sets the elements inherited by this engine.
	 *
	 * @param newElements new elements, now inherited by this engine
	 */
	void setElements(QList<Element> newElements)
	{
		TRACEPOINT;
		elements.clear();
		for (Element &elem : newElements)
		{
			addNewElement(elem);
		}
		reinitFilterPools();
		TRACEPOINT;
	}

	/**
	 * @brief Sets the elements inherited by this engine.
	 *
	 * @param newElements new elements, now inherited by this engine
	 */
	void setElements(std::pair<QList<Element>, QList<Element>> newElements)
	{
		TRACEPOINT;
		elements.clear();
		for (Element &elem : newElements.first)
		{
			addNewElement(elem);
		}
		for (Element &elem : newElements.second)
		{
			addNewElement(elem);
		}
		reinitFilterPools();
		TRACEPOINT;
	}

	/**
	 * @brief Sets the filter function for the given filter command.
	 * @param command given filter command
	 * @param func filter function
	 */
	void setFilterFunc(
	    QString command,
	    std::function<bool(const QString &, const Element &)> func)
	{
		TRACEPOINT;
		filterFuncs[command] = func;
		initSupportedCommandsList();
		reinitFilterPools();
		TRACEPOINT;
	}

	/**
	 * @brief Sets the filter pool function for the given filter command.
	 * @param command given filter command
	 * @param func filter pool function
	 */
	void setFilterPoolFunc(QString command,
	                       std::function<QString(const Element &)> func)
	{
		TRACEPOINT;
		filterPoolFuncs[command] = func;
		initSupportedCommandsList();
		reinitFilterPools();
		TRACEPOINT;
	}

	/**
	 * @brief Sets the filter cs function for the given filter command.
	 * A filter cs is a filter that accepts several comma separated
	 * arguments.
	 * @param command given filter command
	 * @param func filter function
	 */
	void setFilterCSFunc(
	    QString command,
	    std::function<bool(const QStringList &, const Element &)> func)
	{
		TRACEPOINT;
		filterCSFuncs[command] = func;
		initSupportedCommandsList();
		reinitFilterPools();
		TRACEPOINT;
	}

	/**
	 * @brief Sets the filter cs pool function for the given filter command.
	 * A filter cs is a filter that accepts several comma separated
	 * arguments.
	 * @param command given filter command
	 * @param func filter pool function
	 */
	void
	setFilterCSPoolFunc(QString command,
	                    std::function<QSet<QString>(const Element &)> func)
	{
		TRACEPOINT;
		filterCSPoolFuncs[command] = func;
		initSupportedCommandsList();
		reinitFilterPools();
		TRACEPOINT;
	}

	/**
	 * @brief Derives a basic filter, group and sort function from the given
	 * function.
	 * @note It's slightly slower than just creating the methods on your on
	 * and adding them
	 * with the appropriate setters.
	 * @param command filter, group and sort command name
	 * @param func function returning a string for an element, which is used
	 * for filtering,
	 * sorting and grouping
	 * @param withFilterCS does the filter allow several, comma separated
	 * arguments?
	 */
	void addStringCmdFunc(QString command,
	                      std::function<QString(const Element &)> func,
	                      bool withFilterCS = true)
	{
		TRACEPOINT;
		if (withFilterCS)
		{
			filterCSFuncs[command] = [func](const QStringList &args,
			                                const Element &elem)
			{ return args.contains(func(elem)); };
			filterCSPoolFuncs[command] = [func](const Element &elem)
			{ return qtutil::createStringSet(func(elem)); };
		}
		else
		{
			filterFuncs[command] = [func](const QString &query,
			                              const Element &elem)
			{ return query == func(elem); };
			filterPoolFuncs[command] = [func](const Element &elem)
			{ return func(elem); };
		}
		sortFuncs[command] = [func](const Element &elem1,
		                            const Element &elem2)
		{ return func(elem1) < func(elem2); };
		groupFuncs[command] = [func](const Element &elem)
		{ return func(elem); };
		initSupportedCommandsList();
		reinitFilterPools();
		TRACEPOINT;
	}

	/**
	 * @brief Derives a basic (int based) filter, group and sort function
	 * from the given function.
	 * @note It's slightly slower than just creating the methods on your on
	 * and adding them
	 * with the appropriate setters.
	 * @param command filter, group and sort command name
	 * @param func function returning an integer for an element, which is
	 * used for filtering,
	 * sorting and grouping
	 * @param withFilterCS does the filter allow several, comma separated
	 * arguments?
	 * @param widthRangeCmd add `[command]_range` (inclusive) range filter?
	 * @note Range filter only works if withFilterCS is true.
	 */
	void addIntegerCmdFunc(QString command,
	                       std::function<int(const Element &)> func,
	                       bool withFilterCS = true,
	                       bool withRangeCmd = true)
	{
		TRACEPOINT;
		if (withFilterCS)
		{
			filterCSFuncs[command] = [func](const QStringList &args,
			                                const Element &elem)
			{ return args.contains(QString::number(func(elem))); };
			filterCSPoolFuncs[command] = [func](const Element &elem)
			{
				return qtutil::createStringSet(
				    QString::number(func(elem)));
			};

			if (withRangeCmd)
			{
				QString range_cmd = command + "_range";
				auto filterFunc = filterCSFuncs[command];
				filterCSFuncs[range_cmd] = [func, filterFunc](
				    const QStringList &args,
				    const Element &elem)
				{
					if (args.size() < 2)
					{
						return true;
					}
					bool ok = true;
					long first = args[0].toLong(&ok);
					long second = args[1].toLong(&ok);
					int intElem = func(elem);
					return !ok || (first <= intElem &&
					               intElem <= second);
				};
				filterCSPoolFuncs[range_cmd] =
				    filterCSPoolFuncs[command];
			}
		}
		else
		{
			filterFuncs[command] = [func](const QString &query,
			                              const Element &elem)
			{ return query.toInt() == func(elem); };
			filterPoolFuncs[command] = [func](const Element &elem)
			{ return QString::number(func(elem)); };
		}
		sortFuncs[command] = [func](const Element &elem1,
		                            const Element &elem2)
		{ return func(elem1) < func(elem2); };
		groupFuncs[command] = [func, command](const Element &elem)
		{ return QString(command + " %1").arg(func(elem)); };
		initSupportedCommandsList();
		reinitFilterPools();
		TRACEPOINT;
	}

	/**
	 * @brief Derives a basic (float based) filter, group and sort function
	 * from the given function.
	 * @note It's slightly slower than just creating the methods on your on
	 * and adding them
	 * with the appropriate setters.
	 * @param command filter, group and sort command name
	 * @param func function returning a float for an element, which is used
	 * for filtering,
	 * sorting and grouping
	 * @param withFilterCS does the filter allow several, comma separated
	 * arguments?
	 * @param widthRangeCmd add `[command]_range` (inclusive) range filter?
	 * @note Range filter only works if withFilterCS is true.
	 */
	void addFloatCmdFunc(QString command,
	                     std::function<float(const Element &)> func,
	                     bool withFilterCS = true, bool withRangeCmd = true)
	{
		TRACEPOINT;
		if (withFilterCS)
		{
			filterCSFuncs[command] = [func](const QStringList &args,
			                                const Element &elem)
			{ return args.contains(QString::number(func(elem))); };
			filterCSPoolFuncs[command] = [func](const Element &elem)
			{
				return qtutil::createStringSet(
				    QString::number(func(elem)));
			};

			if (withRangeCmd)
			{
				QString range_cmd = command + "_range";
				auto filterFunc = filterCSFuncs[command];
				filterCSFuncs[range_cmd] = [func, filterFunc](
				    const QStringList &args,
				    const Element &elem)
				{
					if (args.size() < 2)
					{
						return true;
					}
					bool ok = true;
					long first = args[0].toDouble(&ok);
					long second = args[1].toDouble(&ok);
					float floatElem = func(elem);
					return !ok || (first <= floatElem &&
					               floatElem <= second);
				};
				filterCSPoolFuncs[range_cmd] =
				    filterCSPoolFuncs[command];
			}
		}
		else
		{
			filterFuncs[command] = [func](const QString &query,
			                              const Element &elem)
			{ return query.toFloat() == func(elem); };
			filterPoolFuncs[command] = [func](const Element &elem)
			{ return QString::number(func(elem)); };
		}
		sortFuncs[command] = [func](const Element &elem1,
		                            const Element &elem2)
		{ return func(elem1) < func(elem2); };
		groupFuncs[command] = [func, command](const Element &elem)
		{ return QString(command + " %1").arg(func(elem)); };
		initSupportedCommandsList();
		reinitFilterPools();
		TRACEPOINT;
	}

	/**
	 * @brief Removes the elements that match the given function.
	 * @param matchFunc given match function
	 */
	void removeElements(std::function<bool(const Element &)> matchFunc)
	{
		TRACEPOINT;
		auto newEnd =
		    std::remove_if(elements.begin(), elements.end(), matchFunc);
		elements.erase(newEnd, elements.end());
		reinitFilterPools();
		TRACEPOINT;
	}

      private:
	QList<Element> elements;
	QString lastQuery = "";
	QStringList supportedCmds;
	QMap<QString, std::function<bool(const QString &, const Element &)>>
	filterFuncs;
	QMap<QString, std::function<QString(const Element &)>> filterPoolFuncs;
	QHash<QString, QSet<QString>> filterPool;

	QMap<QString, std::function<bool(const QStringList &, const Element &)>>
	filterCSFuncs;
	QMap<QString, std::function<QSet<QString>(const Element &)>>
	filterCSPoolFuncs;
	QHash<QString, QSet<QString>> filterCSPool;

	QMap<QString, std::function<int(const Element &, const Element &)>>
	sortFuncs;
	QMap<QString, std::function<QString(const Element &)>> groupFuncs;

	QList<Element> executeFilters(const QList<Element> &elements,
	                              const QStringList &cmdStrings)
	{
		TRACEPOINT;
		std::vector<std::function<bool(const Element &)>> filters;

		for (const QString &cmdString : cmdStrings)
		{
			using namespace std::placeholders;
			QStringList arr =
			    cmdString.split(" ", QString::SkipEmptyParts);
			QString cmd = arr.takeFirst();
			if (arr.empty())
				continue;
			if (isFilterCmd(cmd))
			{
				QString argument = arr.join(" ");
				filters.emplace_back(
				    std::bind(filterFuncs[cmd], argument, _1));
			}
			else if (isFilterCSCmd(cmd))
			{
				QStringList arguments = arr.join("").split(
				    ",", QString::SkipEmptyParts);
				std::for_each(arguments.begin(),
				              arguments.end(), [](QString &str)
				{ str.replace("\\,", ","); });
				filters.emplace_back(std::bind(
				    filterCSFuncs[cmd], arguments, _1));
			}
		}
		TRACEPOINT;
		if (filters.empty())
		{
			TRACEPOINT;
			return elements;
		}
		QList<Element> retList;
		// copy if all filters match
		using StringFilter = std::function<bool(const Element &)>;
		TRACEPOINT;
		auto copy_if = [&](const Element &element)
		{
			// find in filters
			auto find_if = [&](StringFilter filter)
			{
				TRACEPOINT;
				return !filter(element);
			};
			auto returnval =
			    std::find_if(filters.begin(), filters.end(),
			                 find_if) == filters.end();
			TRACEPOINT;
			return returnval;
		};
		std::copy_if(elements.begin(), elements.end(),
		             std::back_inserter(retList), copy_if);
		TRACEPOINT;
		return retList;
	}

	QList<Element> executeSortCmds(const QList<Element> &elements,
	                               const QStringList &cmdStrings)
	{
		TRACEPOINT;
		QList<std::pair<QString, bool>> sortCmds;
		for (QString cmdString : cmdStrings)
		{
			QStringList arr =
			    cmdString.split(" ", QString::SkipEmptyParts);
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
					sortCmds.append(
					    std::make_pair(cmdPart, asc));
				}
			}
		}
		TRACEPOINT;
		QList<Element> resList(elements);
		for (auto sortCmd : sortCmds)
		{
			if (sortCmd.second)
			{
				auto sortFunc = sortFuncs[sortCmd.first];
				qStableSort(resList.begin(), resList.end(),
				            [&](const Element &elem1,
				                const Element &elem2)
				{ return sortFunc(elem1, elem2); });
			}
			else
			{
				auto sortFunc = sortFuncs[sortCmd.first];
				qStableSort(resList.begin(), resList.end(),
				            [&](const Element &elem1,
				                const Element &elem2)
				{ return sortFunc(elem2, elem1); });
			}
		}
		TRACEPOINT;
		return resList;
	}

/**
	 * @note I use std::vector here, as QList does strange things...
	 */
	std::vector<ElementGroup<Element> > executeGroupCmds(const QList<Element> &elements,
														 const QStringList &cmdStrings)
	{
		TRACEPOINT;
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
		TRACEPOINT;
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
		if (containsGroupTitleAlteringQuery(cmdStrings))
		{
			auto alterCode = alteringCodeFromQueryList(cmdStrings);
			for (auto it = groups.begin(); it != groups.end(); ++it)
			{
				auto titles = it->first.split("\\|" , QString::SkipEmptyParts);
				ElementGroup<Element> elementGroup(execGroupTitleAltering(titles, alterCode),
												   it->second);
				groupList.push_back(elementGroup);
			}
		}
		else
		{
			for (auto it = groups.begin(); it != groups.end(); ++it)
			{
				ElementGroup<Element> elementGroup(it->first.split("\\|" , QString::SkipEmptyParts),
												   it->second);
				groupList.push_back(elementGroup);
			}
		}
		TRACEPOINT;
		return groupList;
	}

	using GroupTitleAlteringCode = std::pair<std::vector<char>, std::vector<char>>;
	
	QStringList execGroupTitleAltering(QStringList titles, GroupTitleAlteringCode alteringCode)
	{
		for (auto &title : titles)
		{
			title = execGroupTitleAltering(title, alteringCode);
		}
		return titles;
	}
	
	QString execGroupTitleAltering(QString title, GroupTitleAlteringCode alteringCode)
	{
		//Inspired by @see https://github.com/parttimenerd/tiny-bf/blob/master/tinybf.rb
		//The implemented language is inspired on the bf dialect pbrain.
		//To use this language, append "#alter_group_titles [input]|[code]" to your filter
		//query ("[input]|" is optional and don't use this command with non ASCII characters
		//and in this code appends an zero byte to the input).
		//The data registers vector is initialized with the title (a group title).
		//If this program doesn't output anything, the final registers vector (without all zeros) 
		//will be treated as its output. The output (its first 200 characters at maximum)
		//then replaces the given group title (somewhere above in the code).
		//The code is only aloud to run 1.000.000 commands to prevent endless loops.
		//Also important is, that the registers vector is only 40.000 registers big
		//and if the register pointer gets greater than that, it's set to zero.
		//Example: "[some group command] #alter_group_titles ~{70}"
		//         Does replace the first character of each title with an `F`
		//
		//Attention: This is an undocumented and not really tested feature,
		//that was implemented in a sleepless night, therefore use it with care and
		//expect weird things to happen...
		//(My fedora linux system logged me out several time after running a weird piece
		//of code.)
		//
		//A short syntax reference of the used bf dialec:
		//Valid bf (and pbrain) code is also valid altering code, 
		//except that you should not mindlessly use the following characters in your code:
		// `|`, `(`, `)`, `[`, `]`, `{`, `}` and the following:
		// `:`            : calls the procedure with name [value of current register,
		// `(` code `)`   : creates a procedure with name [value of current register]
		//                  the default procedures are:
		//                  0: sets the current zell to zero
		// `~{` number `}`: sets the current register to the (decimal) `number`
		// `"{` chars `}` : places the chars into the registers, starting at the current one
		//                  chars must not contain any sort of brackets 
		// number `c`     : executes `c` number times, only works with the following commands:
		//                    `+`, `-`, `<`, `>`, `.`, `,` and `:`
		// `\`            : push the current register value on the stack
		// `/`            : pop a value from the stack (or zero if the stack is empty) into
		//                  the current register
		// `?(` ifBlock `)` `(` elseBlock `)`: condition, executes `ifBlock` if the current
		//                  register value is greater than zero
									
		if (!checkAlteringCodeSyntax(alteringCode))
		{
			return "Syntax error.";
		}
		if (title.size() > 40000)
		{
			return QString("Title \"%1\"to long.").arg(shortenString(title, 200));
		}
		
		std::vector<char> output;
		std::vector<char> registers(40000, 0);
		std::vector<char> stack;
		std::vector<char> input = alteringCode.first;
		std::vector<char> code = alteringCode.second;
		std::vector<std::vector<char>> procedures(128, std::vector<char>());
		
		//init the default procedures
		procedures[0] = {'~', '{', '0', '}'};
				
		input.push_back(0);
		std::reverse(input.begin(), input.end());
		auto str = title.toStdString();
		auto titleChars = std::vector<char>(str.begin(), str.end());
		std::copy(titleChars.begin(), titleChars.end(), registers.begin());		

		size_t pointer = 0;      //current position in the registers vector
		size_t commandCount = 0;
		
		execGroupTitleAlteringCode(code, registers, stack, input, output,
								   procedures, pointer, commandCount);
		
		if (output.size() > 0)
		{
			return asciiCharVectorToQString(output);
		}
		std::vector<char> newVec;
		std::copy_if(registers.begin(), registers.end(), std::back_inserter(newVec), 
					 [](char c) { return c > 0; });
		if (newVec.size() > 200)
		{
			newVec = std::vector<char>(newVec.begin(), newVec.begin() + 200);
		}
		return asciiCharVectorToQString(newVec);
	}
	
	void execGroupTitleAlteringCode(std::vector<char> &code,
									std::vector<char> &registers,
									std::vector<char> &stack,
									std::vector<char> &input,
									std::vector<char> &output,
									std::vector<std::vector<char>> &procedures,
									size_t &pointer,
									size_t &commandCount
									)
	{
		std::vector<int> bracketStack;
		int codePointer = -1;
		char currentCommand = 0;

		while (++codePointer < code.size() && commandCount++ < 1000000)
		{
			currentCommand = code.at(codePointer);
			char &currentRegister = registers.at(pointer);
			switch (currentCommand)
			{
			case '+':
				if (++registers.at(pointer) > 127)
				{
					registers[pointer] = 0;
				}
				break;
			case '-':
				if (--registers.at(pointer) < 0)
				{
					registers[pointer] = 127;
				}
				break;
			case '.':
				output.push_back(registers.at(pointer));
				break;
			case ',':
				if (input.size() > 0)
				{
					registers[pointer] = input.back();
					input.pop_back();
				}
				break;
			case '<':
				if (--pointer < 0)
				{
					pointer = 0;
				}
				break;
			case '>':
				if (registers.size() <= ++pointer)
				{
					pointer = 0;
				}
				break;
			case '[':
				bracketStack.push_back(codePointer);
				break;
			case ']':
				if (registers.at(pointer) > 0)
				{
					codePointer = bracketStack.back();
				}
				else
				{
					bracketStack.pop_back();
				}
				break;
			case '(':  //add procedure
					procedures[currentRegister] = alteringCodeCodeBlock(code, codePointer);
				break;
			case ':':  //call procedure
				execGroupTitleAlteringCode(procedures.at(registers.at(pointer)),
										   registers, stack, input, output, procedures,
										   pointer, commandCount);
				break;
			case '~': //set the current register to the given value with a "~{9}" like syntax
				DEBUG(code);
				DEBUG(codePointer);
				if (code.size() > codePointer + 3 && code.at(codePointer + 1) == '{')
				{
					DEBUG(code);
					DEBUG(codePointer);
					size_t number = 0;
					size_t i = codePointer + 2;
					while (i < code.size() && code.at(i) >= '0' && code.at(i) <= '9')
					{
						number = number * 10 + code.at(i) - '0';
						i++;
					}
					while (i < code.size() && code.at(i) != '}')
					{
						i++;
					}
					if (i < code.size() && code.at(i) == '}')
					{
						registers[pointer] = number % 128;
					}
					codePointer = i;
					DEBUG(codePointer);
				}
				break;
			case '"': //syntax '"{chars}"
				DEBUG(code);
				DEBUG(codePointer);
				if (code.size() > codePointer + 3 && code.at(codePointer + 1) == '{')
				{
					size_t codeI = codePointer + 2;
					size_t registerI = pointer;
					while (codeI < code.size() && code.at(codeI) != '}')
					{
						if (registerI < registers.size())
						{
							registers[registerI] = code.at(codeI);
						}
						codeI++;
						registerI++;
					}
					codePointer = codeI;
					DEBUG(codePointer);
				}
				break;
			case '?': //condition, syntax "?()()"
				if (code.size() > codePointer + 4 && code.at(codePointer + 1) == '(')
				{
					codePointer++;
					auto ifBlock = alteringCodeCodeBlock(code, codePointer);
					std::vector<char> elseBlock;
					if (code.size() > codePointer + 1 && code.at(codePointer) == '(')
					{
						elseBlock = alteringCodeCodeBlock(code, codePointer);
					}
					std::vector<char> block = ifBlock;
					if (registers.at(pointer) == 0)
					{
						block = elseBlock;
					}
					execGroupTitleAlteringCode(block, registers, stack, input, output,
											   procedures, pointer, commandCount);
				}
				break;
			case '\\': //push
				stack.push_back(registers.at(pointer));
				break;
			case '/': //pop
				if (stack.empty())
				{
					registers[pointer] = 0;
				}
				else
				{
					registers[pointer] = stack.back();
					stack.pop_back();
				}
				break;
			}
			if (currentCommand >= '0' && currentCommand <= '9') //RUM like repetitions
			{
				size_t number = currentCommand - '0';
				size_t i = codePointer + 1;
				while (i < code.size() && code.at(i) >= '0' && code.at(i) <= '9')
				{
					number = number * 10 + code.at(i) - '0';
				}
				if (i < code.size())
				{
					std::vector<char> acceptableChars = { '+', '-', '<', '>', '.', ',', ':' };
					if (std::find(acceptableChars.begin(), acceptableChars.end(), code.at(i))
						!= acceptableChars.end())
					{
						std::vector<char> repeatedCmd(number, code.at(i));
						execGroupTitleAlteringCode(repeatedCmd, registers, stack, input,
												   output, procedures, pointer, commandCount);
					}
					codePointer = i;
				}
				else
				{
					return;
				}
			}
		}
	}

	std::vector<char> alteringCodeCodeBlock(std::vector<char> &code, int &codePointer)
	{
		//Parse the following code for a code block (`(` code `)`) and return it.
		//The codePointer has to be on the first left paranthesis and will be on the char
		//after the code blocks right paranthesis.
		int br = 0;
		int matchingBracket = -1;
		for (int i = codePointer; i < code.size() && matchingBracket == -1; i++)
		{
			if (code.at(i) == '(')
			{
				br++;
			}
			else if (code.at(i) == ')')
			{
				br--;
				if (br == 0 && matchingBracket == -1)
				{
					matchingBracket = i;
				}
			}
		}
		codePointer = matchingBracket + 1;
		if (matchingBracket - codePointer > 0)
		{
			return std::vector<char>(code.begin() + codePointer + 1,
									 code.begin() + matchingBracket - 1);
		}
		return std::vector<char>();
	};
	
	GroupTitleAlteringCode alteringCodeFromQueryList(const QStringList &cmdStrings)
	{
		std::vector<char> input;
		std::vector<char> code;
		for (const auto& cmdString : cmdStrings)
		{
			auto arr = cmdString.split(" ");
			if (arr.size() > 1 && arr.takeFirst() == "alter_group_titles")
			{
				arr = arr.join(" ").trimmed().split("|");
				if (arr.size() >= 2)
				{
					auto str = arr.at(0).toStdString();
					input = std::vector<char>(str.begin(), str.end());
				}
				if (arr.size() >= 1)
				{
					auto str = arr.at(arr.size() - 1).toStdString();
					code = std::vector<char>(str.begin(), str.end());
				}
				break;
			}
		}
		return std::make_pair(input, code);
	}
	
	bool checkAlteringCodeSyntax(const GroupTitleAlteringCode &code)
	{
		auto codeArr = code.second;
		int leftSquareBrackets = std::count(codeArr.begin(), codeArr.end(), '[');
		int rightSquareBrackets = std::count(codeArr.begin(), codeArr.end(), ']');
		int leftBrackets = std::count(codeArr.begin(), codeArr.end(), '(');
		int rightBrackets = std::count(codeArr.begin(), codeArr.end(), ')');
		int leftCurledBrackets = std::count(codeArr.begin(), codeArr.end(), '{');
		int rightCurledBrackets = std::count(codeArr.begin(), codeArr.end(), '}');
		if (leftBrackets == rightBrackets && leftSquareBrackets == rightSquareBrackets &&
				leftCurledBrackets == rightCurledBrackets)
		{
			int squareBr = 0;
			int curledBr = 0;
			int br = 0;
			for (auto cmd : codeArr)
			{
				switch (cmd)
				{
				case '[':
					squareBr++;
					break;
				case ']':
					if (br > 0 || curledBr > 0)
					{
						return false;
					}
					squareBr--;
					break;
				case '(':
					br++;
					break;
				case ')':
					if (squareBr > 0 || curledBr > 0)
					{
						return false;
					}
					br--;
				}
			}
			return true;
		}
		return false;
	}
	
	bool containsGroupTitleAlteringQuery(const QStringList &cmdStrings)
	{
		for (auto cmdString : cmdStrings)
		{
			if (cmdString.startsWith("alter_group_titles"))
			{
				return true;
			}
		}
		return false;
	}
	
	QStringList getSuggestionsForCmdQuery(const QString &cmdQuery, size_t number)
	{
		TRACEPOINT;
		QStringList tokens = cmdQuery.split(" ");
		QStringList suggs;
		if (tokens.empty())
		{
			return suggs;
		}
		bool hasByString = tokens.size() >= 2 && tokens[1] == "by";
		QString cmd = tokens[0];
		if (cmd == "group" || cmd == "sort")
		{
			int frontCut = std::min(1 + (hasByString ? 1 : 0), tokens.size());
			tokens = cmdQuery.split(" ", QString::SkipEmptyParts)
				.mid(frontCut, tokens.size());
			QStringList args = tokens.join(" ").split(",", QString::SkipEmptyParts);
			args.removeDuplicates();
			for (auto &arg : args)
			{
				int wsLen = 0;
				for (wsLen = 0; wsLen < arg.size() && arg[wsLen] == ' '; wsLen++);
				arg = arg.right(arg.size() - wsLen);
			}
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
		else if (cmd == "alter_group_titles")
		{
			suggs.push_back(cmdQuery);
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
		TRACEPOINT;
		return suggs.mid(0, number);
	}

	QStringList getSuggestionsForSortCmd(QStringList args)
	{
		TRACEPOINT;
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
		if (pool.contains(arr[0]))
		{
			list.append("asc");
			list.append("desc");
			if (arr.size() > 1)
			{
				list = sortStringsByStringEquality(list, arr[1]);
			} else {
				list.prepend("");
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
		TRACEPOINT;
		return list;
	}

	QStringList getSuggestionsForGroupCmd(QStringList args)
	{
		TRACEPOINT;
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
		TRACEPOINT;
		return list;
	}

	QStringList getSuggestionsForFilterCmd(const QString &cmd, const QString &argument)
	{
		TRACEPOINT;
		QStringList pool(filterPool[cmd].toList());
		return sortStringsByStringEquality(pool, argument);
	}

	QStringList getSuggestionsForFilterCSCmd(const QString &cmd, QStringList args)
	{
		TRACEPOINT;
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
		TRACEPOINT;
		return list;
	}

	QStringList getSuggestionsForCmd(const QString &cmd)
	{
		TRACEPOINT;
		return sortStringsByStringEquality(supportedCmds, cmd);
	}
	
	/**
	 * @brief Init the inherited list of supported commands.
	 * E.g. "#sort by", "#group by", "#[filter name]"
	 */
	void initSupportedCommandsList()
	{
		TRACEPOINT;
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
		TRACEPOINT;
	}
	
	void updateFilterPools(Element element)
	{
		TRACEPOINT;
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
		TRACEPOINT;
	}
	
	void reinitFilterPools()
	{
		TRACEPOINT;
		auto it = filterPoolFuncs.begin();
		while (it != filterPoolFuncs.end())
		{
			filterPool[it.key()].clear();
			for (auto element : elements)
			{
				filterPool[it.key()].insert(it.value()(element));
			}
			++it;
		}
		auto it2 = filterCSPoolFuncs.begin();
		while (it2 != filterCSPoolFuncs.end())
		{
			filterCSPool[it2.key()].clear();
			for (auto element : elements)
			{
				filterCSPool[it2.key()].unite(it2.value()(element));
			}
			++it2;
		}
		TRACEPOINT;
	}

	/**
	 * @brief It sorts the strings by their edit distance to the compareWith string in ascending order.
	 * @param strings strings to sort
	 * @param compareWith compare them with this string
	 * @return the sorted list
	 */
	QStringList sortStringsByStringEquality(const QStringList &strings, QString compareWith)
	{
		TRACEPOINT;
		QMap<int, QStringList> weightedStrings;
		auto compareWithWords = compareWith.split(" ", QString::SkipEmptyParts);
		for (const QString &str : strings)
		{
			int strEqu = 0xFFFFFF; //infinity...
			for (auto word : str.split(" ", QString::SkipEmptyParts))
			{
				auto wordA = word.leftJustified(15, ' ');
				for (const auto &word2 : compareWithWords)
				{
					auto wordB = word2.leftJustified(15, ' ');
					int editDist = editDistance(wordA, wordB);
					if (word.startsWith(word2) || word2.startsWith(word)
						|| (word.size() > 2 && (word2.endsWith(word) ||
							word.endsWith(word2))))
					{
						editDist /= 8;
					}
					strEqu = std::min(strEqu, editDist);
				}
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
			list.sort();
			retList.append(list);
		}
		TRACEPOINT;
		return retList;
	}

	bool isSortCmd(const QString &cmd)
	{
		TRACEPOINT;
		return sortFuncs.count(cmd) > 0;
	}

	bool isGroupCmd(const QString &cmd)
	{
		TRACEPOINT;
		return groupFuncs.count(cmd) > 0;
	}

	bool isFilterCmd(const QString &cmd)
	{
		TRACEPOINT;
		return filterFuncs.count(cmd) > 0;
	}

	bool isFilterCSCmd(const QString &cmd)
	{
		TRACEPOINT;
		return filterCSFuncs.count(cmd) > 0;
	}

	void joinCommand(QString &item, const QString &cmd, QStringList args, bool omitCmd = false)
	{
		TRACEPOINT;
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
		TRACEPOINT;
	}
};

}
}

#endif
