#include <iostream>
#include <string>
#include "../include/libwinmedia.hpp"

int32_t main(int32_t ac, const char** av) {
  using namespace std;
  using namespace lwm;
  if (ac < 2) {
    cout << "No URI provided.\n"
         << "Example Usage:\n" << av[0]
         << " file://C:/alexmercerind/music.mp3\n" << av[0]
         << " https://alexmercerind.github.io/video.mp4\n";
    return EXIT_FAILURE;
  }
  auto player = Player(0);
  auto media = Media(av[1]);
  player.Open(vector<Media>{media});
  player.Play();
  player.events()->Position([](int32_t position) -> void {
    cout << "Current playback position is " << position << " ms.\n";
  });
  cin.get();
  return EXIT_SUCCESS;
}
