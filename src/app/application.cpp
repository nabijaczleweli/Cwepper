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

#include "application.hpp"
#include "screens/application/splash_screen.hpp"
#include "../reference/container.hpp"
#include "../util/configurable.hpp"
#include "../util/file.hpp"
#include <iostream>
#include <chrono>
#include <thread>


using namespace sf;
using namespace std;
using namespace std::chrono;
using namespace std::chrono_literals;
using namespace cpponfig;


application::application() : force_redraw(true) {}

application::~application() {}

int application::run() {
	window.create(VideoMode::getDesktopMode(), app_name, Style::Fullscreen);

	const Image & icon = main_image_loader[textures_root + "/gui/general/window_main.png"];
	window.setIcon(icon.getSize().x, icon.getSize().x, icon.getPixelsPtr());

	schedule_screen<splash_screen>();
	return loop();
}

int application::loop() {
	volatile int result = 0;

	window.setActive(false);

	thread draw_thread([&]() {
		const auto idle_delay = 1000000us / idle_fps;

		window.setActive(true);
		while(window.isOpen()) {
			const bool wasforced = force_redraw;
			const auto start = high_resolution_clock::now();

			if(const int i = draw()) {
				result = i;
				break;
			}

			const auto end = high_resolution_clock::now();
			const auto stepped_sleep_duration = duration_cast<chrono::microseconds>(idle_delay - (end - start)) / idle_fps_chunks;

			for(unsigned int i = 0; !force_redraw && i < idle_fps_chunks; ++i) {
				this_thread::sleep_for(stepped_sleep_duration);

				if(scheduled_screen && active_screen_mutex.try_lock()) {
					while(scheduled_screen) {
						active_screen = move(scheduled_screen);
						active_screen->setup();
					}
					active_screen_mutex.unlock();
					schedule_redraw();
				}
			}
			if(wasforced)
				force_redraw = false;
		}
	});

	Event event;
	while(!result && window.waitEvent(event)) {
		if(active_screen) {
			lock_guard<mutex> locker(active_screen_mutex);
			if(const int i = active_screen->handle_event(event))
				result = i;
		}
	}

	schedule_redraw();
	if(draw_thread.joinable()) {
		cout << "Waiting for the draw thread to finish...\n";
		draw_thread.join();
	}
	cout << "The draw thread finished!\n";

	return result;
}

int application::draw() {
	window.clear(Color::Black);

	if(active_screen) {
		lock_guard<mutex> locker(active_screen_mutex);
		active_screen->draw();
	}

	window.display();
	return 0;
}

void application::schedule_redraw() {
	force_redraw = true;
}

void application::config(configuration & cfg) {
	idle_fps = cfg.get("application:idle_FPS", property("1", "FPS when nothing is happenning on-screen")).floating();
	idle_fps_chunks = cfg.get("application:idle_FPS_chunks", property("10", "Denominator for individual delay; up this, if you're "
	                                                                        "experiencing non-responiveness when switching to game window")).unsigned_integer();
}
