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
  Media(std::wstring uri, int32_t id = 0, bool parse = false)
      : id_(id), uri_(uri) {
    if (parse) {
      Internal::MediaCreate(id_, uri.c_str(), parse);
      duration_ = Internal::MediaGetDuration(id_);
    }
  }

  int32_t id() const { return id_; }

  std::wstring& uri() { return uri_; }

  int32_t duration() const { return duration_; }

  void Dispose() { Internal::MediaDispose(id_); }

 private:
  int32_t id_;
  std::wstring uri_;
  int32_t duration_;
  friend class Player;
};
}

#ifdef __cplusplus
}
#endif
#endif
