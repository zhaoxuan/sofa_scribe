/*
 * formatters.h
 * Copyright (C) 2014 john <john@apple.local>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef FORMATTERS_H
#define FORMATTERS_H
#include <string>
#include "zipkinCore_types.h"

/*
 * max random number for uniq_id
 * XXX: By experimentation zipkin has trouble recording traces with ids larger than (2 ** 56) - 1
 * pow(2, 56) - 1 = 72057594037927935
 */
const long MAX_RAND = 72057594037927935;

int ipv4_to_long(std::string);
long uniq_id();
long timestamp();

/*
 * serialize a thrift object to binary string
 */
//void serialize_to_binary(Span, std::string);

#endif /* !FORMATTERS_H */

