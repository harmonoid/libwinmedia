#include <iostream>
#include <string>
#include <filesystem>
#include "../include/libwinmedia.hpp"

int main(int ac, const char** av) {
  std::string music_file =
      "file://" + std::filesystem::current_path().string() + "/Music.M4A";
  auto tags = lwm::Tags::FromMusic(music_file);
  std::cout << "album        : " << tags.album() << ".\n"
            << "album_artist : " << tags.album_artist() << ".\n"
            << "bitrate      : " << tags.bitrate() << ".\n";
  lwm::Tags::ExtractThumbnail(
      music_file, std::filesystem::current_path().string(),
      "ExtractedAlbumArt.PNG", lwm::ThumbnailMode::Music, 400);
  std::wcout << "Saved thumbnail to ./ExtractedAlbumArt.PNG.\n";
  std::cin.get();
  return EXIT_SUCCESS;
}
