#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <ctime>

#include "common/logger.h"

#include "app_ctrl.h"
#include "com_ctrl.h"

ApplicationController::ApplicationController(CommunicationController& com_ctrl)
    : com_ctrl_(com_ctrl),
      notification_(),
      notification_received_(false),
      device_info_(),
      device_ready_(false),
      loop_session_(nullptr),
      current_command_(new EmptyCommand()) {
}

ApplicationController::~ApplicationController() {
}

void ApplicationController::HandleUdpData(
    const boost::asio::const_buffer& buffer) {
  if (!notification_received_) {
    std::stringstream stream(
        std::string(reinterpret_cast<const char*>(buffer.data()),
            buffer.size()));
    boost::property_tree::ptree tree;
    boost::property_tree::read_json(stream, tree);

    if (ParseDeviceNotification(tree)) {
      notification_received_ = true;
      SendGetConfigRequest();
    } else {
      DLOG(INFO) << "UDP Data: " << stream.str();
    }
  }
}

void ApplicationController::NotifyUdpDataSend() {
}

void ApplicationController::HandleServicePacket(
    anet::net::SessionPtr session, const anet::http::Packet& packet) {
  anet::net::Session::BufferPtr data = packet.Serialize();
  VLOG(2) << "Servcie Data: "
      << std::string(reinterpret_cast<const char*>(&data->at(0)),
                     data->size());

  if (anet::http::Packet::Status::OK == packet.GetStatus()) {
    LOG(INFO) << "Device info received";
    device_ready_ = true;
  }
}

void ApplicationController::NotifyServiceDataSend(
    anet::net::SessionPtr session) {
}

void ApplicationController::NotifyServiceSessionClose(
    anet::net::SessionPtr session) {
}

void ApplicationController::HandleLoopPacket(
    anet::net::SessionPtr session, const anet::http::Packet& packet) {
  loop_session_ = session;

  anet::net::Session::BufferPtr data = packet.Serialize();
  VLOG(2) << "Servcie Data: "
      << std::string(reinterpret_cast<const char*>(&data->at(0)),
                     data->size());
  if ((anet::http::Packet::Type::REQUEST == packet.GetType())
      && anet::http::Packet::Method::GET == packet.GetMethod()) {
    anet::http::Url url(packet.GetUrl());

    DLOG(INFO) << "Path: " << url.path;
    if ("/videoList.xml" == url.path) {
      HandleVideoListRequest(url);
    }
  }
}

void ApplicationController::NotifyLoopDataSend(
    anet::net::SessionPtr session) {
}

void ApplicationController::NotifyLoopSessionClose(
    anet::net::SessionPtr session) {
  if (session == loop_session_) {
    loop_session_.reset();
  }
}

void ApplicationController::PlayVideo(const std::string& url) {
  LOG(INFO) << "Request Play video. URL: " << url;

  current_command_.reset(new PlayCommand(url));
}

void ApplicationController::ResetCommand() {
  LOG(INFO) << "Reset command";

  current_command_.reset(new EmptyCommand());
}

void ApplicationController::SendGetConfigRequest() {
  anet::tcp::SessionPtr session = com_ctrl_.CreateHttpClientSession(
          boost::asio::ip::address::from_string(notification_.v2_url.host));
  if (nullptr == session) {
    DLOG(ERROR) << "SESSION NOT CREATED";
  } else {
    DLOG(INFO) << __PRETTY_FUNCTION__;
    anet::http::Packet packet;
    packet.Init(1,1, anet::http::Packet::Status::INIT,
        anet::http::Packet::Method::GET, anet::http::Packet::Type::REQUEST);
    packet.SetUrl(notification_.v2_url.path); //parse url
    packet.AddHeaderField("Host",
        notification_.v2_url.host
        + ((0 != notification_.v2_url.port) ?
        (std::string(":") + std::to_string(notification_.v2_url.port)) : ""));
    packet.AddHeaderField("User-Agent", "Dalvik/2.1.0 (Linux;)");
    packet.AddHeaderField("Accept-Encoding", "gzip, deflate");
    packet.AddHeaderField("Connection", "keep-alive");
    packet.AddHeaderField("Accept", "*/*");
    packet.AddHeaderField("Content-Type", "application/json");

    anet::net::Session::BufferPtr buffer = packet.Serialize();
    if (nullptr != buffer) {
      DLOG(INFO) << "Send GetDeviceInfo request";
      VLOG(2)
          << std::string(reinterpret_cast<char*>(&buffer->at(0)),
                         buffer->size());
      session->Write(buffer);
      session->Read();
    }
  }
}

