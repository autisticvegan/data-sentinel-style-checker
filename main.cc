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
#include <istream>
#include <iostream>
#include <regex>

#include "HTML/HTML.h"
#include "StyleCheckerConfig.h"
// TODO list:
// Functions to add:
// make an option for recursive or not
// Port and do benchmarks for Scala / Elixir / Go

void Check_For_Spaces_Between_Commas(std::vector<int>& lines, std::vector<std::string>& input) {
	int count = 1;
	for (const auto& s : input) {

		if (s.length() > 6 && s.substr(0, 6) == "import") {
			++count;
			continue;
		}

		std::vector<size_t> positions;

		size_t pos = s.find(',', 0);

		while (pos != std::string::npos) {
			positions.push_back(pos);
			pos = s.find(',', pos + 1);
		}

		for (const auto& t : positions) {
			if (t < s.length() - 2) {
				if ((s[t + 1] != ' ') && (s[t + 1] != '\n')) {
					lines.push_back(count);
				}
			}
		}
		++count;
	}
}

void Check_For_Formatted_Else(std::vector<int>& lines, std::vector<std::string>& input) {
	int count = 1;
	for (const auto& s : input) {
		std::size_t index = s.find("else");

		if (index != std::string::npos && (index < s.length() - 6) && (index >= 2)) {
			if (s[index + 5] != '{' && s[index - 2] != '}') {
				lines.push_back(count); 
			}
		}
		++count;
	}
}

void Check_For_Double_Blank_Lines(std::vector<int>& lines, std::vector<std::string>& input) {
	int count = 1;
	bool last_was_blank = false;
	for (const auto& s : input) {
		if (s == "\n") {
			if (last_was_blank) {
				lines.push_back(count);
			}

			last_was_blank = true;
		} else {
			last_was_blank = false;
		}
		++count;
	}
}

void Check_For_Space_Between_Equals_And_Brace(std::vector<int>& lines, std::vector<std::string>& input) {
	int count = 1;
	for (const auto& s : input) {
		std::size_t indexOfIf = s.find("=");

		if (indexOfIf != std::string::npos && (indexOfIf < s.length() - 1)) {
			if (s[indexOfIf + 1] == '{') {
				lines.push_back(count);
			}
		}
		++count;
	}
}

// assumption: only 1 if per line (lol)
void Check_For_Space_Between_If_And_Parentheses(std::vector<int>& lines, std::vector<std::string>& input) {
	int count = 1;
	for (const auto& s : input) {
		std::size_t indexOfIf = s.find("if");

		if (indexOfIf != std::string::npos && (indexOfIf < s.length() - 2)) {
			if (s[indexOfIf + 2] == '(') {
				lines.push_back(count);
			}
		}
		++count;
	}
}

void Check_Return_Types_On_Next_Line(std::vector<int>& lines, std::vector<std::string>& input) {
	std::string concat;

	// put the defs + 1 lines into the lines vector, and if you get a false, then you put that one in the lines

	std::vector<int> lines_Of_Defs_Plus_One;

	int count = 1;
	for (const auto& a : input) {
		if (a.find("def ", 0, 4) != std::string::npos)
		{
			lines_Of_Defs_Plus_One.push_back(count);
		}
		++count;
		concat += a + "\n";
	}

	std::vector<size_t> positions;

	size_t pos = concat.find("def ", 0);

	while (pos != std::string::npos) {
		positions.push_back(pos);
		pos = concat.find("def ", pos + 1);
	}

	if (positions.empty())
		return;

	std::unordered_set<size_t> positions_of_sadfaces; // ):

	for (const auto& t : positions) {
		pos = concat.find("):", t);
		if (pos != std::string::npos) {
			positions_of_sadfaces.insert(pos);
		}
	}
	
	std::vector<size_t> actual_sadfaces(positions_of_sadfaces.begin(), positions_of_sadfaces.end());

	for (int i = 0; i < actual_sadfaces.size(); ++i) {
		if (actual_sadfaces[i] < concat.size() - 4) {
			size_t p = actual_sadfaces[i];

			if (concat[p + 2] != '\n' && concat[p + 3] != '\n') {
				if (lines_Of_Defs_Plus_One.size() > i) {
					lines.push_back(lines_Of_Defs_Plus_One[i]);
				}
			}
		}
	}
}

