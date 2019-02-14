#include "utils.h"

std::string join_strings(const std::vector<std::string>& v, char c) {
	std::string s;

	for (auto p = v.begin(); p != v.end(); ++p) {
		s += *p;
		if (p != v.end() - 1)
			s += c;
	}
	return s;
}

std::vector<std::string> split_string(const std::string &s, char delim) {
	std::vector<std::string> res;
	char word[s.size() + 1];
	unsigned wordIdx = 0;
	for(const char &chr : s) {
		if (chr != delim) {
			word[wordIdx++] = chr;
		} else {
			word[wordIdx] = '\0';
			res.emplace_back(std::string(word));
			wordIdx = 0;
		}
	}
	if (wordIdx) {
		word[wordIdx] = '\0';
		res.emplace_back(std::string(word));
	}
	return res;
}
