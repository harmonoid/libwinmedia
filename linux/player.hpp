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

  void Open(std::string uri);

  void Play();

  void Run();

  webview::webview* webview() const { return webview_.get(); }
  std::string& source() { return source_; }

 private:
  int32_t id_;
  std::string source_ =
      std::filesystem::temp_directory_path().string() + "/source.html";
  std::unique_ptr<std::thread> thread_ = nullptr;
  std::unique_ptr<webview::webview> webview_ =
      std::make_unique<webview::webview>(true, nullptr);
  std::unique_ptr<std::promise<void>> promise_ =
      std::make_unique<std::promise<void>>();
  bool initialized_ = false;

  inline void LockGuard() {
    if (!initialized_) {
      promise_->get_future().wait();
      initialized_ = true;
    }
  }

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
  webview_->navigate("file://" + source_);
  webview_->set_title(ConvertToString(window_title));
  webview_->set_size(480, 360, WEBVIEW_HINT_NONE);
  if (!show_window) {
    gtk_widget_hide(GTK_WIDGET(webview_->window()));
  }
}

void Player::Open(std::string uri) {
  LockGuard();
  webview_->eval("player.src = encodeURI('" + uri + "');");
}

void Player::Play() {
  LockGuard();
  webview_->eval("player.play();");
}

void Player::Run() { webview_->run(); }

#endif
