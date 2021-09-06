#define UNICODE
#include <cwchar>
#include <filesystem>
#include <fstream>
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

  static constexpr auto kPlayerSource =
      "<!DOCTYPE html>"
      "<html>"
      "<video controls id='player'></video>"
      "<style>"
      "   * {"
      "       background: #000;"
      "       margin: 0;"
      "       padding: 0;"
      "   }"
      "   video {"
      "       height: 100%;"
      "       width: 100%;"
      "   }"
      "</style>"
      "<script>"
      "   let player = document.getElementById('player');"
      "   let button = document.createElement('button');"
      "</script>"
      "</html>";
};

Player::Player(int32_t id, bool show_window = false,
               std::wstring window_title = L"libwinmedia")
    : id_(id) {
  std::fstream file(source_, std::ios::out);
  file << Player::kPlayerSource;
  file.close();
  webview_->navigate("file://" + source_);
  webview_->set_title(ConvertToString(window_title));
  if (!show_window) {
    gtk_widget_hide(GTK_WIDGET(webview_->window()));
  }
}

void Player::Open(std::string uri) {
  webview_->eval("player.src = encodeURI('" + uri + "');");
}

void Player::Play() { webview_->eval("player.play();"); }

void Player::Run() { webview_->run(); }

#endif
