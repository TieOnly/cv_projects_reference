#include "PrintText.h"

PT::WordList PT::splitTextIntoWords( const std::string &text )
{
	WordList words;
	std::istringstream in(text);
	std::copy(std::istream_iterator<std::string>(in),
			std::istream_iterator<std::string>(),
			std::back_inserter(words));
	return words;
}

void PT::justifyLine( std::string line )
{
	size_t pos = line.find_first_of(' ');
	if (pos != std::string::npos) {
		while (line.size() < pageWidth) {
			pos = line.find_first_not_of(' ', pos);
			line.insert(pos, " ");
			pos = line.find_first_of(' ', pos+1);
			if (pos == std::string::npos) {
				pos = line.find_first_of(' ');
			}
		}
	}
	std::cout << line << std::endl;
}

void PT::_printText( const std::string &text, PrintLineFunction printLineFunc )
{
	WordList words = splitTextIntoWords(text);

	std::string line;
	for (const std::string& word : words) {
		if (line.size() + word.size() + 1 > pageWidth) { // next word doesn't fit into the line.
			printLineFunc(line);
			line.clear();
			line = word;
		} else {
			if (!line.empty()) {
				line.append(" ");
			}
			line.append(word);
		}
	}
	if (printLineFunc != justifyLine)
		printLineFunc(line);
	else
		std::cout << line << std::endl;
}

void PT::printLineLeft( const std::string line )
{
	std::cout << line << std::endl;
}

void PT::printLineRight( const std::string line )
{
	std::cout << std::setw(pageWidth) << std::right << line << std::endl;
}

void PT::printLineCenter( const std::string line )
{
	std::cout << std::string((pageWidth - line.size())/2, ' ') << line << std::endl;
}

void PT::printText( const std::string &text, Alignment align )
{
	PrintLineFunction array[] = { printLineLeft, printLineRight, printLineCenter, justifyLine };
	_printText(text, array[static_cast<size_t>(align)]);
}