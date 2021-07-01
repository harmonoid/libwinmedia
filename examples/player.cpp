#include "../include/libwinmedia.hpp"

#include <iostream>
#include <string>


using namespace WinMedia;
using namespace std;

int32_t main(int argc, const char* argv[]) {
    if (argc < 2) {
        cout << "No media provided.\n"
        << "Example Usage:\n"
        << argv[0] << " file://C:/Music/MySong.mp3\n"
        << argv[0] << " https://www.example.com/MyVideo.mp4\n";
        return EXIT_FAILURE;
    }
    // Create a `Media`.
    string uri(argv[1]);
    Media media = Media(wstring(uri.begin(), uri.end()));
    // Create a new `Player` & open `Media` in it.
    Player player = Player();
    player.open(media);
    player.play();
    cout << "Playing...\n";
    // Prevent console from closing.
    cin.get();
    return EXIT_SUCCESS;
}
