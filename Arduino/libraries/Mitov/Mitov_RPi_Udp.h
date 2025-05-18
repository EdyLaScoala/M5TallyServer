////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2022 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#pragma once

//---------------------------------------------------------------------------
#include "Arduino.h"

#include <Udp.h>
//#include <cbuf.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
class RPi_cbuf
{
public:
    RPi_cbuf(size_t size);
    ~RPi_cbuf();

    size_t resizeAdd(size_t addSize);
    size_t resize(size_t newSize);
    size_t available() const;
    size_t size();

    size_t room() const;

    inline bool empty() const
    {
        return _begin == _end;
    }

    inline bool full() const
    {
        return wrap_if_bufend(_end + 1) == _begin;
    }

    int peek();
    size_t peek(char *dst, size_t size);

    int read();
    size_t read(char* dst, size_t size);

    size_t write(char c);
    size_t write(const char* src, size_t size);

    void flush();
    size_t remove(size_t size);

    RPi_cbuf *next;

private:
    inline char* wrap_if_bufend(char* ptr) const
    {
        return (ptr == _bufend) ? _buf : ptr;
    }

    size_t _size;
    char* _buf;
    const char* _bufend;
    char* _begin;
    char* _end;

};
//---------------------------------------------------------------------------
class RPi_UDP : public UDP {
private:
  int udp_server;
  IPAddress multicast_ip;
  IPAddress remote_ip;
  uint16_t server_port;
  uint16_t remote_port;
  char * tx_buffer;
  size_t tx_buffer_len;
  RPi_cbuf * rx_buffer;
public:
  RPi_UDP();
  ~RPi_UDP();
  uint8_t begin(IPAddress a, uint16_t p);
  uint8_t begin(uint16_t p);
  uint8_t beginMulticast(IPAddress a, uint16_t p);
  void stop();
  int beginMulticastPacket();
  int beginPacket();
  int beginPacket(IPAddress ip, uint16_t port);
  int beginPacket(const char *host, uint16_t port);
  int endPacket();
  size_t write(uint8_t);
  size_t write(const uint8_t *buffer, size_t size);
  int parsePacket();
  int available();
  int read();
  int read(unsigned char* buffer, size_t len);
  int read(char* buffer, size_t len);
  int peek();
  void flush();
  IPAddress remoteIP();
  uint16_t remotePort();
};
//---------------------------------------------------------------------------
#include <netinet/in.h>
#include <asm/ioctls.h>
//#include <asm/fcntl.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
//#include <string.h>
#include <sys/types.h>

//#include <sys/socket.h>
//#include "WiFi.h"
//#include <lwip/sockets.h>
//#include <lwip/sockets.h>
//#include <lwip/netdb.h>
#include <errno.h>

#include <netdb.h>

#include <netinet/tcp.h>

#undef write
#undef read

RPi_UDP::RPi_UDP()
: udp_server(-1)
, server_port(0)
, remote_port(0)
, tx_buffer(0)
, tx_buffer_len(0)
, rx_buffer(0)
{}

RPi_UDP::~RPi_UDP(){
   stop();
}

uint8_t RPi_UDP::begin(IPAddress address, uint16_t port){
  stop();

  server_port = port;

  tx_buffer = new char[1460];
  if(!tx_buffer){
//    log_e("could not create tx buffer: %d", errno);
    return 0;
  }

  if ((udp_server=socket(AF_INET, SOCK_DGRAM, 0)) == -1){
//    log_e("could not create socket: %d", errno);
    return 0;
  }

  int yes = 1;
  if (setsockopt(udp_server,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes)) < 0) {
//      log_e("could not set socket option: %d", errno);
      stop();
      return 0;
  }

  struct sockaddr_in addr;
  memset((char *) &addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(server_port);
  addr.sin_addr.s_addr = (in_addr_t)address;
  if(bind(udp_server , (struct sockaddr*)&addr, sizeof(addr)) == -1){
//    log_e("could not bind socket: %d", errno);
    stop();
    return 0;
  }
  fcntl(udp_server, F_SETFL, O_NONBLOCK);
  return 1;
}

uint8_t RPi_UDP::begin(uint16_t p){
  return begin(IPAddress(INADDR_ANY), p);
}

