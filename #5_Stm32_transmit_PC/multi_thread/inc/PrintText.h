// Origin: https://stackoverflow.com/questions/22983008/how-to-print-justified-text-in-the-console-using-modern-c
#pragma once
#include <iostream>
#include <iomanip>
#include <sstream>
#include <list>
#include <iterator>

namespace PT
{
	const int pageWidth = 78;
	typedef std::list<std::string> WordList;
	typedef void (*PrintLineFunction)( const std::string );

	enum class Alignment {
		Left,
		Right,
		Center,
		Justify
	};

	WordList splitTextIntoWords( const std::string &text );
	void justifyLine( std::string line );

	void _printText( const std::string &text, PrintLineFunction printLineFunc );
	void printLineLeft( const std::string line );
	void printLineRight( const std::string line );
	void printLineCenter( const std::string line );
	void printText( const std::string &text, Alignment align );
};

