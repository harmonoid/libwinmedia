#include <cstdio>
#include "../include/internal.hpp"

int32_t main() {
  using namespace Internal;
  // Create a list of medias.
  const char* media_uris[] = {
      "http://commondatastorage.googleapis.com/gtv-videos-bucket/sample/"
      "ForBiggerJoyrides.mp4"};
  const int media_ids[] = {0};
  // Create a player instance.
  PlayerCreate(0);
  // Set frame callback (comment out the code to prevent crash from happening).
  PlayerSetFrameEventHandler(
      0, [](uint8_t*, int32_t, int32_t width, int32_t height) {
        printf("Video width: %d, Video height: %d.", width, height);
      });
  // Open list of medias.
  PlayerOpen(0, 1, media_uris, media_ids);
  // Start playing the player.
  PlayerPlay(0);
  // Prevent console from closing.
  getchar();
  return 0;
}