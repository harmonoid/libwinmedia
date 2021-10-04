#ifndef UNICODE
#define UNICODE
#endif
#include <ctime>
#include <filesystem>
#include <fstream>
#include <future>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "../external/webview/webview.h"

#ifndef PLAYER_H_
#define PLAYER_H_

class Player {
 public:
  Player(int32_t id, bool show_window, std::string window_title);

  bool is_playing() const { return is_playing_; }
  bool is_buffering() const { return is_buffering_; }
  bool is_completed() const { return is_completed_; }
  int32_t position() const { return position_; }
  int32_t duration() const { return duration_; }
  float download_progress() const { return download_progress_; }
  float volume() const { return volume_; }
  float rate() const { return rate_; }
  int32_t index() const { return index_; }
  std::vector<int32_t>& media_ids() { return media_ids_; }
  std::vector<std::string>& media_uris() { return media_uris_; }
  bool is_autorepeat() const { return is_autorepeat_; }
  bool is_looping() const { return is_looping_; }
  bool is_shuffling() const { return is_shuffling_; }

  void ShowWindow();

  void CloseWindow();

  void Open(std::vector<std::string> uris, std::vector<int32_t> ids);

  void Add(std::string uri, int32_t id);

  void Play();

  void Pause();

  void Stop();

  void Next();

  void Back();

  void Jump(int32_t index);

  void Seek(int32_t position);

  void SetVolume(float volume);

  void SetRate(float rate);

  void SetIsAutoRepeat(bool is_autorepeat);

  void SetIsLooping(bool is_looping);

  void SetIsShuffling(bool is_shuffling);

  void SetIsPlayingEventHandler(std::function<void(bool)> is_playing_callback);

  void SetIsBufferingEventHandler(
      std::function<void(bool)> is_buffering_callback);

  void SetIsCompletedEventHandler(
      std::function<void(bool)> is_completed_callback);

  void SetPositionEventHandler(std::function<void(int32_t)> position_callback);

  void SetDurationEventHandler(std::function<void(int32_t)> duration_callback);

  void SetVolumeEventHandler(std::function<void(float)> volume_callback);

  void SetRateEventHandler(std::function<void(float)> rate_callback);

  void SetIndexEventHandler(std::function<void(float)> index_callback);

  void SetDownloadProgressEventHandler(
      std::function<void(float)> download_progress_callback);

  void SetErrorEventHandler(std::function<void(std::string)> error_callback);

  void Run();

  webview::webview* webview() const { return webview_.get(); }
  std::string& source() { return source_; }

 private:
  int32_t id_;
  int32_t index_ = 0;
  std::vector<int32_t> media_ids_ = {};
  std::vector<std::string> media_uris_ = {};
  bool is_playing_ = false;
  bool is_buffering_ = false;
  bool is_completed_ = false;
  int32_t position_ = 0;
  int32_t duration_ = 0;
  int32_t buffering_position_ = 0;
  float download_progress_ = 0.0f;
  float volume_ = 0.0f;
  float rate_ = 0.0f;
  bool is_autorepeat_ = false;
  bool is_looping_ = false;
  bool is_shuffling_ = false;
  std::function<void(bool)> is_playing_callback_ = [](bool) {};
  std::function<void(bool)> is_buffering_callback_ = [](bool) {};
  std::function<void(bool)> is_completed_callback_ = [](bool) {};
  std::function<void(int32_t)> position_callback_ = [](int32_t) {};
  std::function<void(int32_t)> duration_callback_ = [](int32_t) {};
  std::function<void(int32_t)> buffering_callback_ = [](int32_t) {};
  std::function<void(float)> volume_callback_ = [](float) {};
  std::function<void(float)> rate_callback_ = [](float) {};
  std::function<void(int32_t)> index_callback_ = [](int32_t) {};
  std::function<void(float)> download_progress_callback_ = [](float) {};
  std::function<void(std::string)> error_callback_ = [](std::string) {};
  std::string source_ =
      std::filesystem::temp_directory_path().string() + "/source.html";
  std::unique_ptr<std::thread> thread_ = nullptr;
  std::unique_ptr<webview::webview> webview_ =
      std::make_unique<webview::webview>(true, nullptr);
  std::unique_ptr<std::promise<void>> promise_ =
      std::make_unique<std::promise<void>>();
  bool initialized_ = false;

  inline void EnsureFuture();

