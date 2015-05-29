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


#include "main_menu_screen.hpp"
#include "../../../reference/container.hpp"
#include "../../../util/broken_gcc.hpp"
#include "../../application.hpp"
#include "../game/main_game_screen.hpp"


using namespace std;
using namespace sf;


typedef main_menu_screen::direction direction;


const constexpr static auto interlap_checker = [&](const auto & button, const auto & xpos, const auto & ypos) {
	return button.first.getGlobalBounds().contains(xpos, ypos);
};


void main_menu_screen::move_selection(direction dir) {
	auto pre = selected;
	switch(dir) {
		case direction::up :
			if(selected != main_buttons.begin())
				--selected;
			break;
		case direction::down :
			if(selected != --main_buttons.end())
				++selected;
			break;
	}

	if(pre != selected)
		app.schedule_redraw();
}

void main_menu_screen::press_button() {
	(selected->second)(selected->first);
}

void main_menu_screen::setup() {
	screen::setup();

	const auto & winsize(window.getSize());

	for(auto cur = main_buttons.rbegin(), end = main_buttons.rend(); cur != end; ++cur) {
		auto & button = *cur;
		const auto buttidx = distance(main_buttons.rbegin(), cur);

		button.first.setPosition((winsize.x * (59.f / 60.f)) - button.first.getGlobalBounds().width,
		                         (winsize.y * (7.f / 8.f)) - (buttidx + 1) * button.first.getGlobalBounds().height - (winsize.y * (buttidx / 90.f)));
	}
}

int main_menu_screen::draw() {
	for(auto & button : main_buttons) {
		// Force all buttons except for selected to White, and force selected to Red
		button.first.setColor(Color::White);
		selected->first.setColor(Color::Red);

		window.draw(button.first);
	}

	return 0;
}

int main_menu_screen::handle_event(const Event & event) {
	using placeholders::_1;

	if(int i = screen::handle_event(event))
		return i;

	switch(event.type) {
		case Event::MouseMoved : {
				const auto itr = find_if(main_buttons.begin(), main_buttons.end(), bind(interlap_checker, _1, event.mouseMove.x, event.mouseMove.y));
				if(itr != main_buttons.end()) {
					selected = itr;
					app.schedule_redraw();
				}
			}
			break;

		case Event::MouseButtonPressed :
			if(event.mouseButton.button == Mouse::Left && find_if(main_buttons.begin(), main_buttons.end(), bind(interlap_checker, _1, event.mouseButton.x,
			                                                                                                     event.mouseButton.y)) != main_buttons.end())
				press_button();
			break;

		case Event::MouseWheelMoved :
			move_selection((event.mouseWheel.delta > 0) ? direction::up : direction::down);
			break;
		case Event::KeyPressed :
			switch(event.key.code) {
				case Keyboard::Key::Up :
					move_selection(direction::up);
					break;
				case Keyboard::Key::Down :
					move_selection(direction::down);
					break;
				case Keyboard::Key::Return :
				case Keyboard::Key::Space :
					press_button();
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}

	return 0;
}

main_menu_screen::main_menu_screen(application & theapp) : screen(theapp) {
	main_buttons.emplace_back(Text(global_izer.translate_key("gui.application.text.start"), font_standard/*swirly*/), [&](Text &) {
		app.schedule_screen<main_game_screen>();
	});
	main_buttons.emplace_back(Text(global_izer.translate_key("gui.application.text.quit"), font_standard/*swirly*/), [&](Text &) {
		window.close();
	});

	selected = main_buttons.begin();
}
main_menu_screen::main_menu_screen(const main_menu_screen & other) : screen(other), main_buttons(other.main_buttons), selected(other.selected) {}
main_menu_screen::main_menu_screen(main_menu_screen && other) : screen(move(other)), main_buttons(move(other.main_buttons)), selected(other.selected) {}

main_menu_screen::~main_menu_screen() {}