void ApplicationController::SendSwitchRequest() {
  anet::tcp::SessionPtr session = com_ctrl_.CreateHttpClientSession(
          boost::asio::ip::address::from_string(notification_.v2_url.host));
  if (nullptr == session) {
    DLOG(ERROR) << "SESSION NOT CREATED";
  } else {
    DLOG(INFO) << __PRETTY_FUNCTION__;
    anet::http::Packet packet;
    packet.Init(1,1, anet::http::Packet::Status::INIT,
        anet::http::Packet::Method::GET, anet::http::Packet::Type::REQUEST);

    packet.SetUrl(notification_.v2_url.path
        + "channels/de.2kit.CastBrowser"); //parse url

    packet.AddHeaderField("Host",
        notification_.v2_url.host
        + ((0 != notification_.v2_url.port) ?
        (std::string(":") + std::to_string(notification_.v2_url.port)) : ""));

    packet.AddHeaderField("User-Agent", "Dalvik/2.1.0 (Linux;)");
    packet.AddHeaderField("Accept-Encoding", "gzip, deflate");
    packet.AddHeaderField("Connection", "Upgrade");
    packet.AddHeaderField("Accept", "*/*");
    packet.AddHeaderField("Sec-WebSocket-Version", "13");
    packet.AddHeaderField("Sec-WebSocket-Key", "+hhAR9DmQr6o/rbZSQQkpQ==");
    packet.AddHeaderField("Sec-WebSocket-Extensions", "x-webkit-deflate-frame");
    packet.AddHeaderField("Upgrade", "websocket");
    packet.AddHeaderField("Pragma", "no-cache");
    packet.AddHeaderField("Cache-Control", "no-cache");

    anet::net::Session::BufferPtr buffer = packet.Serialize();
    if (nullptr != buffer) {
      DLOG(INFO) << "Send Switch request";
      VLOG(2)
          << std::string(reinterpret_cast<char*>(&buffer->at(0)),
                         buffer->size());
      session->Write(buffer);
      session->Read();
    }
  }
}

void ApplicationController::HandleVideoListRequest(
    const anet::http::Url& url) {
  DeviceInfo new_device_info;

  UpdateDeviceInfo(new_device_info.stopped, "stopped", url);
  UpdateDeviceInfo(new_device_info.paused, "paused", url);
  UpdateDeviceInfo(new_device_info.buffering, "buffering", url);
  UpdateDeviceInfo(new_device_info.position, "position", url);
  UpdateDeviceInfo(new_device_info.duration, "duration", url);
  UpdateDeviceInfo(new_device_info.timestamp, "timestamp", url);
  UpdateDeviceInfo(new_device_info.platform, "platform", url);
  UpdateDeviceInfo(new_device_info.year, "year", url);
  UpdateDeviceInfo(new_device_info.version, "version", url);
  UpdateDeviceInfo(new_device_info.firmware, "firmware", url);
  UpdateDeviceInfo(new_device_info.model_id, "modelid", url);
  UpdateDeviceInfo(new_device_info.real_model, "realmodel", url);

  if ((device_info_.stopped != new_device_info.stopped)
      || (device_info_.paused != new_device_info.paused)
      || (device_info_.buffering != new_device_info.buffering)) {
    LOG(INFO) << "Device State: {S:"
        << std::to_string(new_device_info.stopped)
        << ", P:" << std::to_string(new_device_info.paused)
        << ", B:" << std::to_string(new_device_info.buffering)
        << "}";
  }
  device_info_ = new_device_info;

  Control();
  ResponseVideoList();
}

void ApplicationController::ResponseVideoList() {
  if (nullptr == loop_session_) {
    DLOG(ERROR) << "SESSION IS CLOSED";
  } else {
    DLOG(INFO) << __PRETTY_FUNCTION__;

    anet::http::Packet packet;
    packet.Init(1,1, anet::http::Packet::Status::OK,
        anet::http::Packet::Method::INIT, anet::http::Packet::Type::RESPONSE);
    packet.AddHeaderField("Connection", "keep-alive");
    packet.AddHeaderField("Access-Control-Allow-Origin", "*");

    anet::http::Packet::BufferPtr body = CreateRssPacket();
    packet.AddHeaderField("Content-Length", std::to_string(body->size()));
    packet.AssignBody(boost::move(body));

    anet::net::Session::BufferPtr new_buffer = packet.Serialize();
    if (nullptr != new_buffer) {
      VLOG(2)
          << std::string(reinterpret_cast<char*>(&new_buffer->at(0)),
                         new_buffer->size());
      loop_session_->Write(new_buffer);
    }
  }
}

