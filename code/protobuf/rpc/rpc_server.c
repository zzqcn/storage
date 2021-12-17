#include "rpc.h"

void free_resp(void *data, void *hint) {
  free(data);
}

void send_err_msg(void *sock) {
  zmq_msg_t msg;
  void *data;
  pack_hdr_t hdr;

  zmq_msg_init_size(&msg, MSG_HDR_LEN);
  data = zmq_msg_data(&msg);
  hdr.msg_id = 4;
  memcpy(data, &hdr, MSG_HDR_LEN);
  zmq_msg_send(&msg, sock, 0);
  zmq_msg_close(&msg);
}

void send_resp(void *sock, Hello__HelloResponse *resp) {
  size_t len;
  uint8_t *data = encode_resp(resp, &len);
  zmq_msg_t msg;
  zmq_msg_init_data(&msg, data, len, free_resp, NULL);
  zmq_msg_send(&msg, sock, 0);
  zmq_msg_close(&msg);
}

void worker1(void *sock, uint8_t *data, size_t data_len) {
  Hello__HelloRequest *req = decode_req(data, data_len);
  assert(req != NULL);
  printf("worker1 recv: %d, %s\n", req->value, req->msg);
  free_req_unpack(req);

  Hello__HelloResponse resp;
  hello__hello_response__init(&resp);
  resp.msg = "from worker1";
  send_resp(sock, &resp);
}

void worker2(void *sock, uint8_t *data, size_t data_len) {
  Hello__HelloRequest *req = decode_req(data, data_len);
  assert(req != NULL);
  printf("worker2 recv: %d, %s\n", req->value, req->msg);
  free_req_unpack(req);

  Hello__HelloResponse resp;
  hello__hello_response__init(&resp);
  resp.msg = "from worker2";
  send_resp(sock, &resp);
}

int main(int argc, char **argv) {
  int ret;
  void *ctx;
  void *sock;
  zmq_msg_t msg;

  ctx = zmq_ctx_new();
  assert(ctx != NULL);
  sock = zmq_socket(ctx, ZMQ_REP);
  assert(ctx != NULL);
  ret = zmq_bind(sock, "tcp://*:9527");
  assert(ret == 0);

  zmq_pollitem_t fds[] = {
      {sock, 0, ZMQ_POLLIN, 0},
  };

  while (1) {
    ret = zmq_poll(fds, sizeof(fds) / sizeof(fds[0]), -1);
    if (ret <= 0) {
      usleep(10000);
      continue;
    }

    if (fds[0].revents & ZMQ_POLLIN) {
      int n;
      pack_hdr_t *hdr;
      uint8_t *data;

      zmq_msg_init(&msg);
      n = zmq_msg_recv(&msg, sock, 0);
      if (n <= 0)
        goto close;
      if (n < MSG_HDR_LEN) {
        send_err_msg(sock);
        goto close;
      }

      data = zmq_msg_data(&msg);
      n = zmq_msg_size(&msg);
      hdr = (pack_hdr_t *)data;
      if (hdr->msg_id == 1)
        worker1(sock, data + MSG_HDR_LEN, n - MSG_HDR_LEN);
      else if (hdr->msg_id == 2)
        worker2(sock, data + MSG_HDR_LEN, n - MSG_HDR_LEN);
      else
        send_err_msg(sock);

    close:
      zmq_msg_close(&msg);
    }
  }

  zmq_close(sock);
  zmq_ctx_destroy(ctx);

  return 0;
}
