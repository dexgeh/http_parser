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
  try_parse(HTTP_REQUEST, 400, "test");
  try_parse(HTTP_REQUEST, 400, "GE");
  try_parse(HTTP_REQUEST, 400, "GET");
  try_parse(HTTP_REQUEST, 414, "GET ");
  try_parse(HTTP_REQUEST, 414, "GET /");
  try_parse(HTTP_REQUEST, 400, "GET / ");
  try_parse(HTTP_REQUEST, 413, "GET / \r\n");
  try_parse(HTTP_REQUEST, 413, "GET / \n");
  try_parse(HTTP_REQUEST, 400, "GET / HTTP/");
  try_parse(HTTP_REQUEST, 400, "GET / HTTP/1.1");
  try_parse(HTTP_REQUEST, 413, "GET / HTTP/1.1\r\n");
  try_parse(HTTP_REQUEST, 413, "GET / HTTP/1.1\r\nHost: test.com");
  try_parse(HTTP_REQUEST, 413, "GET / HTTP/1.1\r\nHost: test.com\r\n");
  try_parse(HTTP_REQUEST, 0, "GET / HTTP/1.1\r\nHost: test.com\r\n\r\n");
  try_parse(HTTP_REQUEST, 0, "GET / HTTP/1.1\r\nHost: test.com\r\nHeader:\t \r    \n     \t    Value\r\n\r\n");

  try_parse(HTTP_RESPONSE, 400, "test");
  try_parse(HTTP_RESPONSE, 400, "HT");
  try_parse(HTTP_RESPONSE, 400, "HTTP/1.1");
  try_parse(HTTP_RESPONSE, 414, "HTTP/1.1 ");
  try_parse(HTTP_RESPONSE, 414, "HTTP/1.1 \n");
  try_parse(HTTP_RESPONSE, 414, "HTTP/1.1 \r\n");
  try_parse(HTTP_RESPONSE, 414, "HTTP/1.1 200");
  try_parse(HTTP_RESPONSE, 0, "HTTP/1.1 200 OK\r\n\r\n");

  return 0;
}
