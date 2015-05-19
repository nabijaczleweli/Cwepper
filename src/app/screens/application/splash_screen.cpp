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


#include "splash_screen.hpp"
#include "../../../reference/container.hpp"
#include "../../application.hpp"
#include <thread>


using namespace sf;
using namespace std;
using namespace cpponfig;


class test_main_screen : public screen {
		Text txt;

		virtual int draw() override {
			txt.setString(txt.getString() + "_");
			window.draw(txt);
			return 0;
		}

		public:
			test_main_screen(application & theapp) : screen(theapp), txt("HUEHU", font_standard) {}
			test_main_screen(const test_main_screen & other) : screen(other), txt(other.txt) {}
			test_main_screen(test_main_screen && other) : screen(move(other)), txt(move(other.txt)) {}
			virtual ~test_main_screen() {}
};


static volatile bool dummy_bool;

static void schedule(application & app) {
	app.schedule_screen<test_main_screen>();
}

void splash_screen::end() {
	ended.get() = true;
	schedule(app);
}


void splash_screen::setup() {
	screen::setup();
	thread thr([&](unsigned int ticks, reference_wrapper<reference_wrapper<volatile bool>> ended, auto callback) {
		volatile bool killed = false;
		ended.get() = ref(killed);

		this_thread::sleep_for(chrono::milliseconds(ticks));
		if(!killed)
			callback();
	}, showing_time, reference_wrapper<decltype(ended)>(ended), bind(schedule, ref(app)));
	thr.detach();

	const float scale = static_cast<float>(window.getSize().y - text.getGlobalBounds().height * 1.5f) / background.getLocalBounds().height;
	background.setScale(scale, scale);
	background.setPosition(window.getSize().x / 2 - background.getGlobalBounds().width / 2, 0);

	//text.setPosition(window.getSize().x / 2 - text.getGlobalBounds().width / 2, window.getSize().y - text.getGlobalBounds().height * 1.25);
	text.setPosition(0, 0);
}

int splash_screen::draw() {
	window.draw(background);
	window.draw(text);
	return 0;
}

int splash_screen::handle_event(const Event & event) {
	if((event.type == Event::KeyPressed && !event.key.alt && !event.key.control && !event.key.shift && !event.key.system) || event.type == Event::MouseButtonPressed)
		end();
	else if(event.type == Event::Closed)
		window.close();
	return 0;
}

void splash_screen::config(configuration & cfg) {
	showing_time = cfg.get("splash_screen:showing_time", property("2000", "[ms]")).unsigned_integer();
}

splash_screen::splash_screen(application & theapp) : screen(theapp), configurable(), background(main_texture_loader[textures_root + "/gui/main/splash.png"]),
                                                     text(app_name, font_pixelish), ended(ref(dummy_bool)) {
	text.setColor(Color::Green);
}
splash_screen::splash_screen(const splash_screen & other) : screen(other), configurable(other), background(other.background), text(other.text),
                                                            ended(other.ended) {}
splash_screen::splash_screen(splash_screen && other) : screen(move(other)), configurable(move(other)), background(move(other.background)),
                                                       text(move(other.text)), ended(move(other.ended)) {}

splash_screen::~splash_screen() {}
