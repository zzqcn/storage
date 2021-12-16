#include "hello.pb-c.h"
#include <stdio.h>
#include <stdlib.h>

uint8_t *serialize(size_t *buf_len) {
  Hello__HelloRequest req;
  size_t len;
  uint8_t *buf;

  hello__hello_request__init(&req);
  req.value = 123;
  req.msg = "Hello";

  len = hello__hello_request__get_packed_size(&req);
  buf = malloc(len);
  hello__hello_request__pack(&req, buf);

  *buf_len = len;
  return buf;
}

void deserialize(uint8_t *buf, size_t buf_len) {
  Hello__HelloRequest *req;

  req = hello__hello_request__unpack(NULL, buf_len, buf);
  printf("req: %d, %s\n", req->value, req->msg);
  hello__hello_request__free_unpacked(req, NULL);
  free(buf);
}

int main(int argc, char **argv) {
  uint8_t *buf;
  size_t buf_len;

  buf = serialize(&buf_len);
  deserialize(buf, buf_len);

  return 0;
}
