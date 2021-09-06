#include "player.hpp"

int32_t main() {
  auto player = std::make_unique<Player>(0, true, L"libwinmedia");
  std::thread thread([player_ptr = player.get()]() -> void {
    std::this_thread::sleep_for(std::chrono::seconds(5));

    player_ptr->webview()->dispatch([=]() -> void {
      player_ptr->Open(
          "file:///home/alexmercerind/Documents/libwinmedia/video.mkv");
      player_ptr->Play();
    });
  });
  player->Run();
  std::cin.get();
  return EXIT_SUCCESS;
}
