#include <iostream>
#include <string>
#include "include/libwinmedia.hpp"

int32_t main(int argc, const char* argv[]) {
    using namespace wm;
    if (argc < 2) {
        std::wcout << L"No URI provided.\n";
        std::wcout << L"Example Usage:\n";
        std::wcout << argv[0] << L" file://C:/alexmercerind/music.mp3\n";
        std::wcout << argv[0] << L" https://alexmercerind.github.io/video.mp4\n";
        return EXIT_FAILURE;
    }
    std::string uri(argv[1]);
    Media media = Media(std::wstring(uri.begin(), uri.end()));
    Player player = Player(true);
    player.open(media);
    player.play();
    std::wcout << L"Playing " << argv[1] << " ...\n";
    /* Prevent console from closing. */
    std::cin.get();
    return EXIT_SUCCESS;
}
