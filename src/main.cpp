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
#include "util/configurable.hpp"
#include "util/broken_gcc.hpp"
#include "util/file.hpp"
#include "cpponfiguration/cpponfig_version.hpp"
#include <iostream>
#include <random>
#include <stdexcept>


using namespace std;
using namespace cpponfig;


static application * app;


void init_app(int argc, char * argv[]);
void deinit_app();
void init_deps();


int main(int argc, char * argv[]) {
	init_deps();
	init_app(argc, argv);
	const auto result = app->run();
	deinit_app();
	if(result)
		cerr << "`app->run()` failed! Oh noes!";
	return result;
}


void init_app(int, char * []) {
	fallback_izer.open();
	local_izer.open(app_language);
	global_izer.merge(local_izer).merge(fallback_izer);

	if(!app)
		app = new application;
	else
		throw invalid_argument("`app` has been tampered with (`!= nullptr`)! Value: " + to_string<void *>(app));

	app_configuration.configure();
}

void deinit_app() {
	delete app;
	app = nullptr;
}

void init_deps() {
	class deps_configable : public configurable {
	private:
		virtual void config(configuration & cfg) override {
			cout << "GCC version " << __GNUC__ << '.' << __GNUC_MINOR__ << '.' << __GNUC_PATCHLEVEL__ << " doesn\'t need initialization.\n"
			     << "SFML version " << SFML_VERSION_MAJOR << '.' << SFML_VERSION_MINOR << " doesn\'t need initialization.\n";

			present_languages = available_languages();
			if(cfg.contains("system:language"))
				app_language = cfg.get("system:language").textual();
			else {
				property files(app_language, "Available languages: ");

				for(const auto & name : present_languages)
					files.comment += name + ", ";

				files.comment = files.comment.substr(0, files.comment.size() - 2);
				cfg.get("system:language", files);
			}
		}

	public:
		deps_configable() : configurable(nothrow) {}

		void preconfig() {
			cout << "Cpponfiguration version " << cpponfiguration_version << " initializing...\n";
			configuration::datetime_footer_type = configuration::datetime_mode::gmt;
			cout << "Cpponfiguration initialized.\n";
		}
	};


	cout << "Initializing dependencies under "
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
	        "...\n\n";


	deps_configable dependencies_config;
	dependencies_config.preconfig();
	app_configuration.load();
	app_configuration.add(dependencies_config);
	app_configuration.configure();
	app_configuration.sof_comments = {"This is " + app_name + "\'s configuration file.", "Modify those values at will, but if",
	                                  "you break anything, it's your fault."};
	cout << "\nAll dependencies initialized.\n\n";
}
