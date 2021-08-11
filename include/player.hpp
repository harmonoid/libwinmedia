/* TODO: Update `Player` class with event handlers. */

#include <functional>

#include "Internal.hpp"
#include "Media.hpp"

#ifndef PLAYER_HEADER
#define PLAYER_HEADER
#ifdef __cplusplus
extern "C" {
#endif


namespace lwm {


class Player;


/* Sets event listeners for various events in `Player`.
 * Takes a function or lambda as an argument.
 * Following are the currently available listeners.
 * Player::events::isPlaying
 * Player::events::isCompleted
 * Player::events::isBuffering
 * Player::events::volume
 * Player::events::rate
 * Player::events::position
 * Player::events::duration
 * 
 * TODO: Make this class friendly for multiple instances.
 */
class PlayerEvents {
public:
    PlayerEvents() {};

    PlayerEvents(int32_t id): id(id) {}

    void isPlaying(std::function<void(bool)> lambda) {
        PlayerEvents::_isPlaying = lambda;
        Internal::Player_setIsPlayingEventHandler(this->id, &PlayerEvents::__isPlaying);
    }

    void isCompleted(std::function<void(bool)> lambda) {
        PlayerEvents::_isCompleted = lambda;
        Internal::Player_setIsCompletedEventHandler(this->id, &PlayerEvents::__isCompleted);
    }

    void isBuffering(std::function<void(bool)> lambda) {
        PlayerEvents::_isBuffering = lambda;
        Internal::Player_setIsBufferingEventHandler(this->id, &PlayerEvents::__isBuffering);
    }

    void volume(std::function<void(float)> lambda) {
        PlayerEvents::_volume = lambda;
        Internal::Player_setVolumeEventHandler(this->id, &PlayerEvents::__volume);
    }

    void rate(std::function<void(float)> lambda) {
        PlayerEvents::_rate = lambda;
        Internal::Player_setRateEventHandler(this->id, &PlayerEvents::__rate);
    }

    void position(std::function<void(int32_t)> lambda) {
        PlayerEvents::_position = lambda;
        Internal::Player_setPositionEventHandler(this->id, &PlayerEvents::__position);
    }

    void duration(std::function<void(int32_t)> lambda) {
        PlayerEvents::_duration = lambda;
        Internal::Player_setDurationEventHandler(this->id, &PlayerEvents::__duration);
    }

private:
    int32_t id = 0;

    static inline std::function<void(bool)> _isPlaying;

    static inline void __isPlaying(bool value) {
        PlayerEvents::_isPlaying(value);
    }

    static inline std::function<void(bool)> _isCompleted;

    static inline void __isCompleted(bool value) {
        PlayerEvents::_isCompleted(value);
    }

    static inline std::function<void(bool)> _isBuffering;

    static inline void __isBuffering(bool value) {
        PlayerEvents::_isBuffering(value);
    }

    static inline std::function<void(float)> _volume;

    static inline void __volume(float value) {
        PlayerEvents::_volume(value);
    }

    static inline std::function<void(float)> _rate;

    static inline void __rate(float value) {
        PlayerEvents::_rate(value);
    }

    static inline std::function<void(int32_t)> _position;

    static inline void __position(int32_t value) {
        PlayerEvents::_position(value);
    }

    static inline std::function<void(int32_t)> _duration;

    static inline void __duration(int32_t value) {
        PlayerEvents::_duration(value);
    }

    friend class Player;
};


/* Creates a new `Player` instance for media playback. */
class Player {
public:
    Player(int32_t id, bool showVideo = false): id(id) {
        Internal::Player_create(this->id, showVideo);
        this->events.id = this->id;
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

    void setPosition(int32_t position) {
        Internal::Player_setPosition(this->id, position);
    }

    void setVolume(float volume) {
        Internal::Player_setVolume(this->id, volume);
    }

    void setRate(float rate) {
        Internal::Player_setRate(this->id, rate);
    }

    void setAudioBalance(float audioBalance) {
        Internal::Player_setAudioBalance(this->id, audioBalance);
    }

    void setAutoplay(bool autoplay) {
        Internal::Player_setAutoplay(this->id, autoplay);
    }

    void setIsLooping(bool looping) {
        Internal::Player_setAutoplay(this->id, looping);
    }

    int32_t getPosition() {
        return Internal::Player_getPosition(this->id);
    }

    float getVolume() {
        return Internal::Player_getVolume(this->id);
    }

    int32_t getRate() {
        return Internal::Player_getRate(this->id);
    }

    float getAudioBalance() {
        return Internal::Player_getAudioBalance(this->id);
    }

    bool isAutoplay() {
        return Internal::Player_isAutoplay(this->id);
    }

    float isLooping() {
        return Internal::Player_isLooping(this->id);
    }

    void dispose() {
        Internal::Player_dispose(this->id);
    }

    PlayerEvents events;
    
private:
    int32_t id;
};


}


#ifdef __cplusplus
}
#endif
#endif
