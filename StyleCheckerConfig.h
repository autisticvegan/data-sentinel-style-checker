#pragma once
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <istream>
#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <unordered_set>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <cctype>
#include <iomanip>

class StyleCheckerConfig {
public:
	std::string file_extension = "scala";
	bool check_imports = false;
	bool check_long_lines = true;
	int long_line_length = 80;
	bool check_else_braces = true;
	bool check_return_type_on_next_line = true;
	bool check_for_spaces_between_commas = true;
	bool check_for_double_blank_lines = true;
	bool check_for_space_between_equals = true;
	bool check_for_space_between_if_and_parentheses = true;
	bool check_for_space_between_if_and_brace = true;
	bool check_for_spark_to_be_first_param = true;

	std::string FILE_EXTENSION = "scala";
	std::string CHECK_IMPORTS = "CHECK_IMPORTS";
	std::string CHECK_LONG_LINES = "CHECK_LONG_LINES";
	std::string LONG_LINE_LENGTH = "LONG_LINE_LENGTH";
	std::string CHECK_ELSE_BRACES = "CHECK_ELSE_BRACES";
	std::string CHECK_RETURN_TYPE_ON_NEXT_LINE = "CHECK_RETURN_TYPE_ON_NEXT_LINE";
	std::string CHECK_FOR_SPACES_BETWEEN_COMMAS = "CHECK_FOR_SPACES_BETWEEN_COMMAS";
	std::string CHECK_FOR_DOUBLE_BLANK_LINES = "CHECK_FOR_DOUBLE_BLANK_LINES";
	std::string CHECK_FOR_SPACE_BETWEEN_EQUALS_AND_BRACE = "CHECK_FOR_SPACE_BETWEEN_EQUALS_AND_BRACE";
	std::string CHECK_FOR_SPACE_BETWEEN_IF_AND_PARENTHESES = "CHECK_FOR_SPACE_BETWEEN_IF_AND_PARENTHESES";
	//unused
	//std::string CHECK_FOR_SPACE_BETWEEN_IF_AND_BRACE = "CHECK_FOR_SPACE_BETWEEN_IF_AND_BRACE";
	std::string CHECK_FOR_SPARK_TO_BE_FIRST_PARAM = "CHECK_FOR_SPARK_TO_BE_FIRST_PARAM";

	std::vector<std::string> rule_strings;


	StyleCheckerConfig(std::string& filepath);
	bool IsCommentLine(std::string& s);

	void OutputConfigToTxtFile(std::string& filepath);
};