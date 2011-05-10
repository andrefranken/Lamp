/******************************************************************************
 *
 *  Latest Chatty Command Line Client
 *  By Electroly.
 *
 *  COMM.H
 *
 *  Network communications.  Allows GET and POST requests to a web server.
 *  
 *****************************************************************************/

#pragma once

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "constant.h"

error_t comm_init     (const char* host);
error_t comm_download (const char* host, const char* path, 
                       std::string *stdstring,
                       const char* post_data,
                       const char* cookie);
void    comm_cleanup  (void);

using boost::asio::ip::tcp;

class client
{
public:
  client(boost::asio::io_service& io_service,
         const std::string& server, 
         const std::string& path,
         bool bPost,
         std::string *stlstring,
         const char *cookie,
         const char *postdata);

  ~client()
  {
      if(m_data != NULL)
         free(m_data);
  }
private:
  void handle_resolve(const boost::system::error_code& err,
                      tcp::resolver::iterator endpoint_iterator);

  void handle_connect(const boost::system::error_code& err,
                      tcp::resolver::iterator endpoint_iterator);

  void handle_write_request(const boost::system::error_code& err);

  void handle_read_status_line(const boost::system::error_code& err);

  void handle_read_headers(const boost::system::error_code& err);

  void handle_read_content(const boost::system::error_code& err);

  tcp::resolver resolver_;
  tcp::socket socket_;
  boost::asio::streambuf request_;
  boost::asio::streambuf response_;

  std::string m_headers;
  char *m_data;
  size_t m_datasize;
  size_t m_mallocsize;
  std::string *m_stlstring;
};