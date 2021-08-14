#include <iostream>
#include <string>
#include <filesystem>
#include "../include/libwinmedia.hpp"

auto TO_WIDESTRING = [](std::string string) -> std::wstring {
  return std::wstring(string.begin(), string.end());
};

int main(int ac, const char** av) {
  std::wstring music_file = TO_WIDESTRING(
      "file://" + std::filesystem::current_path().string() + "/Music.M4A");
  auto tags = lwm::Tags::FromMusic(std::wstring(music_file));
  std::wcout << L"album        : " << tags.album() << ".\n"
             << L"album_artist : " << tags.album_artist() << ".\n"
             << L"bitrate      : " << tags.bitrate() << ".\n";
  lwm::Tags::ExtractThumbnail(
      music_file, TO_WIDESTRING(std::filesystem::current_path().string()),
      L"ExtractedAlbumArt.PNG", lwm::ThumbnailMode::Music, 400);
  std::wcout << "Saved thumbnail to ./ExtractedAlbumArt.PNG.\n";
  std::cin.get();
  return EXIT_SUCCESS;
}
