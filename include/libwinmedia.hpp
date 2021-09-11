/*
 * libwinmedia
 * -----------
 * v0.0.2
 * A cross-platform media playback library for C/C++ with good number of
 * features.
 *
 * Works on Windows & Linux.
 *
 * https://github.com/harmonoid/libwinmedia
 *
 *
 * Minimal Example
 * ```cpp
 * #include "libwinmedia/libwinmedia.hpp"
 *
 * int32_t main(int32_t ac, const char** av) {
 *   using namespace std;
 *   using namespace lwm;
 *   if (ac < 2) {
 *     cout << "No URI provided.\n"
 *         << "Example Usage:\n" << av[0]
 *         << " file://C:/alexmercerind/music.mp3\n" << av[0]
 *         << " https://alexmercerind.github.io/video.mp4\n";
 *     return EXIT_FAILURE;
 *   }
 *   auto player = Player(0);
 *   auto media = Media(av[1]);
 *   player.Open(vector<Media>{media});
 *   player.Play();
 *   player.events()->Position([](int32_t position) -> void {
 *     cout << "Current playback position is " << position << " ms.\n";
 *   });
 *   cin.get();
 *   return EXIT_SUCCESS;
 * }
 * ```
 *
 * MIT License
 *
 * Copyright (c) 2021 Hitesh Kumar Saini <saini123hitesh@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in
 * all
 * copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "media.hpp"
#include "nativecontrols.hpp"
#include "player.hpp"
#include "tags.hpp"
