/*
 * Program for checking style of scala files as per Data-Sentinel team specifications 
 */
#include <vector>
#include <map>
#include <filesystem>
#include <unordered_map>
#include <string>
#include <unordered_set>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <fstream>
#include <sstream>
#include <strstream>
#include <istream>
#include <iostream>

// TODO list:
// make directory a command line argument, and run recursively for all sub directories
// generate HTML report or something (make json and feed it or something)
// make cross platform
// make it so that we can change 

void Check_For_Spaces_Between_Commas(std::vector<int>& lines, std::vector<std::string>& input)
{
	int count = 1;
	for (const auto& s : input)
	{
		std::vector<size_t> positions;

		size_t pos = s.find(',', 0);

		while(pos != std::string::npos)
		{
			positions.push_back(pos);
			pos = s.find(',', pos + 1);
		}

		std::size_t index = s.find(",");

		for (const auto& t : positions)
		{
			if (t < s.length() - 2)
			{
				if (s[t + 1] != ' ' || s[t + 1] != '\n')
				{
					lines.push_back(count++);
				}
			}
		}
	}
}

void Check_For_Formatted_Else(std::vector<int>& lines, std::vector<std::string>& input)
{
	int count = 1;
	for (const auto& s : input)
	{
		std::size_t index = s.find("else");

		if (index != std::string::npos && (index < s.length() - 6) && (index >= 2))
		{
			if (s[index + 5] != '{' && s[index - 2] != '}')
			{
				lines.push_back(count++); 
			}
		}
	}
}

void Check_For_Double_Blank_Lines(std::vector<int>& lines, std::vector<std::string>& input)
{
	int count = 1;
	bool last_was_blank = false;
	for (const auto& s : input)
	{
		if (s == "\n")
		{
			if (last_was_blank)
			{
				lines.push_back(count++);
			}

			last_was_blank = true;
		}
		else
		{
			last_was_blank = false;
		}
	}
}

void Check_For_Space_Between_Equals_And_Brace(std::vector<int>& lines, std::vector<std::string>& input)
{
	int count = 1;
	for (const auto& s : input)
	{
		std::size_t indexOfIf = s.find("=");

		if (indexOfIf != std::string::npos && (indexOfIf < s.length() - 1))
		{
			if (s[indexOfIf + 1] == '{')
			{
				lines.push_back(count++);
			}
		}
	}
}

// assumption: only 1 if per line (lol)
void Check_For_Space_Between_If_And_Parentheses(std::vector<int>& lines, std::vector<std::string>& input)
{
	int count = 1;
	for (const auto& s : input)
	{
		std::size_t indexOfIf = s.find("if");

		if (indexOfIf != std::string::npos && (indexOfIf < s.length() - 2))
		{
			if (s[indexOfIf + 2] == '(')
			{
				lines.push_back(count++);
			}
		}
	}
}

void Check_Comments_Capitalized(std::vector<int>& lines, std::vector<std::string>& input)
{
	int count = 1;
	bool lastLineWasComment = false;
	for (const auto& s : input)
	{

		std::size_t indexFirstComment = s.find("//");
		if (indexFirstComment != std::string::npos)
		{
			if (lastLineWasComment)
			{
				continue;
			}
			else
			{
				std::string lc;
				std::transform(s.begin(), s.end(), lc.begin(), ::tolower);

				std::size_t firstLetterIndex = s.find_first_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");

				if (firstLetterIndex != std::string::npos)
				{
					if (islower(s[firstLetterIndex]))
					{
						lines.push_back(count++);
					}
				}
				lastLineWasComment = true;
			}
		}
		else
		{
			lastLineWasComment = false;
		}
	}
}

// TODO: fix code duplication here
void Check_Return_Types_On_Next_Line(std::vector<int>& lines, std::vector<std::string>& input)
{
	bool searching = false;
	int count = 1;
	for (const auto& s : input)
	{
		// does line contain def? if so, find final closing ) and :
		if (s.find("def ") != std::string::npos && !searching)
		{
			searching = true;
			// find next ), and make sure theres a newline within the next 3 chars (2?)

			int index = s.find_first_of(')');
			if (index != std::string::npos && (index < s.length() - 2))
			{
				if (s[index + 2] != '\n')
				{
					lines.push_back(count++);
				}
			}

		}
		else if(searching)
		{
			int index = s.find_first_of(')');
			if (index != std::string::npos && (index < s.length()-2))
			{
				if(s[index + 2] != '\n')
				{
					lines.push_back(count++);
				}
			}
		}
		else
		{
			searching = false;
		}
	}
}

