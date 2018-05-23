#include "http/parser.h"
#include "common/logger.h"

namespace http {

Parser::Parser()
    : status_(net::PacketHandler::Status::ERROR),
      packet_(new Packet()),
      setting_(),
      parser_(new http_parser()),
      current_field_(),
      body_(nullptr),
      body_index_(0),
      content_length_(0) {
  LOG(LogLevel::DEBUG) << __PRETTY_FUNCTION__;

  memset(&setting_, 0, sizeof(setting_));

  setting_.on_message_begin = &Parser::SHandleMsgBegin;
  setting_.on_url = &Parser::SHandleUrl;
  setting_.on_status = &Parser::SHandleStatus;
  setting_.on_header_field = &Parser::SHandleHeaderField;
  setting_.on_header_value = &Parser::SHandleHeaderValue;
  setting_.on_headers_complete = &Parser::SHandleHeaderComplete;
  setting_.on_body = &Parser::SHandleBody;
  setting_.on_message_complete = &Parser::SHandleMsgComplete;
  setting_.on_chunk_header = &Parser::SHandleHeaderChunk;

  http_parser_init(parser_.get(), HTTP_BOTH);
  parser_->data = this;
}

Parser::~Parser() {
  LOG(LogLevel::DEBUG) << __PRETTY_FUNCTION__;
}

net::PacketHandler::Status Parser::Parse(
    const boost::asio::const_buffer& buffer) {
  status_ = net::PacketHandler::Status::PART_RECEIVED;

  int nparsed = http_parser_execute(
      parser_.get(), &setting_,
      reinterpret_cast<const char*>(buffer.data()), buffer.size());

  if (buffer.size() != nparsed) { //error
    LOG(LogLevel::WARN) << "Size: " << buffer.size();
    LOG(LogLevel::WARN) << "Parsing error code:"
                        << nparsed << " msg: "
                        << GetErrString(parser_->http_errno);
    status_ = net::PacketHandler::Status::ERROR;
  }

  return status_;
}

int Parser::SHandleMsgBegin(http_parser* c_parser) {
  return SHandle(&Parser::HandleMsgBegin, c_parser);
}

int Parser::SHandleUrl(http_parser* c_parser,
                             const char* at, size_t length) {
  return SHandleData(&Parser::HandleUrl, c_parser, at, length);
}

int Parser::SHandleStatus(http_parser* c_parser,
                          const char* at, size_t length) {
  return SHandleData(&Parser::HandleStatus, c_parser, at, length);
}

int Parser::SHandleHeaderField(http_parser* c_parser,
                               const char* at, size_t length) {
  return SHandleData(&Parser::HandleHeaderField, c_parser, at, length);
}

int Parser::SHandleHeaderValue(http_parser* c_parser,
                               const char* at, size_t length) {
  return SHandleData(&Parser::HandleHeaderValue, c_parser, at, length);
}

int Parser::SHandleHeaderComplete(http_parser* c_parser) {
  return SHandle(&Parser::HandleHeaderComplete, c_parser);
}

int Parser::SHandleBody(http_parser* c_parser,
                        const char* at, size_t length) {
  return SHandleData(&Parser::HandleBody, c_parser, at, length);
}

int Parser::SHandleMsgComplete(http_parser* c_parser) {
  return SHandle(&Parser::HandleMsgComplete, c_parser);
}

int Parser::SHandleHeaderChunk(http_parser* c_parser) {
  return SHandle(&Parser::HandleHeaderChunk, c_parser);
}

int Parser::HandleMsgBegin(http_parser* c_parser) {
  LOG(LogLevel::DEBUG) << __PRETTY_FUNCTION__;
  return 0;
}

int Parser::HandleUrl(http_parser* c_parser,
                      const char* at, size_t length) {
  LOG(LogLevel::DEBUG) << __PRETTY_FUNCTION__;
  int retval = -1;

  if (nullptr != at) {
    std::string url(at, length);
    LOG(LogLevel::DEBUG)
        << "URL(" << url << ")";

    packet_->SetUrl(url);
    retval = 0;
  }

  return retval;
  return 0;
}

int Parser::HandleStatus(http_parser* c_parser,
                         const char* at, size_t length) {
  LOG(LogLevel::DEBUG) << "Status: " << std::string(at, length);
  return 0;
}

int Parser::HandleHeaderField(http_parser* c_parser,
                              const char* at, size_t length) {
  LOG(LogLevel::DEBUG) << __PRETTY_FUNCTION__;
  int retval = -1;

  if (nullptr != at) {
    current_field_ = std::string(at, length);
    LOG(LogLevel::DEBUG) << "Field: " << current_field_;
    retval = 0;
  }

  return retval;
}

int Parser::HandleHeaderValue(http_parser* c_parser,
                              const char* at, size_t length) {
  LOG(LogLevel::DEBUG) << __PRETTY_FUNCTION__;

  int retval = -1;
  if (nullptr != at) {
    std::string value(at, length);
    LOG(LogLevel::DEBUG) << "Value: " << value;

    LOG(LogLevel::INFO)
        << "Header field. {Key:(" << current_field_ << ") "
           "Value:(" << value << ")";

    packet_->AddHeaderField(current_field_, value);
    retval = 0;
  }

  return retval;
}

int Parser::HandleHeaderComplete(http_parser* c_parser) {
  LOG(LogLevel::DEBUG) << __PRETTY_FUNCTION__;

  int retval = -1;
  if (nullptr != c_parser) {
    LOG(LogLevel::INFO)
        << "HTTP(" << c_parser->http_major
        << "." << c_parser->http_minor << ") "
        << "status(" << c_parser->status_code << " "
        << GetStatusString(c_parser->status_code) << ") "
        << "method(" << c_parser->method << " "
        << GetMethodString(c_parser->method) << ")";

    packet_->Init(c_parser->http_major, c_parser->http_minor,
        static_cast<Packet::Status>(c_parser->status_code),
        static_cast<Packet::Method>(c_parser->method),
        ((0 == c_parser->status_code) ?
            Packet::Type::REQUEST : Packet::Type::REQUEST));

    LOG(LogLevel::INFO) << "Content length:" << c_parser->content_length;
    content_length_ = c_parser->content_length;

    retval = 0;
  }

  return retval;
}

int Parser::HandleBody(http_parser* c_parser,
                       const char* at, size_t length) {
  LOG(LogLevel::DEBUG) << __PRETTY_FUNCTION__ << "(" << length << ")";

  int retval = -1;
  do {
    if (nullptr == c_parser) {
      break;
    }

    if (nullptr == at) {
      break;
    }

    if (nullptr == body_) {
      if ((0 < content_length_) && (Packet::MAX_BODY_LENGTH >= content_length_)) {
        LOG(LogLevel::DEBUG) << "Allocate http body buffer. Size:"
                             << content_length_;
        body_.reset(new Packet::Buffer(content_length_));
      }
    }

    if (nullptr == body_) {
      break;
    }

    if ((0 < length) && ((content_length_ - body_index_) >= length)) {
      LOG(LogLevel::DEBUG)
          << "Copy http body data at index(" << body_index_ << ") "
          << "length(" << length << ")";

      memcpy(&(body_->at(body_index_)), at, length);
      body_index_ += length;
    } else {
      break;
    }

    retval = 0;
  } while (false);

  return retval;
}

int Parser::HandleMsgComplete(http_parser* c_parser) {
  LOG(LogLevel::DEBUG) << __PRETTY_FUNCTION__;

  if (nullptr != body_) {
    LOG(LogLevel::DEBUG) << "HTTP Body: "
        << std::string(reinterpret_cast<char*>(&body_->at(0)), body_->size());
    packet_->AssignBody(boost::move(body_));
  }

  status_ = net::PacketHandler::Status::OK;

  return 0;
}

int Parser::HandleHeaderChunk(http_parser* c_parser) {
  LOG(LogLevel::DEBUG) << __PRETTY_FUNCTION__;

  status_ = net::PacketHandler::Status::PART_RECEIVED;

  return 0;
}

const char* Parser::GetErrString(unsigned int error) {
  const char* str = "INVALID_ERROR";

#define HTTP_ERRNO_STR_GEN(n, s) case HPE_##n: { str=s; break; }
  switch (error) {
    HTTP_ERRNO_MAP(HTTP_ERRNO_STR_GEN)
  }
#undef HTTP_ERRNO_STR_GEN

  return str;
}

const char* Parser::GetStatusString(unsigned int status) {
  const char* str = "INVALID_STATUS";

#define XX(num, name, string) case HTTP_STATUS_##name: { str=#string; break; }
  switch (status) {
    HTTP_STATUS_MAP(XX)
  }
#undef XX

  return str;
}

const char*Parser::GetMethodString(unsigned int method) {
  const char* str = "INVALID_METHOD";

#define XX(num, name, string) case HTTP_##name: { str=#string; break; }
  switch (method) {
    HTTP_METHOD_MAP(XX)
  }
#undef XX

  return str;
}

int Parser::SHandle(Parser::cpp_http_cb func, http_parser* c_parser) {
  Parser* parser = reinterpret_cast<Parser*>(c_parser->data);
  int retval = 0;
  if (nullptr != parser) {
    retval = (parser->*func)(c_parser);
  }

  return retval;
}

int Parser::SHandleData(cpp_http_data_cb func,
                        http_parser* c_parser,
                        const char* at, size_t length) {
  Parser* parser = reinterpret_cast<Parser*>(c_parser->data);
  int retval = 0;
  if (nullptr != parser) {
    retval = (parser->*func)(c_parser, at, length);
  }

  return retval;
}

}  // namespace http
