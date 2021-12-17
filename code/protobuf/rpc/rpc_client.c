#include "rpc.h"

void free_req(void *data, void *hint) {
  free(data);
}

void send_req(void *sock) {
  Hello__HelloRequest req;
  hello__hello_request__init(&req);
  req.value = 123;
  req.msg = "hello";

  uint8_t *data;
  size_t len;
  data = encode_req(&req, &len);

  zmq_msg_t msg;
  zmq_msg_init_data(&msg, data, len, free_req, NULL);
  zmq_msg_send(&msg, sock, 0);
  zmq_msg_close(&msg);
}

void recv_resp(void *sock) {
  zmq_msg_t msg;
  int n;
  zmq_msg_init(&msg);
  n = zmq_msg_recv(&msg, sock, 0);
  if (n < MSG_HDR_LEN)
    return;

  uint8_t *data = zmq_msg_data(&msg);
  size_t len = zmq_msg_size(&msg);
  if (len > MSG_HDR_LEN) {
    Hello__HelloResponse *resp = decode_resp(data + MSG_HDR_LEN, n - MSG_HDR_LEN);
    printf("client recv: %s\n", resp->msg);
    free_resp_unpack(resp);
  }
  zmq_msg_close(&msg);
}

int main(int argc, char **argv) {
  int ret;
  void *ctx;
  void *sock;

  ctx = zmq_ctx_new();
  assert(ctx != NULL);
  sock = zmq_socket(ctx, ZMQ_REQ);
  assert(ctx != NULL);
  ret = zmq_connect(sock, "tcp://localhost:9527");
  assert(ret == 0);

  send_req(sock);
  recv_resp(sock);
  send_req(sock);
  recv_resp(sock);

  zmq_close(sock);
  zmq_ctx_destroy(ctx);

  return 0;
}
