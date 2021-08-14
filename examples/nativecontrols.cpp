#include <iostream>
#include <string>
#include <filesystem>
#include "../include/libwinmedia.hpp"

auto TO_WIDESTRING = [](std::string string) -> std::wstring {
  return std::wstring(string.begin(), string.end());
};

int main(int ac, const char** av) {
  auto controls = lwm::NativeControls([](auto button) -> void {
    std::wcout << L"Pressed button : " << button << L".\n";
  });
  std::wstring album_art = TO_WIDESTRING(
      "file://" + std::filesystem::current_path().string() + "/AlbumArt.PNG");
  controls.Update(std::make_shared<lwm::MusicNativeControlState>(
                      L"album_artist", L"album", L"track_count", L"artist",
                      L"title", L"track_number"),
                  album_art);
  std::cin.get();
  return EXIT_SUCCESS;
}
