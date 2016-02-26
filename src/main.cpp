// The MIT License (MIT)

// Copyright (c) 2015 nabijaczleweli

// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


#include "app/application.hpp"
#include "reference/container.hpp"
#include "resource/localizer.hpp"
#include "util/file.hpp"
#include <iostream>
#include <random>
#include <string>
#include <stdexcept>


using namespace std;


static application app;


void init_app(int argc, char * argv[]);
void log_versions();


int main(int argc, char * argv[]) {
	log_versions();
	init_app(argc, argv);
	const auto result = app.run();
	if(result)
		cerr << "`app.run()` failed! Oh noes!";
	return result;
}


void init_app(int, char * []) {
	fallback_izer.open();
	local_izer.open(app_configuration.language);
	global_izer.merge(local_izer).merge(fallback_izer);
}

void log_versions() {
	cout << "Compiled under "
#ifdef _WIN32
	        "Windows"
#elif defined(unix) || defined(__unix__) || defined(__unix)
	        "UNIX"
#elif defined(__APPLE__)
	        "Mac OS X"
#elif defined(__linux__)
	        "Linux"
#elif defined(__FreeBSD__)
	        "FreeBSD"
#else
	        "an unknown OS"
#endif
	        ".\n";

	cout << "GCC version " << __GNUC__ << '.' << __GNUC_MINOR__ << '.' << __GNUC_PATCHLEVEL__ << ".\n"
	     << "SFML version " << SFML_VERSION_MAJOR << '.' << SFML_VERSION_MINOR << ".\n";
}
