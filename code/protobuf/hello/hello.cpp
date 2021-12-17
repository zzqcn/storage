#include "hello.pb.h"
#include <google/protobuf/util/json_util.h>
#include <iostream>
#include <string>
using namespace std;

string serialize() {
  string out;
  hello::HelloRequest req;

  req.set_value(123);
  req.set_msg("Hello");
  req.SerializeToString(&out);
  return out;
}

void deserialize(string str) {
  hello::HelloRequest req;

  req.ParseFromString(str);
  cout << "deserialize: " << req.value() << "," << req.msg() << endl;
}

void with_json() {
  hello::HelloRequest req, req2;
  string str;

  req.set_value(123);
  req.set_msg("Hello");

  google::protobuf::util::Status status = google::protobuf::util::MessageToJsonString(req, &str);
  cout << "to JSON: " << str << endl;

  status = google::protobuf::util::JsonStringToMessage(str, &req2);
  cout << "from JSON: " << req2.value() << "," << req.msg() << endl;
}

int main(int argc, char **argv) {
  string str;

  GOOGLE_PROTOBUF_VERIFY_VERSION;

  str = serialize();
  deserialize(str);

  with_json();

  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}
