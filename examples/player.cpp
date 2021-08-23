#include <iostream>
#include <string>
#include <thread>
#include "../include/libwinmedia.hpp"

auto TO_WIDESTRING = [](const char* array) -> std::wstring {
  std::string string = std::string(array);
  return std::wstring(string.begin(), string.end());
};

int main(int ac, const char** av) {
  if (ac < 2) {
    std::wcout << L"No URI provided.\n" << L"Example Usage:\n" << av[0]
               << L" file://C:/alexmercerind/music.mp3\n" << av[0]
               << L" https://alexmercerind.github.io/video.mp4\n";
    return EXIT_FAILURE;
  }
  auto player = lwm::Player(0);
  auto media = std::make_shared<lwm::Media>(0, TO_WIDESTRING(av[1]));
  std::vector<std::shared_ptr<lwm::Media>> medias;
  for (int i = 1; i < ac; i++) {
    medias.emplace_back(std::make_shared<lwm::Media>(0, TO_WIDESTRING(av[1])));
  }
  std::cout << medias.size() << "\n";
  player.Open(medias);
  player.Play();
  player.events()->Position([](int position) -> void {
    std::wcout << L"Position : " << position << L".\n";
  });
  std::this_thread::sleep_for(std::chrono::seconds(2));
  player.Next();
  std::cin.get();
  return EXIT_SUCCESS;
}
