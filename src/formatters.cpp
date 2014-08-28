/*
 * formatters.cpp
 * Copyright (C) 2014 john <john@apple.local>
 *
 * Distributed under terms of the MIT license.
 */

#include "formatters.h"
#include <string>
#include <arpa/inet.h>
#include <math.h>
#include <cstdlib>
#include <sys/time.h>
//#include <boost/shared_ptr.hpp>
//#include <thrift/protocol/TBinaryProtocol.h>
//#include <thrift/transport/TSocket.h>
//#include <thrift/transport/TTransportUtils.h>

using namespace std;
//using namespace apache::thrift;  
//using namespace apache::thrift::transport;  
//using namespace apache::thrift::protocol; 

int ipv4_to_long(string ip) {
  //in_addr addr;
  //inet_aton(ip.c_str(), &addr);
  //return addr.s_addr;
  int a, b, c, d;
  sscanf(ip.c_str(), "%d.%d.%d.%d", &a, &b, &c, &d);
  long ip_long = ( a << 24 ) | ( b << 16 ) | ( c << 8 ) | d;
  return ip_long;
}

long uniq_id() {
  struct timeval tv;
  gettimeofday(&tv,NULL);
  srand(tv.tv_usec);
  long rand1 = abs(rand());
  long rand2 = abs(rand());
  rand1 = ((long)rand1) << (sizeof(int)*8);
  long randL = (rand1 | rand2);
  return randL;
}

long timestamp() {
  struct timeval tv;
  gettimeofday(&tv,NULL);
  return tv.tv_sec * 1000 * 1000 + tv.tv_usec;
}

//void serialize_to_binary(Span& obj, string& s) {
  //boost::shared_ptr<TMemoryBuffer> trans(new TMemoryBuffer());  
  //boost::shared_ptr<TProtocol> proto(new TBinaryProtocol(trans));  
  //obj.write(proto.get());  
  //s = trans->getBufferAsString();  
//}
