#include "StyleCheckerConfig.h"

bool StyleCheckerConfig::IsCommentLine(std::string& str)
{
	return str.find("//") != std::string::npos;
}

// helper function from stack overflow
bool to_bool(std::string str) {
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	std::istringstream is(str);
	bool b;
	is >> std::boolalpha >> b;
	return b;
}

StyleCheckerConfig::StyleCheckerConfig(std::string& filepath)
{
	rule_strings.push_back(FILE_EXTENSION);
	rule_strings.push_back(CHECK_IMPORTS);
	rule_strings.push_back(CHECK_LONG_LINES);
	rule_strings.push_back(LONG_LINE_LENGTH);
	rule_strings.push_back(CHECK_ELSE_BRACES);
	rule_strings.push_back(CHECK_RETURN_TYPE_ON_NEXT_LINE);
	rule_strings.push_back(CHECK_FOR_SPACES_BETWEEN_COMMAS);
	rule_strings.push_back(CHECK_FOR_DOUBLE_BLANK_LINES);
	rule_strings.push_back(CHECK_FOR_SPACE_BETWEEN_EQUALS_AND_BRACE);
	rule_strings.push_back(CHECK_FOR_SPACE_BETWEEN_IF_AND_PARENTHESES);
	rule_strings.push_back(CHECK_FOR_SPARK_TO_BE_FIRST_PARAM);

	std::ifstream infile(filepath);

	std::string currline;
	std::vector<std::string> lines;
	std::vector<int> line_counters;
	// Filter out lines starting with //
	while (std::getline(infile, currline))
	{
		if (!IsCommentLine(currline))
		{
			lines.push_back(currline);
		}
	}
	
	if (rule_strings.size() != lines.size())
	{
		throw "bad config";
	}

	// if we dont see a line or something goes wrong reading it, just set it to the default value
	int linecounter = 0;
	for (int i = 0; i < lines.size(); ++i)
	{
		size_t equals_at = lines[i].find(" = ");
		
		if (equals_at == std::string::npos || equals_at == 0)
		{
			continue;
		}

		//get string before =
		std::string before_equals = lines[i].substr(0, equals_at);

		if (before_equals != rule_strings[i])
		{
			continue;
		}
		// get string from end value, then depending on which number, turn it into a value type such as bool, string, or int

		std::string after_equals = lines[i].substr(equals_at + 3);

		switch (i) 
		{
		case 0:
			//string
			file_extension = after_equals;
			break;

		// make sure this is not negative?
		case 3:
		{
			//int
			int vali = std::stoi(after_equals);
			long_line_length = vali;
			break;
		}

		default:
			// bool
			bool val = to_bool(after_equals);
			
			// TODO: this code is absolutely terrible, fix this
			if (i == 1)
				check_imports = val;
			else if (i == 2)
				check_long_lines = val;
			else if (i == 4)
				check_else_braces = val;
			else if (i == 5)
				check_return_type_on_next_line = val;
			else if (i == 6)
				check_for_spaces_between_commas = val;
			else if (i == 7)
				check_for_double_blank_lines = val;
			else if (i == 8)
				check_for_space_between_equals = val;
			else if (i == 9)
				check_for_space_between_if_and_parentheses = val;
			else if (i == 10)
				check_for_spark_to_be_first_param = val;
		}
		

	}


}

//output function: in case we run without a config file, 
//this will put a default config in the directory of where the program is running.
void StyleCheckerConfig::OutputConfigToTxtFile(std::string& filepath)
{
	std::ofstream outfile;
	outfile.open(filepath);

	// could eventually put the values in a struct containing 3 things - string, int, and bool (union?) variant etc
	// also should have a helper function that takes in 2 strings and returns a string (it puts the equals and the newline)
	outfile << FILE_EXTENSION                             << " = " << file_extension << "\n";
	outfile << CHECK_IMPORTS                              << " = " << check_imports << "\n";
	outfile << CHECK_LONG_LINES                           << " = " << check_long_lines << "\n";
	outfile << LONG_LINE_LENGTH                           << " = " << long_line_length << "\n";
	outfile << CHECK_ELSE_BRACES                          << " = " << check_else_braces << "\n";
	outfile << CHECK_RETURN_TYPE_ON_NEXT_LINE             << " = " << check_return_type_on_next_line << "\n";
	
	outfile << CHECK_FOR_SPACES_BETWEEN_COMMAS            << " = " << check_for_spaces_between_commas << "\n";
	outfile << CHECK_FOR_DOUBLE_BLANK_LINES               << " = " << check_for_double_blank_lines << "\n";
	outfile << CHECK_FOR_SPACE_BETWEEN_EQUALS_AND_BRACE   << " = " << check_for_space_between_equals << "\n";
	outfile << CHECK_FOR_SPACE_BETWEEN_IF_AND_PARENTHESES << " = " << check_for_space_between_if_and_parentheses << "\n";
	outfile << CHECK_FOR_SPARK_TO_BE_FIRST_PARAM          << " = " << check_for_spark_to_be_first_param << "\n";

	outfile.close();
}