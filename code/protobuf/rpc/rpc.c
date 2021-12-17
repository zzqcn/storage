#include "rpc.h"

uint8_t *encode_req(Hello__HelloRequest *req, size_t *len) {
  uint8_t *buf;
  static int N = 0;
  *len = hello__hello_request__get_packed_size(req) + MSG_HDR_LEN;
  buf = malloc(*len);
  pack_hdr_t *hdr = (pack_hdr_t *)buf;
  hdr->msg_id = N % 2 + 1;
  N++;
  hello__hello_request__pack(req, buf + MSG_HDR_LEN);
  return buf;
}

Hello__HelloRequest *decode_req(uint8_t *data, size_t data_len) {
  return hello__hello_request__unpack(NULL, data_len, data);
}

uint8_t *encode_resp(Hello__HelloResponse *resp, size_t *len) {
  uint8_t *buf;
  *len = hello__hello_response__get_packed_size(resp) + MSG_HDR_LEN;
  buf = malloc(*len);
  pack_hdr_t *hdr = (pack_hdr_t *)buf;
  hdr->msg_id = 3;
  hello__hello_response__pack(resp, buf + MSG_HDR_LEN);
  return buf;
}

Hello__HelloResponse *decode_resp(uint8_t *data, size_t data_len) {
  return hello__hello_response__unpack(NULL, data_len, data);
}

void free_req_unpack(Hello__HelloRequest *req) {
  hello__hello_request__free_unpacked(req, NULL);
}

void free_resp_unpack(Hello__HelloResponse *resp) {
  hello__hello_response__free_unpacked(resp, NULL);
}