bool ApplicationController::ParseDeviceNotification(
    boost::property_tree::ptree& tree) {
  bool retval = false;

  do {
    std::string type_str = tree.get<std::string>("type", std::string());
    if (type_str.empty()) {
      DLOG(ERROR) << "Type field is empty";
      break;
    }

    DeviceNotification::Type type =
        DeviceNotification::ConvertToType(type_str);
    if (DeviceNotification::Type::INIT == type) {
      DLOG(ERROR) << "Invalid device type: " << type_str;
      break;
    }

    if (DeviceNotification::Type::ALIVE != type) {
      DLOG(INFO) << "Not supported device type: " << type_str;
      break;
    }

    notification_.type = type;

    std::string v2 = tree.get<std::string>("data.v2.uri", std::string());
    if (v2.empty()) {
      DLOG(ERROR) << "No V2 API URL found";
      break;
    } else {
      DLOG(INFO) << "v2: " << v2;
      anet::http::Url::Parse(v2, notification_.v2_url);
    }

    std::string v1 = tree.get<std::string>("data.v1.uri", std::string());
    if (!v1.empty()) {
      DLOG(INFO) << "v1: " << v1;
      anet::http::Url::Parse(v1, notification_.v1_url);
    }

    uint32_t ttl = tree.get<uint32_t>("ttl", 0);
    if (0 == ttl) {
      DLOG(WARNING) << "empty ttl";
    } else {
      DLOG(INFO) << "ttl: " << ttl;
      notification_.ttl = ttl;
    }

    std::string remote = tree.get<std::string>("remote", std::string());
    if (remote.empty()) {
      DLOG(WARNING) << "empty remote";
    } else {
      DLOG(INFO) << "remote: " << remote;
      notification_.remote = remote;
    }

    std::string sid = tree.get<std::string>("sid", std::string());
    if (sid.empty()) {
      DLOG(WARNING) << "empty sid";
    } else {
      DLOG(INFO) << "sid: " << sid;
      notification_.sid = sid;
    }

    retval = true;
  } while (false);

  return retval;
}

anet::http::Packet::BufferPtr ApplicationController::CreateRssPacket() {
  //Create XML
  boost::property_tree::ptree tree;
  tree.put("rss.<xmlattr>.version", "2.0");
  tree.put("rss.channel.item.commandTimestamp",
           std::to_string(std::time(nullptr)));
  current_command_->UpdateTree(tree);
  current_command_->send = true;

  std::stringstream output_stream;
  boost::property_tree::write_xml(output_stream, tree);
  std::string buffer = output_stream.str();

  anet::http::Packet::BufferPtr packet(
      new anet::http::Packet::Buffer(buffer.begin(), buffer.end()));
  return packet;
}

void ApplicationController::Control() {
}

void ApplicationController::UpdateDeviceInfo(
    uint32_t& value, const std::string& key, const anet::http::Url& url) {
  const std::string value_str = url.GetQueryParameter(key);
  if (value_str.empty()) {
    DLOG(WARNING)
        << "Device Info not found: {Key: " << key << "}";
  } else {
    DLOG(INFO)
        << "Device Info: {Key: " << key
        << ", Value: " << value_str << "}";
    value = ToBool(value_str);
  }
}

void ApplicationController::UpdateDeviceInfo(
    bool& value, const std::string& key, const anet::http::Url& url) {
  const std::string value_str = url.GetQueryParameter(key);
  if (value_str.empty()) {
    DLOG(WARNING)
        << "Device Info not found: {Key: " << key << "}";
  } else {
    DLOG(INFO)
        << "Device Info: {Key: " << key
        << ", Value: " << value_str << "}";
    value = ToBool(value_str);
  }
}

void ApplicationController::UpdateDeviceInfo(
    std::string& value, const std::string& key,
    const anet::http::Url& url) {
  std::string value_str = url.GetQueryParameter(key);
  if (value_str.empty()) {
    DLOG(WARNING)
        << "Device Info not found: {Key: " << key << "}";
  } else {
    DLOG(INFO)
        << "Device Info: {Key: " << key
        << ", Value: " << value_str << "}";
    value = value_str;
  }
}

DeviceNotification::Type DeviceNotification::ConvertToType(
    const std::string& str) {
  Type retval = Type::INIT;

  if ("discover" == str) {
    retval = Type::DISCOVER;
  } else if ("alive" == str) {
    retval = Type::ALIVE;
  }

  return retval;
}

void EmptyCommand::UpdateTree(boost::property_tree::ptree& tree) {
  tree.put("rss.channel.item.command", "EMPTY");
}

PlayCommand::PlayCommand(const std::string& url)
    : url_(url),
      timestamp_(std::time(nullptr)) {
}

void PlayCommand::UpdateTree(boost::property_tree::ptree& tree) {
  std::string description = "Playing: " + url_;

  tree.put("rss.channel.item.title", url_);
  tree.put("rss.channel.item.link", url_);
  tree.put("rss.channel.item.description", description);
  tree.put("rss.channel.item.timestamp", std::to_string(timestamp_));
  tree.put("rss.channel.item.command", "PLAY");
}
