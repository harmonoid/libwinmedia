#include <map>
#include <string>

#include "internal.hpp"

#ifndef TAGS_HEADER
#define TAGS_HEADER
#ifdef __cplusplus
extern "C" {
#endif

namespace lwm {

enum ThumbnailMode {
  Picture,
  Video,
  Music,
  Document,
  List,
  Single,
};

class Tags;

class MusicTags {
 public:
  MusicTags(char** tags) : tags_(tags) {
    album_ = tags[0];
    album_artist_ = tags[1];
    bitrate_ = tags[2];
    composers_ = tags[3];
    conductors_ = tags[4];
    duration_ = tags[5];
    genre_ = tags[6];
    producers_ = tags[7];
    publisher_ = tags[8];
    rating_ = tags[9];
    subtitle_ = tags[10];
    title_ = tags[11];
    track_number_ = tags[12];
    writers_ = tags[13];
    year_ = tags[14];
  }

  std::string& album() { return album_; }
  std::string& album_artist() { return album_artist_; }
  std::string& bitrate() { return bitrate_; }
  std::string& composers() { return composers_; }
  std::string& conductors() { return composers_; }
  std::string& duration() { return duration_; }
  std::string& genre() { return genre_; }
  std::string& producers() { return producers_; }
  std::string& publisher() { return publisher_; }
  std::string& rating() { return rating_; }
  std::string& subtitle() { return subtitle_; }
  std::string& title() { return title_; }
  std::string& track_number() { return track_number_; }
  std::string& writers() { return writers_; }
  std::string& year() { return year_; }

  ~MusicTags() {
    for (int index = 0; index < 15; index++) delete tags_[index];
    delete[] tags_;
  }

 private:
  std::string album_;
  std::string album_artist_;
  std::string bitrate_;
  std::string composers_;
  std::string conductors_;
  std::string duration_;
  std::string genre_;
  std::string producers_;
  std::string publisher_;
  std::string rating_;
  std::string subtitle_;
  std::string title_;
  std::string track_number_;
  std::string writers_;
  std::string year_;
  char** tags_;
  friend class Tags;
};

class VideoTags {
 public:
  std::string& bitrate() { return bitrate_; };
  std::string& directors() { return directors_; };
  std::string& duration() { return duration_; };
  std::string& height() { return height_; };
  std::string& keywords() { return keywords_; };
  std::string& longitude() { return longitude_; };
  std::string& latitude() { return latitude_; };
  std::string& orientation() { return orientation_; };
  std::string& producers() { return producers_; };
  std::string& publisher() { return publisher_; };
  std::string& rating() { return rating_; };
  std::string& subtitle() { return subtitle_; };
  std::string& title() { return title_; };
  std::string& width() { return width_; };
  std::string& writers() { return writers_; };
  std::string& year() { return year_; };

  VideoTags(char** tags) : tags_(tags) {
    bitrate_ = tags[0];
    directors_ = tags[1];
    duration_ = tags[2];
    height_ = tags[3];
    keywords_ = tags[4];
    longitude_ = tags[5];
    latitude_ = tags[6];
    orientation_ = tags[7];
    producers_ = tags[8];
    rating_ = tags[9];
    subtitle_ = tags[10];
    title_ = tags[11];
    width_ = tags[12];
    writers_ = tags[13];
    year_ = tags[14];
    bitrate_ = tags[15];
  }

 private:
  std::string bitrate_;
  std::string directors_;
  std::string duration_;
  std::string height_;
  std::string keywords_;
  std::string longitude_;
  std::string latitude_;
  std::string orientation_;
  std::string producers_;
  std::string publisher_;
  std::string rating_;
  std::string subtitle_;
  std::string title_;
  std::string width_;
  std::string writers_;
  std::string year_;
  char** tags_;
  friend class Tags;
};

class Tags {
 public:
  static MusicTags FromMusic(std::string uri) {
    return MusicTags(Internal::TagsFromMusic(uri.c_str()));
  }

  static VideoTags FromVideo(std::string uri) {
    return VideoTags(Internal::TagsFromVideo(uri.c_str()));
  }

  static void ExtractThumbnail(std::string uri, std::string save,
                               std::string fileName, ThumbnailMode mode,
                               int32_t size) {
    Internal::TagsExtractThumbnail(uri.c_str(), save.c_str(), fileName.c_str(),
                                   static_cast<int>(mode), size);
  }
};
}  // namespace lwm

#ifdef __cplusplus
}
#endif
#endif
