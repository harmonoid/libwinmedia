#include <string>

#include "Internal.hpp"

#ifndef MEDIA_HEADER
#define MEDIA_HEADER
#ifdef __cplusplus
extern "C" {
#endif


class Player;


/// Creates a new Media instance using URI as string.
class Media {
public:
    int32_t duration;

    Media(std::wstring uri, bool parse = false) {
        this->id = Internal::Media_create(uri.c_str(), parse);
        if (parse) this->duration = Internal::Media_getDuration(this->id);
    }

    void dispose() {
        Internal::Media_dispose(this->id);
    }

private:
    int32_t id;

    friend class Player;
};


#ifdef __cplusplus
}
#endif
#endif
