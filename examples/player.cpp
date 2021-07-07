#include "../include/libwinmedia.hpp"

#include <iostream>
#include <string>


using namespace wm;

int32_t main(int argc, const char* argv[]) {
    if (argc < 2) {
        std::wcout << L"No media provided.\n"
        << L"Example Usage:\n"
        << argv[0] << L" file://C:/Music/MySong.mp3\n"
        << argv[0] << L" https://www.example.com/MyVideo.mp4\n";
        return EXIT_FAILURE;
    }
    std::string uri(argv[1]);
    Media media = Media(std::wstring(uri.begin(), uri.end()), true);
    std::cout << "Media duration: " << media.duration << " milliseconds.\n";
    Player player = Player();
    player.open(media);
    player.play();
    std::wcout << L"Playing " << argv[1] << " ...\n";
    std::cin.get();
    return EXIT_SUCCESS;
}
