#include <iostream>
#include <string>
#include <filesystem>
#include "../include/libwinmedia.hpp"

int main(int ac, const char** av) {
  auto controls = lwm::NativeControls([](auto button) -> void {
    std::wcout << "Pressed button : " << button << ".\n";
  });
  auto album_art =
      "file://" + std::filesystem::current_path().string() + "/AlbumArt.PNG";
  std::cout << album_art << std::endl;
  controls.Update(std::make_shared<lwm::MusicNativeControlState>(
                      "album_artist", "album", "track_count", "artist", "title",
                      "track_number"),
                  album_art);
  std::cin.get();
  return EXIT_SUCCESS;
}
