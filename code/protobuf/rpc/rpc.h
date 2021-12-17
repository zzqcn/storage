#ifndef __ZZQ_RPC_H__
#define __ZZQ_RPC_H__

#include "hello.pb-c.h"
#include <assert.h>
#include <protobuf2json.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <zmq.h>

typedef struct pack_hdr {
  uint32_t msg_id;
} pack_hdr_t;

#define MSG_HDR_LEN sizeof(pack_hdr_t)

uint8_t *encode_req(Hello__HelloRequest *req, size_t *len);
Hello__HelloRequest *decode_req(uint8_t *data, size_t data_len);

uint8_t *encode_resp(Hello__HelloResponse *resp, size_t *len);
Hello__HelloResponse *decode_resp(uint8_t *data, size_t len);

void free_req_unpack(Hello__HelloRequest *req);
void free_resp_unpack(Hello__HelloResponse *resp);

#endif
