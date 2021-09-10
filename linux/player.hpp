#define UNICODE
#include <cwchar>
#include <filesystem>
#include <fstream>
#include <future>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "../external/webview/webview.h"
#include "utils.hpp"

#ifndef PLAYER_H_
#define PLAYER_H_

class Player {
 public:
  Player(int32_t id, bool show_window, std::wstring window_title);

  bool is_playing() const { return is_playing_; }
  bool is_buffering() const { return is_buffering_; }
  bool is_completed() const { return is_completed_; }
  int32_t position() const { return position_; }
  int32_t duration() const { return duration_; }
  float volume() const { return volume_; }
  float rate() const { return rate_; }
  int32_t index() const { return index_; }
  std::vector<int32_t>& media_ids() { return media_ids_; }
  std::vector<std::wstring>& media_uris() { return media_uris_; }

  void ShowWindow();

  void CloseWindow();

  void Open(std::vector<std::wstring> uris, std::vector<int32_t> ids);

  void Play();

  void Pause();

  void Stop();

  void Next();

  void Back();

  void Jump(int32_t index);

  void Seek(int32_t position);

  void SetVolume(float volume);

  void SetRate(float rate);

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

  void Run();

  webview::webview* webview() const { return webview_.get(); }
  std::string& source() { return source_; }

 private:
  int32_t id_;
  int32_t index_ = 0;
  std::vector<int32_t> media_ids_ = {};
  std::vector<std::wstring> media_uris_ = {};
  bool is_playing_ = false;
  bool is_buffering_ = false;
  bool is_completed_ = false;
  int32_t position_ = 0;
  int32_t duration_ = 0;
  float volume_ = 0.0;
  float rate_ = 0.0;
  std::function<void(bool)> is_playing_callback_ = [](bool) {};
  std::function<void(bool)> is_buffering_callback_ = [](bool) {};
  std::function<void(bool)> is_completed_callback_ = [](bool) {};
  std::function<void(int32_t)> position_callback_ = [](int32_t) {};
  std::function<void(int32_t)> duration_callback_ = [](int32_t) {};
  std::function<void(float)> volume_callback_ = [](float) {};
  std::function<void(float)> rate_callback_ = [](float) {};
  std::function<void(int32_t)> index_callback_ = [](int32_t) {};
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
      "   player.addEventListener('durationchange', (event) => {"
      "       duration(Math.round(event.target.duration * 1000));"
      "   });"
      "   player.addEventListener('ended', (event) => {"
      "       isPlaying(false);"
      "       isCompleted(true);"
      "   });"
      "   player.addEventListener('volumechange', (event) => {"
      "       volume(event.target.volume);"
      "   });"
      "   player.addEventListener('ratechange', (event) => {"
      "       rate(event.target.rate);"
      "   });"
      "   let button = document.createElement('button');"
      "   window.onload = () => initialized(null);"
      "</script>"
      "</html>";
};

Player::Player(int32_t id, bool show_window = false,
               std::wstring window_title = L"libwinmedia")
    : id_(id) {
  std::fstream file(source_, std::ios::out);
  file << Player::kPlayerSource;
  file.close();
  webview_->bind("initialized", [=](std::string _) -> std::string {
    promise_->set_value();
    return "";
  });
  webview_->bind("isPlaying", [=](std::string event) -> std::string {
    is_playing_ = event.substr(1, event.size() - 2) == "true" ? true : false;
    is_playing_callback_(is_playing_);
    return "";
  });
  webview_->bind("isBuffering", [=](std::string event) -> std::string {
    is_buffering_ = event.substr(1, event.size() - 2) == "true" ? true : false;
    is_buffering_callback_(is_buffering_);
    return "";
  });
  webview_->bind("isCompleted", [=](std::string event) -> std::string {
    is_completed_ = event.substr(1, event.size() - 2) == "true" ? true : false;
    is_completed_callback_(is_completed_);
    if (is_completed_) Next();
    return "";
  });
  webview_->bind("position", [=](std::string event) -> std::string {
    position_ = std::stoi(event.substr(1, event.size() - 2));
    position_callback_(position_);
    return "";
  });
  webview_->bind("duration", [=](std::string event) -> std::string {
    duration_ = std::stoi(event.substr(1, event.size() - 2));
    duration_callback_(duration_);
    return "";
  });
  webview_->bind("volume", [=](std::string event) -> std::string {
    volume_ = std::stof(event.substr(1, event.size() - 2));
    volume_callback_(volume_);
    return "";
  });
  webview_->bind("rate", [=](std::string event) -> std::string {
    rate_ = std::stof(event.substr(1, event.size() - 2));
    rate_callback_(rate_);
    return "";
  });
  webview_->navigate("file://" + source_);
  webview_->set_title(ConvertToString(window_title));
  webview_->set_size(480, 360, WEBVIEW_HINT_NONE);
  if (!show_window) {
    gtk_widget_hide(GTK_WIDGET(webview_->window()));
  }
}

void Player::ShowWindow() {
  EnsureFuture();
  gtk_widget_show(GTK_WIDGET(webview_->window()));
}

void Player::CloseWindow() {
  EnsureFuture();
  gtk_widget_hide(GTK_WIDGET(webview_->window()));
}

void Player::Open(std::vector<std::wstring> uris, std::vector<int32_t> ids) {
  EnsureFuture();
  index_ = 0;
  media_ids_ = ids;
  media_uris_ = uris;
  webview_->eval(
      "player.src = encodeURI('" +
      std::string(media_uris_.front().begin(), media_uris_.front().end()) +
      "');");
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
  if (index_ >= media_uris_.size() - 1) return;
  EnsureFuture();
  Jump(++index_);
}

void Player::Back() {
  if (index_ <= 0) return;
  EnsureFuture();
  Jump(--index_);
}

void Player::Jump(int32_t index) {
  EnsureFuture();
  Pause();
  index_ = index;
  webview_->eval(
      "player.src = encodeURI('" +
      std::string(media_uris_[index].begin(), media_uris_[index].end()) +
      "');");
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

void Player::Run() { webview_->run(); }

void Player::EnsureFuture() {
  if (!initialized_) {
    promise_->get_future().wait();
    initialized_ = true;
  }
}

#endif
