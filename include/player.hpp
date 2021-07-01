#include "Internal.hpp"
#include "Media.hpp"

#ifndef PLAYER_HEADER
#define PLAYER_HEADER
#ifdef __cplusplus
extern "C" {
#endif


namespace WinMedia {

/// Creates a new `Player` instance for `Media` playback.
class Player {
public:
    Player() {
        this->id = Internal::Player_create();
    }
    
    void open(Media media) {
        Internal::Player_open(this->id, media.id);
    }

    void play() {
        Internal::Player_play(this->id);
    }

    void pause() {
        Internal::Player_pause(this->id);
    }

    void setVolume(float volume) {
        Internal::Player_setVolume(this->id, volume);
    }

    void setRate(float rate) {
        Internal::Player_setRate(this->id, rate);
    }

    int32_t getRate() {
        return Internal::Player_getRate(this->id);
    }

    int32_t getPosition() {
        return Internal::Player_getPosition(this->id);
    }

    float getVolume() {
        return Internal::Player_getVolume(this->id);
    }

    float isAutoPlay() {
        return Internal::Player_isAutoplay(this->id);
    }

    float isLooping() {
        return Internal::Player_isLooping(this->id);
    }

    void dispose() {
        Internal::Player_dispose(this->id);
    }

private:
    int32_t id;
};


}


#ifdef __cplusplus
}
#endif
#endif
