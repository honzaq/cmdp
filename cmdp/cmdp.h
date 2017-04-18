#pragma once

#include <stdint.h>
#include <set>
#include <map>
#include <vector>
#include <string>
//#include <algorithm>

namespace cmdp
{

class parser
{
public:
	enum ParseMode : int32_t {
		NONE  = 1 << 0,
	};
	parser() = default;
	parser(int argc, const wchar_t* const argv[], ParseMode mode = NONE);

	void parse(int argc, const wchar_t* const argv[], ParseMode mode = NONE);

	/*std::multiset<std::wstring> const& flags() const { 
		return m_flags;
	}*/
	std::map<std::wstring, std::wstring> const& params() const { 
		return m_params;
	}
	std::vector<std::wstring> const& pos_args() const { 
		return m_pos_args;
	}

private:
	void parse_one_param(const wchar_t* const param);

private:
	/*std::vector<std::wstring> m_args;*/
	std::map<std::wstring, std::wstring> m_params;
	std::vector<std::wstring> m_pos_args;
	/*std::multiset<std::wstring> m_flags;*/
	/*std::set<std::wstring> registeredParams_;*/
	std::wstring m_empty;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

parser::parser(int argc, const wchar_t* const argv[], ParseMode mode /*= PREFER_FLAG_FOR_UNREG_OPTION*/)
{
	parse(argc, argv, mode);
}

void parser::parse(int argc, const wchar_t* const argv[], ParseMode mode /*= PREFER_FLAG_FOR_UNREG_OPTION*/)
{
	// Convert to strings
	//m_args.resize(argc);
	//std::transform(argv, argv + argc, m_args.begin(), [](const wchar_t* const arg) { return arg; });

/*
 * -flag
 * --flag
 * /flag
 * /param=<param_value> (do not allow spaces between param and '=')
 * -param=<param_value>
 * --param=<param_value>
 * /param:<param_value>
 * -param:<param_value>
 *
 * /param <param_value> (next argv)
 * -param <param_value> (next argv)
 *
 * <param_value>:
 *   text_without_spaces 
 *   "text with spaces, \" is not end, end with "
 * ignore spaces+tabs on beginning
 */

	for (auto i = 0; i < argc; ++i) {
		parse_one_param(argv[i]);
	}
}

void parser::parse_one_param(const wchar_t* const param)
{
	enum States {
		none,
		flag_detect,
		flag_read,
		param_value_detect,
		param_value_read,
		param_value_read_escaped,
		param_value_quoted_detect,
		param_value_quoted_read,
		param_value_quoted_read_escaped,
	};

	States state = none;
	size_t len = wcslen(param);

	std::wstring param_name;
	std::wstring param_value;

	for (size_t i = 0; i < len; ++i) {

		switch (state)
		{
		case none:
			if (param[i] == ' ' || param[i] == '\t') {
				continue;
			}
			else if (param[i] == '-' || param[i] == '/') {
				state = flag_detect;
				continue;
			}
			break;
		case flag_detect:
			if (param[i] == '-') {
				state = flag_read;
				continue;
			}
			else if ((param[i] >= 'a' && param[i] <= 'z') || (param[i] >= '0' && param[i] <= '9') || (param[i] >= 'A' && param[i] <= 'Z')) {
				param_name += param[i];
				state = flag_read;
				continue;
			}
			else {
				state = none;
				continue;
			}
			break;
		case flag_read:
			if ((param[i] >= 'a' && param[i] <= 'z') || (param[i] >= '0' && param[i] <= '9') || (param[i] >= 'A' && param[i] <= 'Z')) {
				param_name += param[i];
				continue;
			}
			else if (param[i] == '=' || param[i] == ':') {
				state = param_value_detect;
				continue;
			}
			else {
				state = none;
				break; // invalid break
			}
			break;
		case param_value_detect:
			if ((param[i] >= 'a' && param[i] <= 'z') || (param[i] >= '0' && param[i] <= '9') || (param[i] >= 'A' && param[i] <= 'Z')) {
				param_value += param[i];
				state = param_value_read;
				continue;
			}
			else if (param[i] == '"') {
				state = param_value_quoted_detect;
				continue;
			}
			else if (param[i] == '\\') {
				state = param_value_read_escaped;
				continue;
			}
			else {
				state = none;
				break; // invalid break
			}
			break;
		case param_value_read_escaped:
			if (param[i] == 't') param_value += '\t';
			else if (param[i] == '\\') param_value += '\\';
			else if (param[i] == '"') param_value += '\"';
			state = param_value_read;
			continue;
			break;
		case param_value_read:
			param_value += param[i];
			continue;
			break;
		case param_value_quoted_detect:
			if (param[i] == '\\') {
				state = param_value_quoted_read_escaped;
				continue;
			}
			else if (param[i] == '"') {
				// Store whole param
				state = none;
				continue;
			}
			else {
				param_value += param[i];
				state = param_value_quoted_read;
				continue;
			}
			break;
		case param_value_quoted_read_escaped:
			if (param[i] == 't') param_value += '\t';
			else if (param[i] == '\\') param_value += '\\';
			else if (param[i] == '"') param_value += '\"';
			state = param_value_quoted_read;
			continue;
			break;

		case param_value_quoted_read:
			if (param[i] == '\\') {
				state = param_value_quoted_read_escaped;
				continue;
			}
			else if (param[i] == '"') {
				param_value += param[i];
				state = none;
				continue;
			}
			else {
				param_value += param[i];
				continue;
			}
			break;
		} // end of switch
	}

	m_params.insert({ param_name, param_value });
	
//	else if (state == param_value_read) {
		// if end==start read one char
		// else read end-start
//	}
}

} // End of namespace cmdp