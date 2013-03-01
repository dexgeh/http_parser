#ifndef http_parser_guard
#define http_parser_guard 1

struct http_header {
    char *name, *value;
};

struct http_message {

    #define http_request 1
    #define http_response 2
    int type;

    char *method, *url;
    char *protocol;
    char *status, *line;

    #define http_header_max 16
    struct http_header headers[http_header_max];

    char *body;

    int connection_close;
    int connection_keepalive;
    int gzip_supported;
};

int http_parser(char *, struct http_message *, int);

#endif
