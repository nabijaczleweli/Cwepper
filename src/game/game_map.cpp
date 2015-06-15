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


#include "game_map.hpp"
#include "../util/broken_gcc.hpp"
#include "cppformat/format.h"
#include <stdexcept>
#include <string>


using namespace sf;
using namespace std;
using namespace cpponfig;

using fmt::format;


static cell placeholder_cell;

static const auto with_bounds = [](auto & map, int x, int y) -> auto & {
	if(x < 0 || y < 0 || x >= map.cols() || y >= map.rows())
		throw out_of_range(format("Bound check: {} < 0 || {} < 0 || {} >= {} || {} >= {}", x, y, x, map.cols(), y, map.rows()));
	return map(y, x);
};


void game_map::draw(RenderTarget & target, RenderStates states) const {
	target.clear(Color::Transparent);

	RectangleShape shape(static_cast<Vector2f>(target.getSize()));
	shape.setOutlineThickness(-2);
	shape.setOutlineColor(Color::Magenta);
	shape.setFillColor(Color::Transparent);
	target.draw(shape, states);

	for(int y = 0; y < map.rows(); ++y)
		for(int x = 0; x < map.cols(); ++x)
			target.draw(map(y, x), states);
}

void game_map::config(configuration & cfg) {
	const auto p = cfg.get("game_map:mines", property("0.3", "How much of the field is contains with mines.")).floating();
	bernoulli_distribution dist(p);
	mt19937 random(random_device{}());

	for(int y = 0; y < map.rows(); ++y)
		for(int x = 0; x < map.cols(); ++x)
			map(y, x) = cell(Vector2u(x, y), cell_size, bind(ref(dist), ref(random)));
}

game_map::game_map(unsigned int width, unsigned int height, const Vector2u & destsize) : map(height, width) {
	cell_size.x = min(destsize.x / map.cols(), destsize.y / map.rows());
	cell_size.y = cell_size.x;
}

void game_map::click(int x, int y) {
	using placeholders::_1;
	using placeholders::_2;

	if(cell_size.x == 0 || cell_size.y == 0)
		return;

	x = floor(x / cell_size.x);
	y = floor(y / cell_size.y);

	if(y < map.rows() && x < map.cols())
		map(y, x).click(bind(with_bounds, ref(map), _1, _2));
}
