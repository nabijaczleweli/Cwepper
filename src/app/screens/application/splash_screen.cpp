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
#include "main_menu_screen.hpp"
#include <thread>


using namespace sf;
using namespace std;


static volatile bool dummy_bool;

static void schedule(application & app) {
	app.schedule_screen<main_menu_screen>();
}


void splash_screen::end() {
	ended.get() = true;
	schedule(app);
}

void splash_screen::setup() {
	screen::setup();
	thread(
	    [&](unsigned int ticks, reference_wrapper<volatile bool> & ended, auto callback) {
		    volatile bool killed = false;
		    ended                = ref(killed);

		    this_thread::sleep_for(chrono::milliseconds(ticks));
		    if(!killed)
			    callback();
		  },
	    app_configuration.splash_showing_time, reference_wrapper<decltype(ended)>(ended), [&]() { return schedule(app); })
	    .detach();

	const float scale = static_cast<float>(window.getSize().y - text.getGlobalBounds().height * 1.75f) / background.getLocalBounds().height;
	background.setScale(scale, scale);
	background.setPosition(window.getSize().x / 2 - background.getGlobalBounds().width / 2, 0);

	text.setPosition(window.getSize().x / 2 - text.getGlobalBounds().width / 2, window.getSize().y - text.getGlobalBounds().height * 1.75);
}

int splash_screen::draw() {
	window.draw(background);
	window.draw(text);
	return 0;
}

int splash_screen::handle_event(const Event & event) {
	if((event.type == Event::KeyPressed && !event.key.alt && !event.key.control && !event.key.shift && !event.key.system) ||
	   event.type == Event::MouseButtonPressed)
		end();
	else if(event.type == Event::Closed)
		window.close();
	return 0;
}

splash_screen::splash_screen(application & theapp)
      : screen(theapp), background(main_texture_loader[textures_root + "/gui/main/splash.png"]), text(app_name, font_7segment), ended(ref(dummy_bool)) {
	text.setColor(Color::Green);
}

splash_screen::~splash_screen() {}