uint8_t RPi_UDP::beginMulticast(IPAddress a, uint16_t p){
  if(begin(IPAddress(INADDR_ANY), p)){
    if(a != 0){
      struct ip_mreq mreq;
      mreq.imr_multiaddr.s_addr = (in_addr_t)a;
      mreq.imr_interface.s_addr = INADDR_ANY;
      if (setsockopt(udp_server, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
//          log_e("could not join igmp: %d", errno);
          stop();
          return 0;
      }
      multicast_ip = a;
    }
    return 1;
  }
  return 0;
}

void RPi_UDP::stop(){
  if(tx_buffer){
    delete[] tx_buffer;
    tx_buffer = NULL;
  }
  tx_buffer_len = 0;
  if(rx_buffer){
    RPi_cbuf *b = rx_buffer;
    rx_buffer = NULL;
    delete b;
  }
  if(udp_server == -1)
    return;
  if(multicast_ip != 0){
    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = (in_addr_t)multicast_ip;
    mreq.imr_interface.s_addr = (in_addr_t)0;
    setsockopt(udp_server, IPPROTO_IP, IP_DROP_MEMBERSHIP, &mreq, sizeof(mreq));
    multicast_ip = IPAddress(INADDR_ANY);
  }
  close(udp_server);
  udp_server = -1;
}

int RPi_UDP::beginMulticastPacket(){
  if(!server_port || multicast_ip == IPAddress(INADDR_ANY))
    return 0;
  remote_ip = multicast_ip;
  remote_port = server_port;
  return beginPacket();
}

int RPi_UDP::beginPacket(){
  if(!remote_port)
    return 0;

  // allocate tx_buffer if is necessary
  if(!tx_buffer){
    tx_buffer = new char[1460];
    if(!tx_buffer){
//      log_e("could not create tx buffer: %d", errno);
      return 0;
    }
  }

  tx_buffer_len = 0;

  // check whereas socket is already open
  if (udp_server != -1)
    return 1;

  if ((udp_server=socket(AF_INET, SOCK_DGRAM, 0)) == -1){
//    log_e("could not create socket: %d", errno);
    return 0;
  }

  fcntl(udp_server, F_SETFL, O_NONBLOCK);

  return 1;
}

int RPi_UDP::beginPacket(IPAddress ip, uint16_t port){
  remote_ip = ip;
  remote_port = port;
  return beginPacket();
}

int RPi_UDP::beginPacket(const char *host, uint16_t port){
  struct hostent *server;
  server = gethostbyname(host);
  if (server == NULL){
//    log_e("could not get host from dns: %d", errno);
    return 0;
  }
  return beginPacket(IPAddress((const uint8_t *)(server->h_addr_list[0])), port);
}

int RPi_UDP::endPacket(){
  struct sockaddr_in recipient;
  recipient.sin_addr.s_addr = (uint32_t)remote_ip;
  recipient.sin_family = AF_INET;
  recipient.sin_port = htons(remote_port);
  int sent = sendto(udp_server, tx_buffer, tx_buffer_len, 0, (struct sockaddr*) &recipient, sizeof(recipient));
  if(sent < 0){
//    log_e("could not send data: %d", errno);
    return 0;
  }
  return 1;
}

size_t RPi_UDP::write(uint8_t data){
  if(tx_buffer_len == 1460){
    endPacket();
    tx_buffer_len = 0;
  }
  tx_buffer[tx_buffer_len++] = data;
  return 1;
}

size_t RPi_UDP::write(const uint8_t *buffer, size_t size){
  size_t i;
  for(i=0;i<size;i++)
    write(buffer[i]);
  return i;
}

int RPi_UDP::parsePacket(){
  if(rx_buffer)
    return 0;
  struct sockaddr_in si_other;
  int slen = sizeof(si_other) , len;
  char * buf = new char[1460];
  if(!buf){
    return 0;
  }
  if ((len = recvfrom(udp_server, buf, 1460, MSG_DONTWAIT, (struct sockaddr *) &si_other, (socklen_t *)&slen)) == -1){
    delete[] buf;
    if(errno == EWOULDBLOCK){
      return 0;
    }
//    log_e("could not receive data: %d", errno);
    return 0;
  }
  remote_ip = IPAddress(si_other.sin_addr.s_addr);
  remote_port = ntohs(si_other.sin_port);
  if (len > 0) {
    rx_buffer = new RPi_cbuf(len);
    rx_buffer->write(buf, len);
  }
  delete[] buf;
  return len;
}

int RPi_UDP::available(){
  if(!rx_buffer) return 0;
  return rx_buffer->available();
}

int RPi_UDP::read(){
  if(!rx_buffer) return -1;
  int out = rx_buffer->read();
  if(!rx_buffer->available()){
    RPi_cbuf *b = rx_buffer;
    rx_buffer = 0;
    delete b;
  }
  return out;
}

int RPi_UDP::read(unsigned char* buffer, size_t len){
  return read((char *)buffer, len);
}

int RPi_UDP::read(char* buffer, size_t len){
  if(!rx_buffer) return 0;
  int out = rx_buffer->read(buffer, len);
  if(!rx_buffer->available()){
    RPi_cbuf *b = rx_buffer;
    rx_buffer = 0;
    delete b;
  }
  return out;
}

int RPi_UDP::peek(){
  if(!rx_buffer) return -1;
  return rx_buffer->peek();
}

void RPi_UDP::flush(){
  if(!rx_buffer) return;
  RPi_cbuf *b = rx_buffer;
  rx_buffer = 0;
  delete b;
}

IPAddress RPi_UDP::remoteIP(){
  return remote_ip;
}

uint16_t RPi_UDP::remotePort()
{
  return remote_port;
}
//---------------------------------------------------------------------------
RPi_cbuf::RPi_cbuf(size_t size) :
    next(NULL), _size(size+1), _buf(new char[size+1]), _bufend(_buf + size + 1), _begin(_buf), _end(_begin)
{
}

RPi_cbuf::~RPi_cbuf()
{
    delete[] _buf;
}

size_t RPi_cbuf::resizeAdd(size_t addSize)
{
    return resize(_size + addSize);
}

size_t RPi_cbuf::resize(size_t newSize)
{

    size_t bytes_available = available();
    newSize += 1;
    // not lose any data
    // if data can be lost use remove or flush before resize
    if((newSize < bytes_available) || (newSize == _size)) {
        return _size;
    }

    char *newbuf = new char[newSize];
    char *oldbuf = _buf;

    if(!newbuf) {
        return _size;
    }

    if(_buf) {
        read(newbuf, bytes_available);
        memset((newbuf + bytes_available), 0x00, (newSize - bytes_available));
    }

    _begin = newbuf;
    _end = newbuf + bytes_available;
    _bufend = newbuf + newSize;
    _size = newSize;

    _buf = newbuf;
    delete[] oldbuf;

    return _size;
}

size_t RPi_cbuf::available() const
{
    if(_end >= _begin) {
        return _end - _begin;
    }
    return _size - (_begin - _end);
}

size_t RPi_cbuf::size()
{
    return _size;
}

size_t RPi_cbuf::room() const
{
    if(_end >= _begin) {
        return _size - (_end - _begin) - 1;
    }
    return _begin - _end - 1;
}

int RPi_cbuf::peek()
{
    if(empty()) {
        return -1;
    }

    return static_cast<int>(*_begin);
}

size_t RPi_cbuf::peek(char *dst, size_t size)
{
    size_t bytes_available = available();
    size_t size_to_read = (size < bytes_available) ? size : bytes_available;
    size_t size_read = size_to_read;
    char * begin = _begin;
    if(_end < _begin && size_to_read > (size_t) (_bufend - _begin)) {
        size_t top_size = _bufend - _begin;
        memcpy(dst, _begin, top_size);
        begin = _buf;
        size_to_read -= top_size;
        dst += top_size;
    }
    memcpy(dst, begin, size_to_read);
    return size_read;
}

int RPi_cbuf::read()
{
    if(empty()) {
        return -1;
    }

    char result = *_begin;
    _begin = wrap_if_bufend(_begin + 1);
    return static_cast<int>(result);
}

size_t RPi_cbuf::read(char* dst, size_t size)
{
    size_t bytes_available = available();
    size_t size_to_read = (size < bytes_available) ? size : bytes_available;
    size_t size_read = size_to_read;
    if(_end < _begin && size_to_read > (size_t) (_bufend - _begin)) {
        size_t top_size = _bufend - _begin;
        memcpy(dst, _begin, top_size);
        _begin = _buf;
        size_to_read -= top_size;
        dst += top_size;
    }
    memcpy(dst, _begin, size_to_read);
    _begin = wrap_if_bufend(_begin + size_to_read);
    return size_read;
}

size_t RPi_cbuf::write(char c)
{
    if(full()) {
        return 0;
    }

    *_end = c;
    _end = wrap_if_bufend(_end + 1);
    return 1;
}

size_t RPi_cbuf::write(const char* src, size_t size)
{
    size_t bytes_available = room();
    size_t size_to_write = (size < bytes_available) ? size : bytes_available;
    size_t size_written = size_to_write;
    if(_end >= _begin && size_to_write > (size_t) (_bufend - _end)) {
        size_t top_size = _bufend - _end;
        memcpy(_end, src, top_size);
        _end = _buf;
        size_to_write -= top_size;
        src += top_size;
    }
    memcpy(_end, src, size_to_write);
    _end = wrap_if_bufend(_end + size_to_write);
    return size_written;
}

void RPi_cbuf::flush()
{
    _begin = _buf;
    _end = _buf;
}

size_t RPi_cbuf::remove(size_t size)
{
    size_t bytes_available = available();
    if(size >= bytes_available) {
        flush();
        return 0;
    }
    size_t size_to_remove = (size < bytes_available) ? size : bytes_available;
    if(_end < _begin && size_to_remove > (size_t) (_bufend - _begin)) {
        size_t top_size = _bufend - _begin;
        _begin = _buf;
        size_to_remove -= top_size;
    }
    _begin = wrap_if_bufend(_begin + size_to_remove);
    return available();
}
//---------------------------------------------------------------------------
} // namespace Mitov

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

