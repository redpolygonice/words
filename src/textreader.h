#ifndef TEXTREADER_H
#define TEXTREADER_H

#include "types.h"
#include <mutex>

typedef std::map<wstring, uint32_t> WordsMap;
typedef std::map<wstring, uint32_t>::iterator WordsMapIterator;

typedef std::set<std::pair<uint32_t, wstring>, std::greater<>> WordsSet;
typedef std::set<std::pair<uint32_t, wstring>, std::greater<>>::iterator WordsSetIterator;

/** Text reader */
class TextReader
{
private:
	WordsMap _rusWords;
	WordsMap _latinWords;
	std::mutex _mutex;

	static constexpr int buffer_size = 4096;
	const wchar_t ru_a = 1072;
	const wchar_t ru_z = 1103;
	const wchar_t ru_A = 1040;
	const wchar_t ru_Z = 1071;

private:
	void printResults();
	inline bool isRusChar(wchar_t c) const { return (c >= ru_a && c <= ru_z) || (c >= ru_A && c <= ru_Z); }
	inline bool isLatinChar(wchar_t c) const { return (c >= L'a' &&  c<= L'z') || (c >= L'A' && c <= L'Z'); }

public:
	TextReader();
	void start(const IfstreamPtr &stream, const MutexPtr &mutex);
	const WordsMap &rusWords() const { return _rusWords; }
	WordsMap &rusWords() { return _rusWords; }
	const WordsMap &latinWords() const { return _latinWords; }
	WordsMap &latinWords() { return _latinWords; }
};

#endif // TEXTREADER_H
