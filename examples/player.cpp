#include <iostream>
#include <string>
#include "../include/libwinmedia.hpp"

auto TO_WIDESTRING = [](const char* array) -> std::wstring {
  std::string string(array);
  return std::wstring(string.begin(), string.end());
};

int main(int ac, const char** av) {
  using namespace std;
  using namespace lwm;
  if (ac < 2) {
    wcout << L"No URI provided.\n" << L"Example Usage:\n" << av[0]
          << L" file://C:/alexmercerind/music.mp3\n" << av[0]
          << L" https://alexmercerind.github.io/video.mp4\n";
    return EXIT_FAILURE;
  }
  auto player = Player(0);
  auto media = Media(TO_WIDESTRING(av[1]));
  player.Open(vector<Media>{media});
  player.Play();
  player.events()->Position([](int position) -> void {
    wcout << L"Current playback position is " << position << L" ms.\n";
  });
  cin.get();
  return EXIT_SUCCESS;
}
