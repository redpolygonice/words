#include "cmdparser.h"
#include "textprocessor.h"

int main(int argc, char **argv)
{
	CmdParser cmdParser;
	cmdParser.parse(argc, argv);

	TextProcessor processor;
	processor.start(cmdParser.getInputFile());

	return 0;
}
