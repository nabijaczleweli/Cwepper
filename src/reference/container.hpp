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


#pragma once
#ifndef CONTAINER_HPP
#define CONTAINER_HPP


#include "../resource/configurables_configuration.hpp"
#include "../resource/texture_loader.hpp"
#include "../resource/image_loader.hpp"
#include <audiere.h>
#include <SFML/Graphics.hpp>
#include <string>


extern const std::string assets_root;
extern const std::string textures_root;
extern const std::string font_root;
extern const std::string sound_root;

extern const std::string app_name;
extern       configurables_configuration app_configuration;

extern       texture_loader main_texture_loader;
extern       image_loader   main_image_loader;


extern const sf::Font font_standard;


extern const audiere::AudioDevicePtr audio_device;


#endif  // CONTAINER_HPP
