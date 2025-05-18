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
#include "Server.h"
#include "Mitov_RPi_Client.h"

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#define Console UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
class RPi_Server : public Server {
  private:
    int sockfd;
    int _accepted_sockfd = -1;
    uint16_t _port;
    uint8_t _max_clients;
    bool _listening;
    bool _noDelay = false;

  public:
    void listenOnLocalhost(){}

    RPi_Server(uint16_t port=80, uint8_t max_clients=4):sockfd(-1),_accepted_sockfd(-1),_port(port),_max_clients(max_clients),_listening(false),_noDelay(false){}
    ~RPi_Server(){ end();}
    RPi_Client available();
    RPi_Client accept(){return available();}

    void begin( uint16_t port );
	virtual void begin() override
	{
		begin( 0 );
	}

    void setNoDelay(bool nodelay);
    bool getNoDelay();
    bool hasClient();
    size_t write(const uint8_t *data, size_t len);
    size_t write(uint8_t data){
      return write(&data, 1);
    }
    using Print::write;

    void end();
	void close() { end(); }
    void stop() { end(); }
    operator bool(){return _listening;}
    int setTimeout(uint32_t seconds);
	void stopAll() {}
};
//---------------------------------------------------------------------------
#undef write
#undef close

int RPi_Server::setTimeout(uint32_t seconds)
{
  struct timeval tv;
  tv.tv_sec = seconds;
  tv.tv_usec = 0;
  if(setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(struct timeval)) < 0)
    return -1;

  return setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&tv, sizeof(struct timeval));
}

size_t RPi_Server::write(const uint8_t *data, size_t len)
{
  return 0;
}

RPi_Client RPi_Server::available()
{
//  Console.println( "available 0" );

  if(!_listening)
    return RPi_Client();

//  Console.println( "available 1" );

  int client_sock;
  if (_accepted_sockfd >= 0) 
  {
    client_sock = _accepted_sockfd;
    _accepted_sockfd = -1;
  }

  else 
  {
    struct sockaddr_in _client;
    int cs = sizeof(struct sockaddr_in);
//    client_sock = lwip_accept_r(sockfd, (struct sockaddr *)&_client, (socklen_t*)&cs);
    client_sock = ::accept(sockfd, (struct sockaddr *)&_client, (socklen_t*)&cs);
  }

  if(client_sock >= 0)
  {
    int val = 1;
//    if(setsockopt(client_sock, SOL_SOCKET, SO_KEEPALIVE, (char*)&val, sizeof(int)) == ESP_OK) {
    if(setsockopt(client_sock, SOL_SOCKET, SO_KEEPALIVE, (char*)&val, sizeof(int)) == 0 ) 
	{
      val = _noDelay;
//      if(setsockopt(client_sock, IPPROTO_TCP, TCP_NODELAY, (char*)&val, sizeof(int)) == ESP_OK)
      if(setsockopt(client_sock, IPPROTO_TCP, TCP_NODELAY, (char*)&val, sizeof(int)) == 0 )
        return RPi_Client(client_sock);
    }
  }
  return RPi_Client();
}

void RPi_Server::begin(uint16_t port)
{
//  Console.println( "begin 1" );
  if(_listening)
    return;

//  Console.println( "begin 2" );
  if(port)
      _port = port;

  struct sockaddr_in server;
  sockfd = socket(AF_INET , SOCK_STREAM, 0);
  if (sockfd < 0)
    return;

//  Console.println( "begin 3" );

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(_port);
  if(bind(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0)
  {
	end();
//    Console.println( "Can't bind" );
    return;
  }

//  Console.println( "begin 4" );

  if(listen(sockfd, _max_clients) < 0)
  {
	end();
    return;
  }

//  Console.println( "begin 5" );

  fcntl(sockfd, F_SETFL, O_NONBLOCK);
  _listening = true;

//  Console.println( "begin" );

  _noDelay = false;
  _accepted_sockfd = -1;
}

void RPi_Server::setNoDelay(bool nodelay) 
{
    _noDelay = nodelay;
}

bool RPi_Server::getNoDelay() 
{
    return _noDelay;
}

bool RPi_Server::hasClient() 
{
    if (_accepted_sockfd >= 0) 
      return true;

	struct sockaddr_in _client;
    int cs = sizeof(struct sockaddr_in);
//    _accepted_sockfd = lwip_accept_r(sockfd, (struct sockaddr *)&_client, (socklen_t*)&cs);
    _accepted_sockfd = ::accept(sockfd, (struct sockaddr *)&_client, (socklen_t*)&cs);
    if (_accepted_sockfd >= 0) 
      return true;

	return false;
}

void RPi_Server::end()
{
  ::close(sockfd);
  sockfd = -1;
  _listening = false;
}
//---------------------------------------------------------------------------
} // namespace Mitov

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#undef Console
#endif

