#include "utils.h"

#include <cstdarg>
#include <cctype>
#include <algorithm>


bool std_string_iequals(const std::string &a, const std::string &b) {
	return std::equal(a.begin(), a.end(), b.begin(),
	                  [](const char &a, const char &b) {
		                  return std::tolower(a) == std::tolower(b);
	                  });
}


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


// From https://stackoverflow.com/a/69911/2376882
std::string std_string_format_ap_list(const char *fmt, va_list ap) {
	// Allocate a buffer on the stack that's big enough for us almost
	// all the time.  Be prepared to allocate dynamically if it doesn't fit.
	size_t size = 1024;
	char stackbuf[1024];
	std::vector<char> dynamicbuf;
	char *buf = &stackbuf[0];
	va_list ap_copy;

	while (true) {
		// Try to vsnprintf into our buffer.
		va_copy(ap_copy, ap);
		int needed = vsnprintf(buf, size, fmt, ap);
		va_end(ap_copy);

		// NB. C99 (which modern Linux and OS X follow) says vsnprintf
		// failure returns the length it would have needed.  But older
		// glibc and current Windows return -1 for failure, i.e., not
		// telling us how much was needed.

		if (needed <= (int)size && needed >= 0) {
			// It fit fine so we're done.
			return std::string(buf, (size_t) needed);
		}

		// vsnprintf reported that it wanted to write more characters
		// than we allotted.  So try again using a dynamic buffer.  This
		// doesn't happen very often if we chose our initial size well.
		size = (needed > 0) ? (needed+1) : (size*2);
		dynamicbuf.resize(size);
		buf = &dynamicbuf[0];
	}
}

std::string std_string_format(const char *fmt, ...) {
	va_list ap;
	va_start (ap, fmt);
	std::string buf = std_string_format_ap_list(fmt, ap);
	va_end (ap);
	return buf;
}