  static constexpr auto kPlayerSource =
      "<!DOCTYPE html>"
      "<html>"
      "<body>"
      "   <video controls id='player'></video>"
      "</body>"
      "<style>"
      "   * {"
      "       background: #000;"
      "       margin: 0;"
      "       padding: 0;"
      "       overflow: hidden;"
      "   }"
      "   body {"
      "       height: 100%;"
      "       width: 100%;"
      "   }"
      "   video {"
      "       height: 100vh;"
      "       width: 100vw;"
      "   }"
      "</style>"
      "<script>"
      "   let player = document.getElementById('player');"
      "   player.addEventListener('play', (event) => {"
      "       isPlaying(true);"
      "       isCompleted(false);"
      "   });"
      "   player.addEventListener('pause', (event) => {"
      "       isPlaying(false);"
      "   });"
      "   player.addEventListener('playing', (event) => {"
      "       isBuffering(false);"
      "   });"
      "   player.addEventListener('waiting', (event) => {"
      "       isBuffering(true);"
      "   });"
      "   player.addEventListener('timeupdate', (event) => {"
      "       position(Math.round(event.target.currentTime * 1000));"
      "   });"
      "   player.addEventListener('progress', (event) => {"
      "       if (player.buffered.length > 0) {"
      "           downloadProgress(player.buffered.end(player.buffered.length "
      "- 1) / event.target.duration);"
      "       }"
      "   });"
      "   player.addEventListener('durationchange', (event) => {"
      "       duration(Math.round(event.target.duration * 1000));"
      "   });"
      "   player.addEventListener('ended', (event) => {"
      "       isPlaying(false);"
      "       isCompleted(true);"
      "   });"
      "   player.addEventListener('volumechange', (event) => {"
      "       volume(Math.round(event.target.volume * 1000) / 1000);"
      "   });"
      "   player.addEventListener('ratechange', (event) => {"
      "       rate(Math.round(event.target.playbackRate * 1000) / 1000);"
      "   });"
      "   player.addEventListener('error', (event) => {"
      "       error(event.message);"
      "   });"
      "   let button = document.createElement('button');"
      "   window.onload = () => initialized(null);"
      "</script>"
      "</html>";
};

Player::Player(int32_t id, bool show_window = false,
               std::string window_title = "libwinmedia")
    : id_(id) {
  std::fstream file(source_, std::ios::out);
  file << Player::kPlayerSource;
  file.close();
  webview_->bind("initialized", [=](std::string _) -> std::string {
    promise_->set_value();
    webview_->eval("button.click();");
    return "";
  });
  webview_->bind("isPlaying", [=](std::string event) -> std::string {
    if (event == "[null]") return "";
    try {
      is_playing_ = event.substr(1, event.size() - 2) == "true" ? true : false;
      is_playing_callback_(is_playing_);
    } catch (...) {
    }
    return "";
  });
  webview_->bind("isBuffering", [=](std::string event) -> std::string {
    if (event == "[null]") return "";
    try {
      is_buffering_ =
          event.substr(1, event.size() - 2) == "true" ? true : false;
      is_buffering_callback_(is_buffering_);
    } catch (...) {
    }
    return "";
  });
  webview_->bind("isCompleted", [=](std::string event) -> std::string {
    if (event == "[null]") return "";
    try {
      is_completed_ =
          event.substr(1, event.size() - 2) == "true" ? true : false;
      is_completed_callback_(is_completed_);
      if (is_completed_) {
        if (is_looping_)
          Play();
        else
          Next();
      };
    } catch (...) {
    }
    return "";
  });
  webview_->bind("position", [=](std::string event) -> std::string {
    if (event == "[null]") return "";
    try {
      position_ = std::stoi(event.substr(1, event.size() - 2));
      position_callback_(position_);
    } catch (...) {
    }
    return "";
  });
  webview_->bind("duration", [=](std::string event) -> std::string {
    if (event == "[null]") return "";
    try {
      duration_ = std::stoi(event.substr(1, event.size() - 2));
      duration_callback_(duration_);
    } catch (...) {
    }
    return "";
  });
  webview_->bind("downloadProgress", [=](std::string event) -> std::string {
    if (event == "[null]") return "";
    try {
      download_progress_ = std::stof(event.substr(1, event.size() - 2));
      download_progress_callback_(download_progress_);
    } catch (...) {
    }
    return "";
  });
  webview_->bind("volume", [=](std::string event) -> std::string {
    if (event == "[null]") return "";
    try {
      volume_ = std::stof(event.substr(1, event.size() - 2));
      volume_callback_(volume_);
    } catch (...) {
    }
    return "";
  });
  webview_->bind("rate", [=](std::string event) -> std::string {
    if (event == "[null]") return "";
    try {
      rate_ = std::stof(event.substr(1, event.size() - 2));
      rate_callback_(rate_);
    } catch (...) {
    }
    return "";
  });
  webview_->bind("error", [=](std::string event) -> std::string {
    try {
      error_callback_(event.substr(1, event.size() - 2));
    } catch (...) {
    }
    return "";
  });
  webview_->navigate("file://" + source_);
  webview_->set_title(window_title);
  webview_->set_size(480, 360, WEBVIEW_HINT_NONE);
  if (!show_window) {
    gtk_widget_hide(GTK_WIDGET(webview_->window()));
  }
  srand(time(0));
}

