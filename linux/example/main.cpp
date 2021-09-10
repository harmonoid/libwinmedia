#include "player.hpp"

extern "C" {

using dispatch_fn_t = std::function<void()>;

void dispatch(std::function<void()> f) {
  g_idle_add_full(G_PRIORITY_HIGH_IDLE, (GSourceFunc)([](void* f) -> int {
                    (*static_cast<dispatch_fn_t*>(f))();
                    return G_SOURCE_REMOVE;
                  }),
                  new std::function<void()>(f),
                  [](void* f) { delete static_cast<dispatch_fn_t*>(f); });
}

void StartPlayer() {
  dispatch([]() -> void {
    auto player = new Player(0, true, L"libwinmedia");
    new std::thread([=]() {
      player->Open(
          "file:///home/alexmercerind/Documents/libwinmedia/video.mkv");
      player->Play();
    });
  });
}
}

int32_t main() {
  auto player = new Player(0, true, L"libwinmedia");
  player->SetPositionEventHandler(
      [](int32_t position) { std::cout << position << "\n"; });
  new std::thread([=]() {
    player->Open(
        std::vector<std::string>{
            "file:///home/alexmercerind/Documents/libwinmedia/video.mkv"},
        std::vector<int32_t>{0});
    player->Play();
    player->Seek(10000);
    player->SetVolume(0.5);
  });
  player->Run();
  return EXIT_SUCCESS;
}