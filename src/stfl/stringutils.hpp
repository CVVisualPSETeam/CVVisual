#ifndef CVVISUAL_STRINGUTILS_HPP
#define CVVISUAL_STRINGUTILS_HPP

#include <algorithm>
#include <cstddef>
#include <numeric>

/**
 * @brief Implementation of the levenshtein distance, a metric for the edit distance between to strings.
 * Original source: http://en.wikibooks.org/wiki/Algorithm_implementation/Strings/Levenshtein_distance#C.2B.2B
 * @param str1 first string
 * @param str2 second string
 * @return edit distance
 */
size_t editDistance(QString str1, QString str2) {
	const auto len1 = str1.size();
	const auto len2 = str2.size();
	
	std::vector<size_t> col(len2 + 1)
	std::vector<size_t> prevCol(len2 + 1);
	
	std::iota(prevCol.begin(), prevCol.end(), 0);
	
	for (size_t i = 0; i < len1; i++)
	{
		col[0] = i + 1;
		
		for (size_t j = 0; j < len2; j++)
		{
			col[j + 1] = std::min({
					1 + col[j],
					1 + prevCol[1 + j]),
					prevCol[j] + (str1[i] == str2[j]) });
		}
		std::swap(col, prevCol);
	}
	return prevCol[len2];
}

/**
 * @brief Implementation of a phonetic algorithm to compare two words.
 * It generates the NYSIIS for both words and returns the levenshtein edit distance between them.
 * Attention: using a phonetic algorithm is much slower, than the simple levenshtein.
 * @param word1 first word
 * @param word2 second word
 * @return equality of both words, 0 means both words are equal, the greater the number, the more unequal are both words.
 */
int phoneticEquality(QString word1, QString word2) {
	if (word1 == word2)
		return 0;
	//std::cout << toString(word1) << " => " << toString(nysiisForWord(word1)) << ", " << toString(word2) << " => " << toString(nysiisForWord(word2)) << std::endl;
	return editDistance(nysiisForWord(word1), nysiisForWord(word2)) + 1;
}

/**
 * @brief Examines the NYSIIS of the given word.
 * The NYSIIS is the New York State Identification and Intelligence System Phonetic Code,
 * http://en.wikipedia.org/wiki/NYSIIS.
 * @param word given word
 * @return NYSIIS of the given word
 */
QString nysiisForWord(QString word) {
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

QString removeRepeatedCharacters(const QString &str) {
	QString res;
	int i = 0;
	while (i < str.size() - 1) {
		if (str[i] != str[i + 1]) {
			res.append(str[i]);
		}
		i++;
	}
	res.append(str[str.size() - 1]);
	return res;
}

void replaceRegExpsInText(QHash<QString, QRegExp> &regexps, QString &text) {
	auto it = regexps.constBegin();
	while (it != regexps.constEnd()) {
		text.replace(it.value(), it.key());
		++it;
	}
}

void replaceIfStartsWith(QString &str, const QString &search, const QString &replacement) {
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

void replaceIfStartsWith(QString &word, const std::map<QString, QString> &replacements) {
	for (auto iterator = replacements.begin(); iterator != replacements.end(); iterator++) {
		replaceIfStartsWith(word, iterator->first, iterator->second);
	}
}

void replaceIfEndsWith(QString &str, const QString &search, const QString &replacement) {
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

void replaceIfEndsWith(QString &word, const std::map<QString, QString> &replacements) {
	for (auto iterator = replacements.begin(); iterator != replacements.end(); iterator++) {
		replaceIfEndsWith(word, iterator->first, iterator->second);
	}
}

bool isVowel(const QChar &someChar) {
	return someChar == 'a' || someChar == 'e' || someChar == 'i' || someChar == 'o' || someChar == 'u';
}

bool isSingleWord(const QString &str) {
	for (QChar c : str)
		if (!c.isLetter())
			return false;
	return true;
}

#endif

