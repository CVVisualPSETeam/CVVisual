#include "stringutils.hpp"

#include <algorithm>
#include <cstddef>
#include <numeric>

#include "../util/util.hpp"

size_t editDistance(const QString& str1, const QString& str2)
{
	// this function uses unsigned instead of size_t because Qt has the extremly stupid idea
	// to overload operator[] for int and unsigned, making using it with anything else impossible
	//
	// Qt: shame on you
	
	const unsigned len1 = str1.size();
	const unsigned len2 = str2.size();
	
	std::vector<size_t> col(len2 + 1);
	std::vector<size_t> prevCol(len2 + 1);
	
	std::iota(prevCol.begin(), prevCol.end(), 0);
	
	for (unsigned i = 0; i < len1; i++)
	{
		col[0] = i + 1;
		
		for (unsigned j = 0; j < len2; j++)
		{
			col[j + 1] = std::min({
					1 + col[j],
					1 + prevCol[1 + j],
					// TODO: this looks wrong:
					prevCol[j] + (str1[i] == str2[j]) });
		}
		std::swap(col, prevCol);
	}
	return prevCol[len2];
}

int phoneticEquality(QString word1, QString word2)
{
	if (word1 == word2)
		return 0;
	return editDistance(nysiisForWord(word1), nysiisForWord(word2)) + 1;
}

QString nysiisForWord(QString word)
{
	if (word.size() == 0) {
		return "";
	}
	QString code;
	word = word.toUpper();
	static std::map<QString, QString> replacements = {
		{"MAC", "MCC"},
		{"KN", "NN"},
		{"K", "C"},
		{"PH", "FF"},
		{"PF", "FF"},
		{"SCH", "SSS"}
	};
	replaceIfStartsWith(word, replacements);
	static std::map<QString, QString> replacements2 = {
		{"EE", "Y"},
		{"IE", "Y"},
		{"DT", "D"},
		{"RT", "D"},
		{"NT", "D"},
		{"ND", "D"}
	};
	replaceIfEndsWith(word, replacements2);
	code.append(word[0]);
	word = word.right(word.size() - 1);
	static std::map<QString, QString> replacements3 = {
		{"EV", "AF"},
		{"Ü", "A"},
		{"Ö", "A"},
		{"Ä", "A"},
		{"O", "G"},
		{"Z", "S"},
		{"M", "N"},
		{"KN", "N"},
		{"K", "C"},
		{"SCH", "SSS"},
		{"PH", "FF"}
	};
	while (word.size() > 0) {
		if (isVowel(word[0]))
			word[0] = QChar('A');
		replaceIfStartsWith(word, replacements);
		if (!(word.startsWith("H") && (!isVowel(code[code.size() - 1])
				|| (word.size() >= 2 && !isVowel(word[1]))))
				&& !(word.startsWith("W") && isVowel(code[code.size() - 1]))) {
			if (word[0] != code[code.size() - 1]) {
				code.append(word[0]);
			}
		}
		word = word.right(word.size() - 1);
	}
	if (code.endsWith("S")) {
		code = code.left(code.size() - 1);
	}
	if (code.endsWith("AY")) {
		code = code.right(code.length() - 1);
		code[code.length() - 1] = QChar('Y');
	} else if (code.endsWith("A")) {
		code = code.left(code.size() - 1);
	}
	code = removeRepeatedCharacters(code);
	return code;
}

QString removeRepeatedCharacters(const QString &str)
{
	if(str.isEmpty())
	{
		return "";
	}
	
	QString res;
	res += str[0];
	std::copy_if(str.begin(), str.end(), std::back_inserter(res),
			[&](QChar c){return c != res[res.size() - 1];});
	return res;
}

void replaceRegExpsInText(QHash<QString, QRegExp> &regexps, QString &text)
{
	auto it = regexps.constBegin();
	while (it != regexps.constEnd()) {
		text.replace(it.value(), it.key());
		++it;
	}
}

void replaceIfStartsWith(QString &str, const QString &search, const QString &replacement)
{
	if (str.startsWith(search)) {
		if (search.length() == replacement.length()) {
			for (int i = 0; i < replacement.length(); i++) {
				str[i] = replacement[i];
			}
		} else {
			str = str.right(str.length() - search.length()).prepend(replacement);
		}
	}
}

void replaceIfStartsWith(QString &word, const std::map<QString, QString> &replacements)
{
	for (auto iterator = replacements.begin(); iterator != replacements.end(); iterator++) {
		replaceIfStartsWith(word, iterator->first, iterator->second);
	}
}

void replaceIfEndsWith(QString &str, const QString &search, const QString &replacement)
{
	if (str.endsWith(search)) {
		if (search.length() == replacement.length()) {
			for (int i = str.length() - replacement.length(); i < str.length(); i++) {
				str[i] = replacement[i];
			}
		} else {
			str = str.left(str.length() - search.length()).append(replacement);
		}
	}
}

void replaceIfEndsWith(QString &word, const std::map<QString, QString> &replacements)
{
	for (auto iterator = replacements.begin(); iterator != replacements.end(); iterator++) {
		replaceIfEndsWith(word, iterator->first, iterator->second);
	}
}

bool isVowel(const QChar &someChar)
{
	std::initializer_list<QChar> vowels {'a', 'e', 'i', 'o', 'u'};
	return cvv::util::isAnyOf(someChar, vowels);
}

bool isSingleWord(const QString &str)
{
	const auto isLetter = [](QChar c) {return c.isLetter();};
	return std::find_if_not(str.begin(), str.end(), isLetter) != str.end();
}

