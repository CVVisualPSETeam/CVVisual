#ifndef CVVISUAL_STRINGUTILS_HPP
#define CVVISUAL_STRINGUTILS_HPP

#include <map>

#include <QString>
#include <QHash>
#include <QRegExp>

/**
 * @brief Implementation of the levenshtein distance, a metric for the edit distance between to strings.
 * Original source: http://en.wikibooks.org/wiki/Algorithm_implementation/Strings/Levenshtein_distance#C.2B.2B
 * @param str1 first string
 * @param str2 second string
 * @return edit distance
 */
size_t editDistance(const QString& str1, const QString& str2);


/**
 * @brief Implementation of a phonetic algorithm to compare two words.
 * It generates the NYSIIS for both words and returns the levenshtein edit distance between them.
 * Attention: using a phonetic algorithm is much slower, than the simple levenshtein.
 * @param word1 first word
 * @param word2 second word
 * @return equality of both words, 0 means both words are equal, the greater the number, the more unequal are both words.
 */
int phoneticEquality(QString word1, QString word2);

/**
 * @brief Examines the NYSIIS of the given word.
 * The NYSIIS is the New York State Identification and Intelligence System Phonetic Code,
 * http://en.wikipedia.org/wiki/NYSIIS.
 * @param word given word
 * @return NYSIIS of the given word
 */
QString nysiisForWord(QString word);

QString removeRepeatedCharacters(const QString &str);

void replaceRegExpsInText(QHash<QString, QRegExp> &regexps, QString &text);

void replaceIfStartsWith(QString &str, const QString &search, const QString &replacement);

void replaceIfStartsWith(QString &word, const std::map<QString, QString> &replacements);

void replaceIfEndsWith(QString &str, const QString &search, const QString &replacement);

void replaceIfEndsWith(QString &word, const std::map<QString, QString> &replacements);

bool isVowel(const QChar &someChar);

bool isSingleWord(const QString &str);

#endif

