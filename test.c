#include "http_parser.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void try_parse(int type, int expect, char *data) {
  int len = strlen(data);
  char buffer[len + 1];
  memset(buffer, 0, len + 1);
  memcpy(buffer, data, len);
  struct http_message message;
  int result = http_parser(buffer, &message, type);
  if (expect != result) {
    fprintf(stderr, "Expected %d, got %d for message:\n%s\n", expect, result, data);
  }
}

int main(void) {
  try_parse(http_request, 400, "test");
  try_parse(http_request, 400, "GE");
  try_parse(http_request, 400, "GET");
  try_parse(http_request, 414, "GET ");
  try_parse(http_request, 414, "GET /");
  try_parse(http_request, 400, "GET / ");
  try_parse(http_request, 413, "GET / \r\n");
  try_parse(http_request, 413, "GET / \n");
  try_parse(http_request, 400, "GET / HTTP/");
  try_parse(http_request, 400, "GET / HTTP/1.1");
  try_parse(http_request, 413, "GET / HTTP/1.1\r\n");
  try_parse(http_request, 413, "GET / HTTP/1.1\r\nHost: test.com");
  try_parse(http_request, 413, "GET / HTTP/1.1\r\nHost: test.com\r\n");
  try_parse(http_request, 0, "GET / HTTP/1.1\r\nHost: test.com\r\n\r\n");
  try_parse(http_request, 0, "GET / HTTP/1.1\r\nHost: test.com\r\nHeader:\t \r    \n     \t    Value\r\n\r\n");

  try_parse(http_response, 400, "test");
  try_parse(http_response, 400, "HT");
  try_parse(http_response, 400, "HTTP/1.1");
  try_parse(http_response, 414, "HTTP/1.1 ");
  try_parse(http_response, 414, "HTTP/1.1 \n");
  try_parse(http_response, 414, "HTTP/1.1 \r\n");
  try_parse(http_response, 414, "HTTP/1.1 200");
  try_parse(http_response, 0, "HTTP/1.1 200 OK\r\n\r\n");

  return 0;
}