void Check_80_Lines(std::vector<int>& lines, std::vector<std::string>& input)
{
	int count = 1;
	for (const auto& s : input)
	{
		if (s.length() > 80)
		{
			lines.push_back(count++);
		}
	}
}

void Print_Offending_Lines(std::string message, std::vector<int>& line_counters)
{
	if (line_counters.size() == 0)
		return;

	std::cout << message << "\n";

	for (const auto& i : line_counters)
	{
		std::cout << i << "\n";
	}

	line_counters.clear();
}

int main(void)
{
	std::string curr_path = std::experimental::filesystem::current_path().string();

	std::experimental::filesystem::path actual_path = std::experimental::filesystem::current_path();
	// Step 1: read in all the .scala files in this directory

	int lines_over_80 = 0;
	int return_types_not_on_next_line = 0;
	int comments_not_capitalized = 0;
	int no_space_between_if_and_par = 0;
	int no_space_between_eq_and_brace = 0;
	int double_blank_lines = 0;
	int non_formatted_else = 0;
	int no_space_between_commas = 0;

	// Step 2: for each of these files, check all the rules, and output the results
	// (have a vector for each rule violation (e.g. 80 lines, and push back line numbers
	// keep running total for all errors so we can print at end

	for (auto& f : std::experimental::filesystem::recursive_directory_iterator(actual_path))
	{
		std::string curr_file = f.path().string();

		std::ifstream infile(curr_file);

		std::cout << "\n Scanning file " << curr_file << "\n";

		std::string currline;
		int linecount = 1;
		std::vector<std::string> lines;
		std::vector<int> line_counters;
		while (std::getline(infile, currline))
		{
			lines.push_back(currline);
		}

		Check_80_Lines(line_counters, lines);
		lines_over_80 += line_counters.size();
		Print_Offending_Lines("The following lines were longer than 80 characters: ", line_counters);

		Check_Return_Types_On_Next_Line(line_counters, lines);
		return_types_not_on_next_line += line_counters.size();
		Print_Offending_Lines("The following lines didn't have a return type on the next line: ", line_counters);

		Check_Comments_Capitalized(line_counters, lines);
		comments_not_capitalized += line_counters.size();
		Print_Offending_Lines("The following lines had a comment that should be capitalized: ", line_counters);

		Check_For_Space_Between_If_And_Parentheses(line_counters, lines);
		no_space_between_if_and_par += line_counters.size();
		Print_Offending_Lines("The following lines didn't have a space between if and the parentheses: ", line_counters);

		Check_For_Space_Between_Equals_And_Brace(line_counters, lines);
		no_space_between_eq_and_brace += line_counters.size();
		Print_Offending_Lines("The following lines didn't have a space between = and brace: ", line_counters);

		Check_For_Double_Blank_Lines(line_counters, lines);
		double_blank_lines += line_counters.size();
		Print_Offending_Lines("The following lines had double blank lines: ", line_counters);

		Check_For_Formatted_Else(line_counters, lines);
		non_formatted_else += line_counters.size();
		Print_Offending_Lines("The following lines didn't have a properly formatted else: ", line_counters);

		Check_For_Spaces_Between_Commas(line_counters, lines);
		no_space_between_commas += line_counters.size();
		Print_Offending_Lines("The following lines didn't have a space between comma separated values: ", line_counters);
	}


	// Step 3: print out results

	std::cout << "Number of lines over 80: " << lines_over_80 << "\n";
	std::cout << "Number of return types not on next line: " << return_types_not_on_next_line << "\n";
	std::cout << "Comments not capitalized: " << comments_not_capitalized << "\n";
	std::cout << "No space between if and parentheses: " << no_space_between_if_and_par << "\n";
	std::cout << "No space between equals and brace: " << no_space_between_eq_and_brace << "\n";
	std::cout << "Double blank lines: " << double_blank_lines << "\n";
	std::cout << "Wrongly formatted elses: " << non_formatted_else << "\n";
	std::cout << "No spaces between comma separated things: " << no_space_between_commas << "\n";

	return 0;
}