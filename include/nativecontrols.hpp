#include <functional>

#include "Internal.hpp"

#ifndef NATIVECONTROLS_HEADER
#define NATIVECONTROLS_HEADER
#ifdef __cplusplus
extern "C" {
#endif


namespace wm {


static bool nativeControlsExist;


/* Defines event executed by user on the native media controls. */
enum NativeControlsButton {
    play,
    pause,
    stop,
    record,
    fastForward,
    rewind,
    next,
    previous,
    channelUp,
    channelDown
};


/* Used to create native media controls with manual event handling. */
class NativeControls {
public:

    /* Creates `NativeControls` instance & takes a function as argument for sending back event updates as a callback. */
    NativeControls(std::function<void(NativeControlsButton)> handler) {
        if (!nativeControlsExist) {
            NativeControls::handler = handler;
            Internal::NativeControls_create(
                &NativeControls::_handler
            );
            nativeControlsExist = true;
        }
    }

private:
    static std::function<void(NativeControlsButton)> handler;

    static void _handler(int32_t button) {
        NativeControls::handler(
            static_cast<NativeControlsButton>(button)
        );
    }
};


}


#ifdef __cplusplus
}
#endif
#endif
