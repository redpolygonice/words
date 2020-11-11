#include "textprocessor.h"
#include "counter.h"

#include <codecvt>

TextProcessor::TextProcessor()
:_mutex(std::make_shared<std::shared_mutex>())
{
}

void TextProcessor::createFile(const string& fileName)
{
	FILE* in = NULL;
	in = fopen(fileName.c_str(), "rb");
	if (in == NULL)
		return;

	fseek(in, 0, SEEK_END);
	long size = ftell(in);
	fseek(in, 0, SEEK_SET);

	char* buffer = new char[size];
	fread(buffer, size, 1, in);
	fclose(in);

	FILE* out = NULL;
	out = fopen("file.txt", "wb+");
	if (out == NULL)
		return;

	long long gb = 1024 * 1024 * 1024;
	long long count = gb / size + 1;

	for (long long i = 0; i < count; ++i)
	{
		fwrite(buffer, size, 1, out);
		fflush(out);
	}

	fclose(out);
}

void TextProcessor::start(const string &fileName)
{
	if (!fs::exists(fileName))
	{
		std::cout << "File \"" << fileName << "\" does not exist!" << std::endl;
		return;
	}

	IfstreamPtr ifs = std::make_shared<std::wifstream>(fileName, std::ios::in | std::ios::binary);
	ifs->imbue(std::locale(ifs->getloc(),
		new std::codecvt_byname<wchar_t, char, std::mbstate_t>("ru_RU.UTF-8")));
	if (ifs->bad())
	{
		std::cout << "Can't open file \"" << fileName << "\"" << std::endl;
		return;
	}

	ifs->seekg(0, std::ios::end);
	size_t size = ifs->tellg();
	ifs->seekg(0, std::ios::beg);
	if (size == 0)
	{
		std::cout << "File \"" << fileName << "\"" << " is empty" << std::endl;
		return;
	}

	setlocale(LC_ALL, "ru_RU.UTF-8");
	std::cout << "Файл: " << fileName << std::endl;
	std::cout << "Количество потоков: " << threadCount << std::endl;
	std::cout << "Обработка текста ..." << std::endl;
	Counter counter;

	// Create threads
	for (int i = 0; i < threadCount; ++i)
	{
		TextThreadPtr threadPtr(new TextThread());
		threadPtr->create(ifs, _mutex);
		_data.push_back(threadPtr);
	}

	// Wait all threads finished
	//for (auto &thread : _data)
		//thread->join();

	while (true)
	{
		int count = 0;
		for (TextThreadPtr &thread : _data)
		{
			if (thread->active())
				count++;
		}

		if (count == 0)
			break;

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	std::cout << std::endl << "Результаты:" << std::endl;
	printResults();
	counter.show();
	ifs->close();
}

void TextProcessor::stop()
{
	for (TextThreadPtr &threadPtr : _data)
		threadPtr->stop();
	_data.clear();
}

void TextProcessor::mergeMap(WordsMap &source, WordsMap &dest)
{
	for (WordsMapIterator it = source.begin(); it != source.end(); ++it)
	{
		WordsMapIterator findIt = dest.find(it->first);
		if (findIt != dest.end())
			findIt->second += it->second;
		else
			dest.insert({ it->first, it->second });
	}
}

void TextProcessor::printResults()
{
	WordsMap rusWords;
	WordsMap latinWords;

	// Merge maps from each thread
	for (TextThreadPtr &thread : _data)
	{
		mergeMap(thread->reader().rusWords(), rusWords);
		mergeMap(thread->reader().latinWords(), latinWords);
		//rusWords.merge(thread->reader().rusWords());
		//latinWords.merge(thread->reader().latinWords());
	}

	std::cout << "Количество русских уникальных слов: " << rusWords.size() << std::endl;
	std::cout << "10 наиболее популярных русских слов:" << std::endl;

	// Create temporary set for ordering
	WordsSet temp;
	for (auto const &[k, v] : rusWords)
		temp.emplace(v, k);

	// Print first 10 records
	int count = 0;
	for (WordsSetIterator it = temp.begin(); it != temp.end() && count < 10; ++it, count++)
		std::wcout << std::setw(15) << it->second << " - " << std::setw(7) << it->first << std::endl;

	std::cout << std::endl;
	std::cout << "Количество английских уникальных слов: " << latinWords.size() << std::endl;
	std::cout << "10 наиболее популярных английских слов:" << std::endl;

	temp.clear();
	for (auto const &[k, v] : latinWords)
		temp.emplace(v, k);

	count = 0;
	for (WordsSetIterator it = temp.begin(); it != temp.end() && count < 10; ++it, count++)
		std::wcout << std::setw(15) << it->second << " - " << std::setw(7) << it->first << std::endl;
}

TextThread::TextThread()
:_active(false)
,_stream(nullptr)
,_mutex(nullptr)
{
}

void TextThread::create(const IfstreamPtr& stream, const MutexPtr& mutex)
{
	_active = true;
	_stream = stream;
	_mutex = mutex;
	_theTread = std::thread([this]() { run(); });
	_theTread.detach();
}

void TextThread::run()
{
	_reader.start(_stream, _mutex);
	_active = false;
}

