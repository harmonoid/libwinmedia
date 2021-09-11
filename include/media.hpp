#include <string>

#include "internal.hpp"

#ifndef MEDIA_HEADER
#define MEDIA_HEADER
#ifdef __cplusplus
extern "C" {
#endif

namespace lwm {

class Player;

class Media {
 public:
  Media(std::string uri, int32_t id = 0, bool parse = false)
      : id_(id), uri_(uri) {
#ifndef __linux__
    // TODO (alexmercerind): Add Linux support.
    if (parse) {
      Internal::MediaCreate(id_, uri.c_str(), parse);
      duration_ = Internal::MediaGetDuration(id_);
    }
#endif
  }

  int32_t id() const { return id_; }

  std::string& uri() { return uri_; }

  int32_t duration() const { return duration_; }

  void Dispose() {
#ifndef __linux__
    Internal::MediaDispose(id_);
#endif
  }

 private:
  int32_t id_;
  std::string uri_;
  int32_t duration_;
  friend class Player;
};
}  // namespace lwm

#ifdef __cplusplus
}
#endif
#endif
