#include "rpc.h"

#define ENC_JSON_FLAGS (JSON_INDENT(2) | JSON_PRESERVE_ORDER)
#define DEC_JSON_FLAGS 0

uint8_t *encode_req(Hello__HelloRequest *req, size_t *len, int json_mode) {
  uint8_t *buf = NULL;
  static int N = 0;

  if (!json_mode) {
    *len = hello__hello_request__get_packed_size(req) + MSG_HDR_LEN;
    buf = malloc(*len);
    pack_hdr_t *hdr = (pack_hdr_t *)buf;
    hdr->codec = CODEC_PROTOBUF;
    hdr->msg_id = N % 2 + 1;
    N++;
    hello__hello_request__pack(req, buf + MSG_HDR_LEN);
  } else {
    char *json_str;
    protobuf2json_string(&req->base, ENC_JSON_FLAGS, &json_str, NULL, 0);
    *len = strlen(json_str) + MSG_HDR_LEN;
    buf = malloc(*len);
    pack_hdr_t *hdr = (pack_hdr_t *)buf;
    hdr->codec = CODEC_JSON;
    hdr->msg_id = N % 2 + 1;
    N++;
    memcpy(buf + MSG_HDR_LEN, json_str, strlen(json_str));
    free(json_str);
  }

  return buf;
}

Hello__HelloRequest *decode_req(uint8_t *data, size_t data_len, int json_mode) {
  if (!json_mode) {
    return hello__hello_request__unpack(NULL, data_len, data);
  } else {
    char *json_str = (char *)data;
    ProtobufCMessage *msg;
    json2protobuf_string(json_str, DEC_JSON_FLAGS, &hello__hello_request__descriptor, &msg, NULL,
                         0);
    return (Hello__HelloRequest *)msg;
  }
}

uint8_t *encode_resp(Hello__HelloResponse *resp, size_t *len, int json_mode) {
  uint8_t *buf = NULL;

  if (!json_mode) {
    *len = hello__hello_response__get_packed_size(resp) + MSG_HDR_LEN;
    buf = malloc(*len);
    pack_hdr_t *hdr = (pack_hdr_t *)buf;
    hdr->msg_id = 3;
    hello__hello_response__pack(resp, buf + MSG_HDR_LEN);
  } else {
    char *json_str;
    protobuf2json_string(&resp->base, ENC_JSON_FLAGS, &json_str, NULL, 0);
    *len = strlen(json_str) + MSG_HDR_LEN;
    buf = malloc(*len);
    pack_hdr_t *hdr = (pack_hdr_t *)buf;
    hdr->codec = CODEC_JSON;
    hdr->msg_id = 3;
    memcpy(buf + MSG_HDR_LEN, json_str, strlen(json_str));
    free(json_str);
  }

  return buf;
}

Hello__HelloResponse *decode_resp(uint8_t *data, size_t data_len, int json_mode) {
  if (!json_mode) {
    return hello__hello_response__unpack(NULL, data_len, data);
  } else {
    char *json_str = (char *)data;
    ProtobufCMessage *msg;
    json2protobuf_string(json_str, DEC_JSON_FLAGS, &hello__hello_response__descriptor, &msg, NULL,
                         0);
    return (Hello__HelloResponse *)msg;
  }
}

void free_req_unpack(Hello__HelloRequest *req, int json_mode) {
  if (!json_mode)
    hello__hello_request__free_unpacked(req, NULL);
  else
    free(req);
}

void free_resp_unpack(Hello__HelloResponse *resp, int json_mode) {
  if (!json_mode)
    hello__hello_response__free_unpacked(resp, NULL);
  else
    free(resp);
}
