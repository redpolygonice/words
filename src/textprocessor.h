#ifndef TEXTPROCESSOR_H
#define TEXTPROCESSOR_H

#include "types.h"
#include "textreader.h"

#include <mutex>

class TextThread;
class TextProcessor;

typedef std::shared_ptr<TextThread> TextThreadPtr;

/** Text thread */
class TextThread
{
private:
	bool _active;
	IfstreamPtr _stream;
	TextReader _reader;
	std::thread _theTread;
	MutexPtr _mutex;

private:
	void run();

public:
	TextThread();
	bool active() const { return _active; }
	TextReader &reader() { return _reader; }
	void create(const IfstreamPtr& stream, const MutexPtr& mutex);
	void stop() { _active = false; }
	void join() { _theTread.join(); }
};

/** Text processor */
class TextProcessor
{
private:
	const int threadCount = 8;
	std::vector<TextThreadPtr> _data;
	MutexPtr _mutex;

private:
	void mergeMap(WordsMap &source, WordsMap &dest);
	void printResults();

public:
	TextProcessor();
	void start(const string &fileName);
	void stop();
	static void createFile(const string& fileName);
};

#endif // TEXTPROCESSOR_H