void Check_Long_Lines(std::vector<int>& lines, std::vector<std::string>& input, int line_length, bool check_imports)
{
	int count = 1;
	for (const auto& s : input) {
		if (s.length() > 6 && s.substr(0, 6) == "import" && !check_imports) {
			++count;
			continue;
		}

		if (s.length() > line_length) {
			lines.push_back(count);
		}

		++count;
	}
}

//helper functions for string splitting
std::vector<std::string> split(const std::string& input, const std::string& regex) {
	// passing -1 as the submatch index parameter performs splitting
	std::regex re(regex);
	std::sregex_token_iterator
		first{ input.begin(), input.end(), re, -1 },
		last;
	return { first, last };
}

void Check_For_Spark_To_Be_First_Param(std::vector<int>& lines, std::vector<std::string>& input) {
	std::string concat;

	// find all the defs, and get all the command line params for each one (comma tokenized strings)
	// if there is a SparkSession in any of them, do some stuff, if not, continue
	// do stuff: is the one containing SparkSession the first token? if so, good, if not, push back line number

	std::vector<int> lines_Of_Defs_Plus_One;

	int count = 1;
	for (const auto& a : input) {
		if (a.find("def ", 0, 4) != std::string::npos)
		{
			lines_Of_Defs_Plus_One.push_back(count);
		}
		++count;
		concat += a + "\n";
	}

	std::vector<size_t> positions;

	size_t pos = concat.find("def ", 0);

	while (pos != std::string::npos) {
		positions.push_back(pos);
		pos = concat.find("def ", pos + 1);
	}

	if (positions.empty())
		return;

	std::vector<std::pair<size_t,size_t>> positions_of_sadfaces; // (

	for (const auto& t : positions) {
		pos = concat.find("(", t);

		size_t pos2 = concat.find("):", t);
		if (pos != std::string::npos  && pos < pos2)
		{
			positions_of_sadfaces.push_back({ pos, pos2 });
		}
	}

	//can remove duplicates?
	std::vector<std::pair<size_t, size_t>> actual_sadfaces(positions_of_sadfaces.begin(), positions_of_sadfaces.end());

	for (int i = 0; i < actual_sadfaces.size(); ++i) {
		std::string command_line_params = concat.substr(actual_sadfaces[i].first, actual_sadfaces[i].second - actual_sadfaces[i].first);

		std::vector<std::string> tokens = split(command_line_params, ",");

		for (int j = 0; j < tokens.size(); ++j) {
			//try to find "SparkSession"
			if (tokens[j].find("SparkSession") != std::string::npos) {
				if (j != 0)
				{
					lines.push_back(lines_Of_Defs_Plus_One[i]);
				}

				break;
			}
		}
	}
}

bool Print_Offending_Lines(std::string message, std::vector<int>& line_counters)
{
	if (line_counters.size() == 0)
		return true;

	std::cout << message << "\n";

	for (const auto& i : line_counters)
	{
		std::cout << i << "\n";
	}

	line_counters.clear();
	return false;
}

