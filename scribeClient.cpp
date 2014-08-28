/*
 * scribeClient.cpp
 * Copyright (C) 2014 john <john@apple.local>
 *
 * Distributed under terms of the MIT license.
 */

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

int main(int argc, char *argv[]){
  if (argc != 3) {
    printf("Pls use main ip port.\n");
    printf("Like this: ./main 117.185.16.31 9410\n");
    return 0;
  }

  string zipkin_ip = argv[1];
  int zipkin_port = atoi(argv[2]);

  /*
   * Endpoint is current machine ip and port and service_name
   * include (int32_t)ip, (int)port, (string)service_name
   */
  Endpoint endpoint;
  endpoint.ipv4 = (int32_t)ipv4_to_long("127.0.0.1");
  endpoint.port = 80;
  endpoint.service_name = "www";

  /*
   * Annotation is some event took place, either one by the framework or by the user
   * event included four values:
   *              zipkinCoreConstants().CLIENT_SEND
   *              zipkinCoreConstants().CLIENT_RECV
   *              zipkinCoreConstants().SERVER_RECV
   *              zipkinCoreConstants().SERVER_SEND
   * (long) timestamp, (Endpoint) host
   */
  Annotation annotation_cs;
  annotation_cs.timestamp = (long)timestamp();
  annotation_cs.value = zipkinCoreConstants().CLIENT_SEND;
  annotation_cs.__set_host(endpoint);

  Annotation annotation_cr;
  annotation_cr.timestamp = (long)timestamp();
  annotation_cr.value = zipkinCoreConstants().CLIENT_RECV;
  annotation_cr.__set_host(endpoint);

  /*
   * Span is one RPC call
   * trace_id is global unique id for all spans
   *     uniq_id() is a function to generate uuid in formatters.h
   * name is RPC name
   * id is current machine unique id for this span
   * annotations is array<Annotation>
   * binary_annotations is array<binary_annotations>
   */
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
 
  /*
   * serialize span to binary
   *
   */
  boost::shared_ptr<TMemoryBuffer> trans(new TMemoryBuffer());
  boost::shared_ptr<TBinaryProtocol> proto(new TBinaryProtocol(trans));
  span.write(proto.get());
  string s;
  s = trans->getBufferAsString();
  cout << "binary string: " <<s << endl;

  /*
   * encode binary string to base64 string
   */
  std::string encoded;
  encoded = base64_encode(reinterpret_cast<const unsigned char*>(s.c_str()), s.length());
  cout << "encoded: " << encoded << endl;


  /*
   * Open socket, connect to zipkin server
   * Use BinaryProtocol to send data
   */
  boost::shared_ptr<TSocket> socket(new TSocket(zipkin_ip, zipkin_port));
  boost::shared_ptr<TFramedTransport> transport(new TFramedTransport(socket));
  boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

  /*
   * Use scribe client to send the trace span to zipkin
   */
  boost::shared_ptr<scribeClient> client(new scribeClient(protocol));

  try {
    string category = "zipkin";
    transport->open();
    LogEntry le;
    le.message = encoded;
    le.category = category;

    std::vector<LogEntry> messages;
    messages.push_back(le);
    client->Log(messages);
    transport->close();
  }catch(TException &tx){
    printf("ERROR: %s/n",tx.what());
  }
  return 0;
}
