#include "hello.pb.h"
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
  cout << "req: " << req.value() << "," << req.msg() << endl;
}

int main(int argc, char **argv) {
  string str;

  GOOGLE_PROTOBUF_VERIFY_VERSION;

  str = serialize();
  deserialize(str);

  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}
