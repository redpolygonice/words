#include "textreader.h"

#include <fstream>
#include <codecvt>

TextReader::TextReader()
{
}

void TextReader::start(const IfstreamPtr &stream, const MutexPtr& mutex)
{
	wstring word;
	word.reserve(1024);

	setlocale(LC_ALL, "ru_RU.UTF-8");
	std::locale ruLocale("ru_RU.UTF-8");
	std::locale enLocale("en_US.UTF-8");

	wchar_t c = 0;
	wchar_t buffer[buffer_size + 50];
			
	while (true)
	{
		mutex->lock();

		stream->read(buffer, buffer_size);
		int gcount = stream->gcount();
		bool eof = stream->eof();

		// Check last char and read till space char if need
		if (gcount > 0 && !std::isspace(buffer[gcount - 1], enLocale))
		{
			c = 0;
			do
			{
				stream->get(c);
				if (stream->eof())
					break;

				buffer[gcount - 1] = c;
				gcount++;
			} while (!std::isspace(c, enLocale));
			gcount--;
		}

		mutex->unlock();

		// Parse buffer
		for (int i = 0; i < gcount; ++i)
		{
			c = buffer[i];
			if (std::isspace(c, enLocale))
			{
				if (!word.empty())
				{
					if (isRusChar(word[0]))
						_rusWords[word]++;
					else
						_latinWords[word]++;

					word.clear();
				}
			}
			else if (std::isalpha<wchar_t>(c, enLocale) ||
				std::isalpha<wchar_t>(c, ruLocale))
			{
				word.push_back(c);
			}
		}

		if (eof)
			break;

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

void TextReader::printResults()
{
	std::cout << "Количество русских уникальных слов: " << _rusWords.size() << std::endl;
	std::cout << "10 наиболее популярных русских слов:" << std::endl;

	WordsSet temp;
	for (auto const &[k, v] : _rusWords)
		temp.emplace(v, k);

	int count = 0;
	for (WordsSetIterator it = temp.begin(); it != temp.end() && count < 10; ++it, count++)
		std::wcout << std::setw(15) << it->second << " - " << std::setw(7) << it->first << std::endl;

	std::cout << std::endl;
	std::cout << "Количество английских уникальных слов: " << _latinWords.size() << std::endl;
	std::cout << "10 наиболее популярных английских слов:" << std::endl;

	temp.clear();
	for (auto const &[k, v] : _latinWords)
		temp.emplace(v, k);

	count = 0;
	for (WordsSetIterator it = temp.begin(); it != temp.end() && count < 10; ++it, count++)
		std::wcout << std::setw(15) << it->second << " - " << std::setw(7) << it->first << std::endl;
}
