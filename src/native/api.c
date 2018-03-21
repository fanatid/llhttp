#include <stdlib.h>
#include <string.h>

#include "http_parser.h"

void http_parser_set_type(http_parser_t* parser, enum http_parser_type type) {
  parser->type = type;
}


void http_parser_set_settings(http_parser_t* parser,
                              const http_parser_settings_t* settings) {
  parser->settings = (void*) settings;
}


void http_parser_settings_init(http_parser_settings_t* settings) {
  memset(settings, 0, sizeof(*settings));
}


int http_parser_finish(http_parser_t* parser) {
  int ret;

  switch (parser->finish) {
    case HTTP_FINISH_SAFE_WITH_CB:
      ret = ((http_parser_settings_t*) parser->settings)
        ->on_message_complete(parser);
      if (ret != 0) {
        return ret;
      }

    /* FALLTHROUGH */
    case HTTP_FINISH_SAFE:
      return 0;
    case HTTP_FINISH_UNSAFE:
      return HPE_INVALID_EOF_STATE;
    default:
      abort();
  }
}


/* Callbacks */


int http_parser__on_message_begin(http_parser_t* s, const char* p,
                                  const char* endp) {
  return ((http_parser_settings_t*) s->settings)->on_message_begin(s);
}


int http_parser__on_url(http_parser_t* s, const char* p, const char* endp) {
  return ((http_parser_settings_t*) s->settings)->on_url(s, p, endp - p);
}


int http_parser__on_status(http_parser_t* s, const char* p,
                           const char* endp) {
  return ((http_parser_settings_t*) s->settings)->on_status(s, p, endp - p);
}


int http_parser__on_header_field(http_parser_t* s, const char* p,
                                 const char* endp) {
  return ((http_parser_settings_t*) s->settings)->on_header_field(s, p,
      endp - p);
}


int http_parser__on_header_value(http_parser_t* s, const char* p,
                                 const char* endp) {
  return ((http_parser_settings_t*) s->settings)->on_header_value(s, p,
      endp - p);
}


int http_parser__on_headers_complete(http_parser_t* s, const char* p,
                                     const char* endp) {
  return ((http_parser_settings_t*) s->settings)->on_headers_complete(s);
}


int http_parser__on_message_complete(http_parser_t* s, const char* p,
                                     const char* endp) {
  return ((http_parser_settings_t*) s->settings)->on_message_complete(s);
}


int http_parser__on_body(http_parser_t* s, const char* p, const char* endp) {
  return ((http_parser_settings_t*) s->settings)->on_body(s, p, endp - p);
}


int http_parser__on_chunk_header(http_parser_t* s, const char* p,
                                 const char* endp) {
  return ((http_parser_settings_t*) s->settings)->on_chunk_header(s);
}


int http_parser__on_chunk_complete(http_parser_t* s, const char* p,
                                   const char* endp) {
  return ((http_parser_settings_t*) s->settings)->on_chunk_complete(s);
}