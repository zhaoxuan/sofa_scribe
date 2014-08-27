Introduction
============

This is a c++ scribe client for zipkin.

Requirements
------------

    g++
    boost
    thrift

Installing thrift
-----------------

    git clone git@github.com:apache/thrift.git
    ./configure --prefix=/usr/local
    make
    make install
    
Adding /usr/local to ldconfig path
----------------------------------

    # echo "/usr/local/lib" >> /etc/ld.so.conf
    # ldconfig

Run sofa scribe demo
--------------------

    make
    ./main
    