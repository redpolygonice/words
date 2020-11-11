#include "common.h"
//#include <unistd.h>
#include <sys/stat.h>

string i2s(int number)
{
	char szNumber[20];
	sprintf(szNumber, "%d", number);
	return szNumber;
}

int s2i(const string &text)
{
	return atoi(text.c_str());
}

bool isFileExists(const std::string &fileName)
{
	struct stat st;
	if (stat(fileName.c_str(), &st) == -1)
		return false;

	return true;
}

string currentTime()
{
	std::time_t time = std::time(NULL);
	char timeStr[50];
	std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d_%H-%M-%S", std::localtime(&time));
	return timeStr;
}

StringList splitString(const string &text, const string &separator)
{
	StringList list;
	size_t previous = 0;
	size_t current = text.find(separator);
	while (current != string::npos)
	{
		list.push_back(text.substr(previous, current - previous));
		previous = current + 1;
		current = text.find(separator, previous);
	}
	list.push_back(text.substr(previous, current - previous));
	return list;
}

string strReplace(const string &s, const string &search, const string &replace)
{
	string out = s;
	for (size_t pos = 0; ; pos += replace.length())
	{
		pos = out.find(search, pos);
		if (pos == string::npos) break;
		out.erase(pos, search.length());
		out.insert(pos, replace);
	}
	return out;
}
