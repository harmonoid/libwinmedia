#include <string>

#include "Internal.hpp"

#ifndef MEDIA_HEADER
#define MEDIA_HEADER
#ifdef __cplusplus
extern "C" {
#endif


namespace lwm {


class Player;


class Media {
public:
    int32_t duration = 0;

    Media(int32_t id, std::wstring uri, bool parse = false): id(id) {
        Internal::Media_create(id, uri.c_str(), parse);
        if (parse) this->duration = Internal::Media_getDuration(this->id);
    }

    void dispose() {
        Internal::Media_dispose(this->id);
    }

private:
    int32_t id;

    friend class Player;
};


}

#ifdef __cplusplus
}
#endif
#endif
