#include "hello.pb-c.h"
#include <protobuf2json.h>
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
  printf("deserialize: %d, %s\n", req->value, req->msg);
  hello__hello_request__free_unpacked(req, NULL);
  free(buf);
}

#define TEST_JSON_FLAGS (JSON_INDENT(2) | JSON_PRESERVE_ORDER)

void with_json() {
  int ret;
  Hello__HelloRequest req, *req2;
  // size_t len;
  // uint8_t *buf;
  char *json_str;

  hello__hello_request__init(&req);
  req.value = 123;
  req.msg = "Hello";

  ret = protobuf2json_string(&req.base, TEST_JSON_FLAGS, &json_str, NULL, 0);
  assert(ret == 0);
  printf("to JSON: %s\n", json_str);

  ProtobufCMessage *msg = NULL;
  char error_string[256] = {0};
  ret = json2protobuf_string(json_str, 0, &hello__hello_request__descriptor, &msg, error_string,
                             sizeof(error_string));
  assert(ret == 0);
  req2 = (Hello__HelloRequest *)msg;
  printf("from JSON: %d, %s\n", req2->value, req2->msg);

  free(json_str);
  free(req2);
}

int main(int argc, char **argv) {
  uint8_t *buf;
  size_t buf_len;

  buf = serialize(&buf_len);
  deserialize(buf, buf_len);

  with_json();

  return 0;
}