int main(int argc, char** argv)
{
    if (argc == 0 || argc > 1) {
        std::cout << "This is a stylechecker which only supports scala right now.";
        std::cout << std::endl << "Options are specified in a file called \"stylechecker_config.txt\"";
        std::cout << "which resides in the same directory as this executable.  \n";
        std::cout << "An HTML report will be generated in this directory once all the files have been checked.";
        std::cout << "To generate a default config, use the --generate argument";
        std::cout << "To run the stylechecker, use the --check argument";

        return 0;
    }

    std::string arg = argv[0];
    if (arg == "--generate") {

    } else if (arg == "--check") {

    } else {
        return -1;
    }



	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	// step 0: read in the config file
	std::string curr_path = std::filesystem::current_path().string();
	std::string config_path = curr_path + "/stylechecker_config.txt";
	StyleCheckerConfig config(config_path);

	std::filesystem::path actual_path = std::filesystem::current_path();

	int long_lines = 0;
	int return_types_not_on_next_line = 0;
	int comments_not_capitalized = 0;
	int no_space_between_if_and_par = 0;
	int no_space_between_eq_and_brace = 0;
	int double_blank_lines = 0;
	int non_formatted_else = 0;
	int no_space_between_commas = 0;
	int filecount = 0;
	int nonsparkfirstparam = 0;
	// Step 2: for each of these files, check all the rules, and output the results
	// (have a vector for each rule violation (e.g. 80 lines, and push back line numbers
	// keep running total for all errors so we can print at end
	std::vector <std::filesystem::directory_entry> clean_files;
	std::vector <std::filesystem::directory_entry> unclean_files;
	for (std::filesystem::directory_entry f : std::filesystem::recursive_directory_iterator(actual_path)) {
        std::string curr_file = f.path().string();
        if (curr_file.length() < 7)
            continue;
        std::string ext_we_want = "." + config.file_extension;
        if (f.path().extension() != ext_we_want)
            continue;

        ++filecount;

        std::ifstream infile(curr_file);

        std::cout << "\n Scanning file " << curr_file << "\n";

        std::string currline;
        int linecount = 1;
        std::vector<std::string> lines;
        std::vector<int> line_counters;
        while (std::getline(infile, currline)) {
            lines.push_back(currline);
        }

        bool isClean = true;

        if (config.check_long_lines) {
            Check_Long_Lines(line_counters, lines, config.long_line_length, config.check_imports);
            long_lines += line_counters.size();
            if (!Print_Offending_Lines(
                    "The following lines were longer than " + std::to_string(config.long_line_length) +
                    " characters: ", line_counters))
                isClean = false;
        }

        if (config.check_else_braces) {
            Check_For_Formatted_Else(line_counters, lines);
            non_formatted_else += line_counters.size();
            if (!Print_Offending_Lines("The following lines didn't have a properly formatted else: ", line_counters))
                isClean = false;
        }

        if (config.check_return_type_on_next_line) {
            Check_Return_Types_On_Next_Line(line_counters, lines);
            return_types_not_on_next_line += line_counters.size();
            if (!Print_Offending_Lines("The following lines didn't have a return type on the next line: ",
                                       line_counters))
                isClean = false;
        }

        if (config.check_for_spaces_between_commas) {
            Check_For_Spaces_Between_Commas(line_counters, lines);
            no_space_between_commas += line_counters.size();
            if (!Print_Offending_Lines("The following lines didn't have a space between comma separated values: ",
                                       line_counters))
                isClean = false;
        }

        if (config.check_for_double_blank_lines) {
            Check_For_Double_Blank_Lines(line_counters, lines);
            double_blank_lines += line_counters.size();
            if (!Print_Offending_Lines("The following lines had double blank lines: ", line_counters))
                isClean = false;
        }

        if (config.check_for_space_between_equals) {
            Check_For_Space_Between_Equals_And_Brace(line_counters, lines);
            no_space_between_eq_and_brace += line_counters.size();
            if (!Print_Offending_Lines("The following lines didn't have a space between = and {: ", line_counters))
                isClean = false;
        }

        if (config.check_for_space_between_if_and_parentheses) {
            Check_For_Space_Between_If_And_Parentheses(line_counters, lines);
            no_space_between_if_and_par += line_counters.size();
            if (!Print_Offending_Lines("The following lines didn't have a space between if and the parentheses: ",
                                       line_counters))
                isClean = false;

        }

        if (config.check_for_spark_to_be_first_param) {
            Check_For_Spark_To_Be_First_Param(line_counters, lines);
            nonsparkfirstparam += line_counters.size();
            if (!Print_Offending_Lines("This following lines didn't have spark as the first param in functions: ", line_counters))
                isClean = false;
        }

		isClean ? clean_files.push_back(f) : unclean_files.push_back(f);
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	// Step 3: print out results

	std::vector<std::string> stat_strings;

    if (config.check_long_lines) {
        stat_strings.push_back( "Number of long lines: " + std::to_string(long_lines) + "\n");
    }

    if (config.check_else_braces) {
        stat_strings.push_back( "Wrongly formatted elses: " + std::to_string(non_formatted_else) + "\n");
    }

    if (config.check_return_type_on_next_line) {
        stat_strings.push_back( "Number of return types not on next line: " + std::to_string(return_types_not_on_next_line) + "\n");
    }

    if (config.check_for_spaces_between_commas) {
        stat_strings.push_back( "No spaces between comma separated things: " + std::to_string(no_space_between_commas) + "\n");
    }

    if (config.check_for_double_blank_lines) {
        stat_strings.push_back( "Double blank lines: " + std::to_string(double_blank_lines) + "\n");
    }

    if (config.check_for_space_between_equals) {
        stat_strings.push_back( "No space between equals and brace: " + std::to_string(no_space_between_eq_and_brace) + "\n");
    }

    if (config.check_for_space_between_if_and_parentheses) {
        stat_strings.push_back( "No space between if and parentheses: " + std::to_string(no_space_between_if_and_par) + "\n");
    }

    if (config.check_for_spark_to_be_first_param) {
        stat_strings.push_back( "SparkSession not first param: " + std::to_string(nonsparkfirstparam) + "\n");
    }

	std::cout << std::endl << filecount << " files processed in " <<
	std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1000 << " milliseconds" << std::endl;
	
	for (const auto& s : stat_strings) {
		std::cout << s;
	}

	// HTML outputs:
	// x files processed in Y seconds
	// z files were clean, x - z files were unclean
	// unclean files:
	// x - z
	// clean files:
	// z

	HTML::Document document("Style-Checker Report");
	document << HTML::Header1("Style-Checker Report");

	document << HTML::Paragraph(std::to_string(filecount) + " files were processed.").addAttribute("style", "color:darkcyan;");
	document << HTML::Header2("The following files were clean:").addAttribute("style", "color:green;");
	for (const auto& u : clean_files) {
		document << HTML::Paragraph(u.path().filename().string()).addAttribute("style", "color:green;");
	}

	document << HTML::Break();

	document << HTML::Header2("The following files were unclean:").addAttribute("style", "color:red;");
	for (const auto& u : unclean_files) {
		document << HTML::Paragraph(u.path().filename().string()).addAttribute("style", "color:red");
	}

	document <<  HTML::Break();

	document << HTML::Header2("Style stats:").addAttribute("style", "color:black;");
	
	for (const auto& s : stat_strings) {
		document << HTML::Text(s) << HTML::Break();
	}

	std::string html_path = curr_path + "/stylechecker_report.html";
	std::ofstream outfile;
	outfile.open(html_path);
	outfile << document;
	outfile.close();

	return 0;
}


/*
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
					lines.push_back(count);

				}
				searching = false;
			}

		}
		else if(searching)
		{
			int index = s.find_first_of(')');
			if (index != std::string::npos && (index < s.length()-2))
			{
				if(s[index + 2] != '\n')
				{
					lines.push_back(count);

				}
				searching = false;
			}
		}
		else
		{
			searching = false;
		}
		++count;
	}*/

/* OLD VERSION OF check return types on next line
std::string concat;

	// put the defs + 1 lines into the lines vector, and if you get a false, then you put that one in the lines

	std::vector<int> lines_Of_Defs_Plus_One;

	int count = 1;
	for (const auto& a : input)
	{
		if (a.find("def ", 0, 4) != std::string::npos)
		{
			lines_Of_Defs_Plus_One.push_back(count + 1);
		}
		++count;
		concat += a + "\n";
	}

	std::vector<size_t> positions;

	size_t pos = concat.find("def ", 0);

	while (pos != std::string::npos)
	{
		positions.push_back(pos);
		pos = concat.find("def ", pos + 1);
	}

	if (positions.empty())
		return;

	std::unordered_set<size_t> positions_of_sadfaces; // ):

	for (const auto& t : positions)
	{
		pos = concat.find("):", t);
		if (pos != std::string::npos)
		{
			positions_of_sadfaces.insert(pos);
		}
	}

	std::vector<size_t> actual_sadfaces(positions_of_sadfaces.begin(), positions_of_sadfaces.end());

	for (int i = 0; i < actual_sadfaces.size(); ++i)
	{
		if (actual_sadfaces[i] < concat.size() - 4)
		{
			size_t p = actual_sadfaces[i];

			if (concat[p + 2] != '\n' || concat[p + 3] != '\n')
			{
				if (lines_Of_Defs_Plus_One.size() > i)
				{
					lines.push_back(lines_Of_Defs_Plus_One[i]);
				}
			}
		}
	}

	// Edge case problem: putting web addresses, escape characters, multiline comments, etc.
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
				++count;
				continue;
			}
			else
			{
				std::size_t firstLetterIndex = s.find_first_of("abcdefghijklmnopqrstuvwxyz");

				if (firstLetterIndex != std::string::npos)
				{
					if (islower(s[firstLetterIndex]))
					{
						lines.push_back(count);
					}
				}
				lastLineWasComment = true;
			}
		}
		else
		{
			lastLineWasComment = false;
		}
		++count;
	}
}
*/