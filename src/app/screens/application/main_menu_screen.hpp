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


#pragma once
#ifndef MAIN_MENU_SCREEN_HPP
#define MAIN_MENU_SCREEN_HPP


#include "../screen.hpp"
#include "../../../resource/localizer.hpp"
#include <functional>
#include <utility>
#include <list>


class main_menu_screen : public screen {
	public:
		enum class direction : unsigned char {up, down};
		using button_clickable = std::tuple<sf::Text, localizer::string_t, std::function<void(const sf::Event &)>,
		                                    std::function<localizer::string_t(const localizer::string_t &)>>;

	private:
		std::list<button_clickable> main_buttons;
		decltype(main_buttons.begin()) selected;

		void move_selection(direction dir);
		void select(const sf::Event & event);

	public:
		virtual void setup() override;
		virtual int draw() override;
		virtual int handle_event(const sf::Event & event) override;

		main_menu_screen(application & theapp);
		main_menu_screen(const main_menu_screen & other);
		main_menu_screen(main_menu_screen && other);
		virtual ~main_menu_screen();
};


#endif  // MAIN_MENU_SCREEN_HPP
