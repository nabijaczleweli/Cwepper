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
#include "cppformat/format.h"
#include <memory>


using namespace std;
using namespace sf;

using fmt::format;


typedef main_menu_screen::direction direction;


const constexpr static auto interlap_checker = [&](const auto & button, const auto & xpos, const auto & ypos) {
	return get<0>(button).getGlobalBounds().contains(xpos, ypos);
};

const constexpr static auto on_select = [&](const Event & event, const auto & callback) {
	if((event.type == Event::KeyPressed && (event.key.code == Keyboard::Key::Return || event.key.code == Keyboard::Key::Space)) ||
	   (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left))
		callback();
};

const constexpr static auto simple_translate = [&](const auto & from) {
	return global_izer.translate(from);
};


struct locale_changer {
	application & app;
	decltype(present_languages.cbegin()) lang = find(present_languages.cbegin(), present_languages.cend(), app_language);

	locale_changer(application & theapp) : app(theapp) {}

	localizer::string_t operator()(const localizer::string_t & from) {
		auto trans = format(simple_translate(from), *lang);

		if(lang != present_languages.cbegin())
			trans.insert(0, simple_translate("gui.application.arrow.left"));
		if(lang != --present_languages.cend())
			trans += simple_translate("gui.application.arrow.right");

		return trans;
	}

	void operator()(const Event & event) {
		const auto pre = lang;

		if(event.type == Event::KeyPressed)
			switch(event.key.code) {
				case Keyboard::Key::Left :
					if(lang != present_languages.cbegin())
						--lang;
					break;
				case Keyboard::Key::Right :
					if(lang != --present_languages.cend())
						++lang;
					break;
				default:
					break;
			}

		if(pre != lang) {
			local_izer = localizer(*lang);
			global_izer = localizer(local_izer, fallback_izer);
			app.schedule_redraw();
		}
	}
};


void main_menu_screen::move_selection(direction dir) {
	const auto pre = selected;

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

void main_menu_screen::select(const Event & event) {
	get<2>(*selected)(event);
}

void main_menu_screen::setup() {
	screen::setup();
}

int main_menu_screen::draw() {
	const auto & winsize = window.getSize();

	const unsigned int selected_ridx = distance(selected, main_buttons.end());
	unsigned int buttidx = 0;
	unsigned int texty = winsize.y * (7.f / 8.f);

	for_each(main_buttons.rbegin(), main_buttons.rend(), [&](auto & button) {
		static const auto line_spacing = winsize.y / 90.f;


		auto & txt = get<0>(button);

		txt.setString(get<3>(button)(get<1>(button)));
		++buttidx;

		texty -= txt.getGlobalBounds().height + line_spacing;
		txt.setPosition((winsize.x * (59.f / 60.f)) - txt.getGlobalBounds().width, texty);
		txt.setColor(buttidx == selected_ridx ? Color::Red : Color::White);

		window.draw(txt);
	});

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
			if(find_if(main_buttons.begin(), main_buttons.end(), bind(interlap_checker, _1, event.mouseButton.x, event.mouseButton.y)) != main_buttons.end())
				select(event);
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
				default:
					select(event);
					break;
			}
			break;
		default:
			break;
	}

	return 0;
}

main_menu_screen::main_menu_screen(application & theapp) : screen(theapp) {
	using placeholders::_1;

	main_buttons.emplace_back(Text("", font_swirly), L"gui.application.text.start"s, bind(on_select, _1, [&]() {
		app.schedule_screen<main_game_screen>();
	}), simple_translate);
	main_buttons.emplace_back(Text("", font_swirly), L"gui.application.text.quit"s, bind(on_select, _1, [&]() {
		window.close();
	}), simple_translate);

	const auto switcher = make_shared<locale_changer>(app);
	main_buttons.emplace_back(Text("", font_swirly), L"gui.application.text.switch_locale"s, [=](const Event & event) {
		(*switcher)(event);
	}, [=](const localizer::string_t & from) {
		return (*switcher)(from);
	});

	selected = main_buttons.begin();
}
main_menu_screen::main_menu_screen(const main_menu_screen & other) : screen(other), main_buttons(other.main_buttons), selected(other.selected) {}
main_menu_screen::main_menu_screen(main_menu_screen && other) : screen(move(other)), main_buttons(move(other.main_buttons)), selected(other.selected) {}

main_menu_screen::~main_menu_screen() {}
