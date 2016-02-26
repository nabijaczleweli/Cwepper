// The MIT License (MIT)

// Copyright (c) 2016 nabijaczleweli

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


#include "config.hpp"
#include "cereal/cereal.hpp"
#include "cereal/archives/json.hpp"
#include <fstream>


using namespace std;


template <class Archive>
void serialize(Archive & archive, cwepper_configuration & cc) {
	archive(cereal::make_nvp("application:idle_FPS", cc.idle_fps), cereal::make_nvp("application:idle_FPS_chunks", cc.idle_fps_chunks),
	        cereal::make_nvp("game_map:mines", cc.mine_distribution), cereal::make_nvp("system:language", cc.language),
	        cereal::make_nvp("splash_screen:showing_time", cc.splash_showing_time));
}


cwepper_configuration::cwepper_configuration(string && fname) : filename(move(fname)) {
	ifstream configfile(filename);
	if(configfile.is_open()) {
		cereal::JSONInputArchive archive(configfile);
		try {
			archive(*this);
		} catch(cereal::RapidJSONException &) {
		}
	}
}

cwepper_configuration::~cwepper_configuration() {
	ofstream configfile(filename);
	cereal::JSONOutputArchive archive(configfile);
	archive(cereal::make_nvp("Cwepper configuration", *this));
}
