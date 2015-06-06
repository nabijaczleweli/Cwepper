// The MIT License (MIT)

// Copyright (c) 2014 nabijaczleweli

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


#include "container.hpp"


using namespace sf;
using namespace std;
using namespace audiere;


auto load_font(const char * name) {
	Font tmp;
	tmp.loadFromFile(font_root + '/' + name);
	return move(tmp);
}


const string assets_root("./assets");
const string textures_root("./assets/textures");
const string font_root("./assets/fonts");
const string sound_root("./assets/sound");
const string localization_root("./assets/lang");

const string app_name("Cwepper");
      string app_language("en_US");
      configurables_configuration app_configuration("./" + app_name + ".cfg");

      texture_loader main_texture_loader(main_image_loader);
      image_loader   main_image_loader;

      localizer fallback_izer(nullptr);
      localizer local_izer(nullptr);
      localizer global_izer(nullptr);

      vector<string> present_languages;


const Font font_standard(load_font("2-Questa_Grande_Regular.otf"));
const Font font_7segment(load_font("digital-7 (mono).ttf"));
const Font font_swirly(load_font("MACABRA_.ttf"));


//const AudioDevicePtr audio_device(OpenDevice());
