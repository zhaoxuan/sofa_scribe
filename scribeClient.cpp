#include <iostream>
#include <protocol/TBinaryProtocol.h>
#include <transport/TSocket.h>
#include <transport/TTransportUtils.h>

#include "src/scribe.h"
#include "src/zipkinCore_types.h"
#include "src/zipkinCore_constants.h"
#include "src/formatters.h"
#include "src/base64.h"



using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace scribe::thrift;



//void serialize(Span& obj, string& s) {
  //boost::shared_ptr<TMemoryBuffer> trans(new TMemoryBuffer());
  //boost::shared_ptr<TProtocol> proto(new TBinaryProtocol(trans));
  //obj.write(proto.get());
  //s = trans->getBufferAsString();
//}

int main(){

  // Endpoint is current machine ip and port and service_name
  Endpoint endpoint;
  endpoint.ipv4 = (int32_t)ipv4_to_long("127.0.0.1");
  endpoint.port = 80;
  endpoint.service_name = "www";

  Annotation annotation_cs;
  annotation_cs.timestamp = (long)timestamp();
  annotation_cs.value = zipkinCoreConstants().CLIENT_SEND;
  annotation_cs.host = endpoint;

  Annotation annotation_cr;
  annotation_cr.timestamp = (long)timestamp();
  annotation_cr.value = zipkinCoreConstants().CLIENT_RECV;
  annotation_cr.host = endpoint;

  Span span;
  span.trace_id = uniq_id();
  span.name = "test";
  span.id = uniq_id();
  //optional parent id
  //span.parent_id = uniq_id();
  std::vector<Annotation> annotations;
  annotations.push_back(annotation_cs);
  annotations.push_back(annotation_cr);

  span.annotations = annotations;
  //span.binary_annotations = {};
 
  // serialize span to binary
  boost::shared_ptr<TMemoryBuffer> trans(new TMemoryBuffer());
  boost::shared_ptr<TBinaryProtocol> proto(new TBinaryProtocol(trans));
  span.write(proto.get());
  string s;
  s = trans->getBufferAsString();
  cout << s << endl;

  std::string encoded;
  encoded = base64_encode(reinterpret_cast<const unsigned char*>(s.c_str()), s.length());
  std::cout << "encoded: " << encoded << std::endl;


  boost::shared_ptr<TSocket> socket(new TSocket("127.0.0.1", 9410));
  boost::shared_ptr<TFramedTransport> transport(new TFramedTransport(socket));
  //boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
  boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

  try {
    scribeClient client(protocol);
    string category = "zipkin";
    transport->open();
    LogEntry le;
    le.message = encoded;
    le.category = category;

    std::vector<LogEntry> messages;
    messages.push_back(le);
    client.Log(messages);
    transport->close();
  }catch(TException &tx){
    printf("ERROR: %s/n",tx.what());
  }
  return 0;
}