void Player::ShowWindow() {
  EnsureFuture();
  gtk_widget_show(GTK_WIDGET(webview_->window()));
}

void Player::CloseWindow() {
  EnsureFuture();
  gtk_widget_hide(GTK_WIDGET(webview_->window()));
}

void Player::Open(std::vector<std::string> uris, std::vector<int32_t> ids) {
  EnsureFuture();
  index_ = 0;
  media_ids_ = ids;
  media_uris_ = uris;
  webview_->eval(
      "player.src = '" +
      std::string(media_uris_.front().begin(), media_uris_.front().end()) +
      "';");
}

void Player::Add(std::string uri, int32_t id) {
  EnsureFuture();
  index_ = 0;
  media_ids_.emplace_back(id);
  media_uris_.emplace_back(uri);
}

void Player::Play() {
  EnsureFuture();
  webview_->eval("player.play();");
}

void Player::Pause() {
  EnsureFuture();
  webview_->eval("player.pause();");
}

void Player::Stop() {
  EnsureFuture();
  webview_->terminate();
}

void Player::Next() {
  EnsureFuture();
  if (index_ >= media_uris_.size() - 1) {
    if (is_autorepeat_)
      index_ = -1;
    else
      return;
  }
  if (!is_shuffling_) {
    Jump(++index_);
  } else {
    Jump(rand() % media_uris_.size());
  }
}

void Player::Back() {
  EnsureFuture();
  if (index_ <= 0) {
    if (is_autorepeat_)
      index_ = media_uris_.size();
    else
      return;
  }
  if (!is_shuffling_) {
    Jump(--index_);
  } else {
    Jump(rand() % media_uris_.size());
  }
}

void Player::Jump(int32_t index) {
  EnsureFuture();
  Pause();
  index_ = index;
  webview_->eval(
      "player.src = '" +
      std::string(media_uris_[index].begin(), media_uris_[index].end()) + "';");
  Play();
  index_callback_(index_);
}

void Player::Seek(int32_t position) {
  EnsureFuture();
  webview_->eval("player.currentTime = " + std::to_string(position) +
                 " / 1000.0;");
}

void Player::SetVolume(float volume) {
  EnsureFuture();
  webview_->eval("player.volume = " + std::to_string(volume) + ";");
}

void Player::SetRate(float rate) {
  EnsureFuture();
  webview_->eval("player.playbackRate = " + std::to_string(rate) + ";");
}

void Player::SetIsAutoRepeat(bool is_autorepeat) {
  EnsureFuture();
  is_autorepeat_ = is_autorepeat;
};

void Player::SetIsLooping(bool is_looping) {
  EnsureFuture();
  is_looping_ = is_looping;
};

void Player::SetIsShuffling(bool is_shuffling) {
  EnsureFuture();
  is_shuffling_ = is_shuffling;
};

void Player::SetIsPlayingEventHandler(
    std::function<void(bool)> is_playing_callback) {
  is_playing_callback_ = is_playing_callback;
}

void Player::SetIsBufferingEventHandler(
    std::function<void(bool)> is_buffering_callback) {
  is_buffering_callback_ = is_buffering_callback;
}

void Player::SetIsCompletedEventHandler(
    std::function<void(bool)> is_completed_callback) {
  is_completed_callback_ = is_completed_callback;
}

void Player::SetPositionEventHandler(
    std::function<void(int32_t)> position_callback) {
  position_callback_ = position_callback;
}

void Player::SetDurationEventHandler(
    std::function<void(int32_t)> duration_callback) {
  duration_callback_ = duration_callback;
}

void Player::SetVolumeEventHandler(std::function<void(float)> volume_callback) {
  volume_callback_ = volume_callback;
}

void Player::SetRateEventHandler(std::function<void(float)> rate_callback) {
  rate_callback_ = rate_callback;
}

void Player::SetIndexEventHandler(std::function<void(float)> index_callback) {
  index_callback_ = index_callback;
}

void Player::SetDownloadProgressEventHandler(
    std::function<void(float)> download_progress_callback) {
  download_progress_callback_ = download_progress_callback;
};

void Player::SetErrorEventHandler(
    std::function<void(std::string)> error_callback) {
  error_callback_ = error_callback;
}

void Player::Run() { webview_->run(); }

void Player::EnsureFuture() {
  if (!initialized_) {
    promise_->get_future().wait();
    initialized_ = true;
  }
}

#endif
