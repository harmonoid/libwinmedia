#include <map>
#include <string>

#include "Internal.hpp"

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
  MusicTags(wchar_t** tags) : tags_(tags) {
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

  std::wstring& album() { return album_; }
  std::wstring& album_artist() { return album_artist_; }
  std::wstring& bitrate() { return bitrate_; }
  std::wstring& composers() { return composers_; }
  std::wstring& conductors() { return composers_; }
  std::wstring& duration() { return duration_; }
  std::wstring& genre() { return genre_; }
  std::wstring& producers() { return producers_; }
  std::wstring& publisher() { return publisher_; }
  std::wstring& rating() { return rating_; }
  std::wstring& subtitle() { return subtitle_; }
  std::wstring& title() { return title_; }
  std::wstring& track_number() { return track_number_; }
  std::wstring& writers() { return writers_; }
  std::wstring& year() { return year_; }

  ~MusicTags() {
    for (int index = 0; index < 15; index++) delete tags_[index];
    delete[] tags_;
  }

 private:
  std::wstring album_;
  std::wstring album_artist_;
  std::wstring bitrate_;
  std::wstring composers_;
  std::wstring conductors_;
  std::wstring duration_;
  std::wstring genre_;
  std::wstring producers_;
  std::wstring publisher_;
  std::wstring rating_;
  std::wstring subtitle_;
  std::wstring title_;
  std::wstring track_number_;
  std::wstring writers_;
  std::wstring year_;
  wchar_t** tags_;
  friend class Tags;
};

class VideoTags {
 public:
  std::wstring& bitrate() { return bitrate_; };
  std::wstring& directors() { return directors_; };
  std::wstring& duration() { return duration_; };
  std::wstring& height() { return height_; };
  std::wstring& keywords() { return keywords_; };
  std::wstring& longitude() { return longitude_; };
  std::wstring& latitude() { return latitude_; };
  std::wstring& orientation() { return orientation_; };
  std::wstring& producers() { return producers_; };
  std::wstring& publisher() { return publisher_; };
  std::wstring& rating() { return rating_; };
  std::wstring& subtitle() { return subtitle_; };
  std::wstring& title() { return title_; };
  std::wstring& width() { return width_; };
  std::wstring& writers() { return writers_; };
  std::wstring& year() { return year_; };

  VideoTags(wchar_t** tags) : tags_(tags) {
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
  std::wstring bitrate_;
  std::wstring directors_;
  std::wstring duration_;
  std::wstring height_;
  std::wstring keywords_;
  std::wstring longitude_;
  std::wstring latitude_;
  std::wstring orientation_;
  std::wstring producers_;
  std::wstring publisher_;
  std::wstring rating_;
  std::wstring subtitle_;
  std::wstring title_;
  std::wstring width_;
  std::wstring writers_;
  std::wstring year_;
  wchar_t** tags_;
  friend class Tags;
};

class Tags {
 public:
  static MusicTags FromMusic(std::wstring uri) {
    return MusicTags(Internal::TagsFromMusic(uri.c_str()));
  }

  static VideoTags FromVideo(std::wstring uri) {
    return VideoTags(Internal::TagsFromVideo(uri.c_str()));
  }

  static void ExtractThumbnail(std::wstring uri, std::wstring save,
                               std::wstring fileName, ThumbnailMode mode,
                               int32_t size) {
    Internal::TagsExtractThumbnail(uri.c_str(), save.c_str(), fileName.c_str(),
                                   static_cast<int>(mode), size);
  }
};
}

#ifdef __cplusplus
}
#endif
#endif
