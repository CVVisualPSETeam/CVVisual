#ifndef CVVISUAL_STRINGUTILS_HPP
#define CVVISUAL_STRINGUTILS_HPP

#include <map>
#include <vector>

#include <QString>
#include <QHash>
#include <QRegExp>

namespace cvv
{
namespace stfl
{

/**
 * @brief Implementation of the levenshtein distance.
 * The levenshtein distance is a metric for the edit distance between to
 * strings.
 * Based on
 * http://en.wikibooks.org/wiki/Algorithm_implementation/Strings/Levenshtein_distance#C.2B.2B
 * @param str1 first string
 * @param str2 second string
 * @return edit distance
 */
size_t editDistance(const QString &str1, const QString &str2);

/**
 * @brief Removes repeated chars in the given string.
 * E.g. "Hello World!!!" => "Helo World!"
 * @param str given string
 * @return resulting string
 */
QString removeRepeatedCharacters(const QString &str);

/**
 * @brief Replace the search string with its replacement at the very beginning
 * of the given string.
 * @param str given string
 */
void replaceIfStartsWith(QString &str, const QString &search,
                         const QString &replacement);

/**
 * @brief Replace the replacements at the very beginning of the given string.
 * Replace the key of the replacements map with its map value in the given
 * string
 * @param str given string
 * @param replacements replacements map
 */
void replaceIfStartsWith(QString &str,
                         const std::map<QString, QString> &replacements);

/**
 * @brief Replace the search string with its replacement at the end of the given
 * string.
 * @param str given string
 */
void replaceIfEndsWith(QString &str, const QString &search,
                       const QString &replacement);

/**
 * @brief Replace the replacements at the end of the given string.
 * Replace the key of the replacements map with its map value in the given
 * string
 * @param str given string
 * @param replacements replacements map
 */
void replaceIfEndsWith(QString &str,
                       const std::map<QString, QString> &replacements);

/**
 * Check whether or not the given char is a vowel.
 */
bool isVowel(const QChar &someChar);

/**
 * @brief Check wether the given string is a single word.
 * A word consists only of letters.
 *
 * @param str string to ckeck
 */
bool isSingleWord(const QString &str);

/**
 * @brief Unescapes escaped commas in the given string.
 *
 * @param str given string
 */
void unescapeCommas(QString &str);

/**
 * @brief Shortens the given string to the given length and append "..." if
 * needed.
 * @param str given string
 * @param maxLength maximum length of the returned string
 * @param cutEnd does this method shorten the given string at the end?
 * @param fill should the resulting string be filled up with whitespace to
 * ensure all strings have length maxLength?
 */
QString shortenString(QString &str, int maxLength, bool cutEnd = true,
                      bool fill = false);

/**
 * @brief Converts a given vector of chars into a valid QString.
 * @param chars given vector of chars
 * @return resulting QString
 */
QString asciiCharVectorToQString(std::vector<char> chars);
}
}

#endif
