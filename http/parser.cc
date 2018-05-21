#include "http/parser.h"
#include "common/logger.h"

#include <string>

namespace http {

Parser::Parser()
    : status_(server::PacketHandler::Status::ERROR),
      packet_(new Packet()),
      parser_(new http_parser()) {
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

server::PacketHandler::Status Parser::Parse(
    const boost::asio::const_buffer& buffer) {
  status_ = server::PacketHandler::Status::PART_RECEIVED;

  int nparsed = http_parser_execute(
      parser_.get(), &setting_,
      reinterpret_cast<const char*>(buffer.data()), buffer.size());

  if (buffer.size() != nparsed) { //error
    LOG(LogLevel::WARN) << "Size: " << buffer.size();
    LOG(LogLevel::WARN) << "Parsing error code:"
                        << nparsed << " msg: "
                        << GetErrString(parser_->http_errno);
    status_ = server::PacketHandler::Status::ERROR;
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
  LOG(LogLevel::DEBUG) << "BEGIN";
  return 0;
}

int Parser::HandleUrl(http_parser* c_parser,
                      const char* at, size_t length) {
  LOG(LogLevel::DEBUG) << "URL: " << std::string(at, length);
  return 0;
}

int Parser::HandleStatus(http_parser* c_parser,
                         const char* at, size_t length) {
  LOG(LogLevel::DEBUG) << "Status: " << std::string(at, length);
  return 0;
}

int Parser::HandleHeaderField(http_parser* c_parser,
                              const char* at, size_t length) {
  LOG(LogLevel::DEBUG) << "Field: " << std::string(at, length);
  return 0;
}

int Parser::HandleHeaderValue(http_parser* c_parser,
                              const char* at, size_t length) {
  LOG(LogLevel::DEBUG) << "Value: " << std::string(at, length);
  return 0;
}

int Parser::HandleHeaderComplete(http_parser* c_parser) {
  LOG(LogLevel::DEBUG) << "HEAD COMPLETE";
  return 0;
}

int Parser::HandleBody(http_parser* c_parser,
                       const char* at, size_t length) {
  LOG(LogLevel::DEBUG) << "BODY: " << std::string(at, length);
  return 0;
}

int Parser::HandleMsgComplete(http_parser* c_parser) {
  LOG(LogLevel::DEBUG) << "MSG COMPLETE";
  //common fields here
  status_ = server::PacketHandler::Status::OK;

  return 0;
}

int Parser::HandleHeaderChunk(http_parser* c_parser) {
  LOG(LogLevel::DEBUG) << "HEAD CHUNK";

  status_ = server::PacketHandler::Status::PART_RECEIVED;

  return 0;
}

const char* Parser::GetErrString(unsigned int error) {
  const char* str = "INVALID_ERROR";

#define HTTP_ERRNO_STR_GEN(n, s) case HPE_##n: { str=s; break;}
  switch (error) {
    HTTP_ERRNO_MAP(HTTP_ERRNO_STR_GEN)
  }
#undef HTTP_ERRNO_STR_GEN

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
