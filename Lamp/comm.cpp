/******************************************************************************
 *
 *  Latest Chatty Command Line Client
 *  By Electroly.
 *
 *  COMM.C
 *
 *  Network communications.  Allows GET and POST requests to a web server.
 *  
 *****************************************************************************/

#include "stdafx.h"
#include "platform.h"
#include "comm.h"
#include "mydownlib.h"


/* Internal variables */

/******************************************************************************
 *
 *  comm_download
 *
 *  PARAMETERS: host      - [in] Hostname to connect to.
 *              path      - [in] Path to request from the web server.
 *              buffer    - [out] Will contain a pointer to a new buffer 
 *                          containing the requested data.  Must be freed by
 *                          the caller.
 *              size      - [out] Will contain the number of bytes of data
 *                          returned in `buffer'.
 *              post_data - [in] If non-NULL, then we are making a POST
 *                          request, and this is the payload.
 *  RETURNS:    Error code.
 *  PURPOSE:    Downloads the specified URL into 'buffer'.
 *  
 *****************************************************************************/
error_t comm_download(const char*     host,
                      const char*     path, 
                      std::string*    stdstring,
                      const char*     post_data,
                      const char*     cookie)
{
   error_t result = OK;

   try
   {
      boost::asio::io_service io_service;

      bool bPost = false;
      if(post_data != NULL)
      {
         bPost = true;
      }
              
      client c(io_service, host, path, bPost, stdstring, cookie, post_data);
      io_service.run();
   }
   catch (std::exception& e)
   {
      std::cout << "Exception: " << e.what() << "\n";
   }

   return result;
}


client::client(boost::asio::io_service& io_service,
               const std::string& server, 
               const std::string& path,
               bool bPost,
               std::string *stlstring,
               const char *cookie,
               const char *postdata)
               : resolver_(io_service),
               socket_(io_service),
               m_stlstring(stlstring)
{
   m_data = NULL;
   m_datasize = 0;
   m_mallocsize = 0;

   if(postdata == NULL) 
      bPost = false;

   // Form the request. We specify the "Connection: close" header so that the
   // server will close the socket after transmitting the response. This will
   // allow us to treat all data up until the EOF as the content.
   std::ostream request_stream(&request_);
   if(bPost)
   {
      request_stream << "POST ";
   }
   else
   {
      request_stream << "GET ";
   }
   request_stream << path << " HTTP/1.0\r\n";
   request_stream << "Host: " << server << "\r\n";
   request_stream << "Accept: */*\r\n";
   request_stream << "Accept-Charset: utf-8\r\n";
   request_stream << "Connection: close\r\n";
   request_stream << "User-Agent: Lamp v1.8\r\n";
   if(cookie != NULL)
   {
      request_stream << "Cookie: ";
      request_stream << cookie;
      request_stream << "\r\n";
   }
   if(bPost)
   {
      request_stream << "Content-Type: application/x-www-form-urlencoded\r\n";
      request_stream << "Content-Length: ";
      
      char countstr[50];
      countstr[0] = 0;
      _ultoa_s(strlen(postdata), countstr, 50, 10);
      request_stream << countstr;

      request_stream << "\r\n";
   }
   request_stream << "\r\n"; // end header

   if(bPost)
   {
      request_stream << postdata;
   }

   // Start an asynchronous resolve to translate the server and service names
   // into a list of endpoints.
   tcp::resolver::query query(server, "http");
   resolver_.async_resolve(query,
      boost::bind(&client::handle_resolve, this,
      boost::asio::placeholders::error,
      boost::asio::placeholders::iterator));
}

void client::handle_resolve(const boost::system::error_code& err,
                            tcp::resolver::iterator endpoint_iterator)
{
   if (!err)
   {
      // Attempt a connection to the first endpoint in the list. Each endpoint
      // will be tried until we successfully establish a connection.
      tcp::endpoint endpoint = *endpoint_iterator;
      socket_.async_connect(endpoint,
         boost::bind(&client::handle_connect, this,
         boost::asio::placeholders::error, ++endpoint_iterator));
   }
   else
   {
      std::cout << "Error: " << err.message() << "\n";
   }
}

void client::handle_connect(const boost::system::error_code& err,
                            tcp::resolver::iterator endpoint_iterator)
{
   if (!err)
   {
      // The connection was successful. Send the request.
      boost::asio::async_write(socket_, request_,
         boost::bind(&client::handle_write_request, this,
         boost::asio::placeholders::error));
   }
   else if (endpoint_iterator != tcp::resolver::iterator())
   {
      // The connection failed. Try the next endpoint in the list.
      socket_.close();
      tcp::endpoint endpoint = *endpoint_iterator;
      socket_.async_connect(endpoint,
         boost::bind(&client::handle_connect, this,
         boost::asio::placeholders::error, ++endpoint_iterator));
   }
   else
   {
      std::cout << "Error: " << err.message() << "\n";
   }
}

void client::handle_write_request(const boost::system::error_code& err)
{
   if (!err)
   {
      // Read the response status line. The response_ streambuf will
      // automatically grow to accommodate the entire line. The growth may be
      // limited by passing a maximum size to the streambuf constructor.
      boost::asio::async_read_until(socket_, response_, "\r\n",
         boost::bind(&client::handle_read_status_line, this,
         boost::asio::placeholders::error));
   }
   else
   {
      std::cout << "Error: " << err.message() << "\n";
   }
}

void client::handle_read_status_line(const boost::system::error_code& err)
{
   if (!err)
   {
      // Check that response is OK.
      std::istream response_stream(&response_);
      std::string http_version;
      response_stream >> http_version;
      unsigned int status_code;
      response_stream >> status_code;
      std::string status_message;
      std::getline(response_stream, status_message);
      if (!response_stream || http_version.substr(0, 5) != "HTTP/")
      {
         std::cout << "Invalid response\n";
         return;
      }
      if (status_code != 200)
      {
         std::cout << "Response returned with status code ";
         std::cout << status_code << "\n";
         return;
      }

      if(status_message.length() > 0)
      {
         size_t responsesize = status_message.length();
         if(m_data == NULL)
         {
            m_datasize = 0;
            m_mallocsize = __max(responsesize + 1, 1024 * 10);
            m_data = (char*)malloc(m_mallocsize);
         }
         else if(m_datasize + responsesize > m_mallocsize)
         {
            size_t extra = __max(responsesize + 1, 1024 * 10);
            m_mallocsize += extra;
            char *newdata = (char*)realloc(m_data, m_mallocsize);
            if(newdata == NULL)
            {
               MessageBox(NULL,L"realloc failed",L"oops",MB_OK);
            }
            else
            {
               m_data = newdata;
            }
         }

         memcpy( m_data + m_datasize, status_message.data(), (int)responsesize );

         m_datasize += responsesize;
         m_data[m_datasize] = 0;

         if(m_stlstring != NULL)
         {
            size_t size_before = m_stlstring->length();
            (*m_stlstring) += m_data;
            if(m_stlstring->length() - size_before != m_datasize)
            {
               MessageBox(NULL,L"stdstring resize failed",L"oops",MB_OK);
            }

            m_datasize = 0;
         }
      }

      // Read the response headers, which are terminated by a blank line.
      boost::asio::async_read_until(socket_, response_, "\r\n\r\n",
         boost::bind(&client::handle_read_headers, this,
         boost::asio::placeholders::error));
   }
   else
   {
      std::cout << "Error: " << err << "\n";
   }
}

void client::handle_read_headers(const boost::system::error_code& err)
{
   if (!err)
   {
      // Process the response headers.
      std::istream response_stream(&response_);
      std::string line;
      while (std::getline(response_stream, line) && line != "\r")
      {
         m_headers += line;
         m_headers += "\n";
      }
      m_headers += "\n";

      if(m_headers.length() > 0)
      {
         size_t responsesize = m_headers.length();
         if(m_data == NULL)
         {
            m_datasize = 0;
            m_mallocsize = __max(responsesize + 1, 1024 * 10);
            m_data = (char*)malloc(m_mallocsize);
         }
         else if(m_datasize + responsesize > m_mallocsize)
         {
            size_t extra = __max(responsesize + 1, 1024 * 10);
            m_mallocsize += extra;
            char *newdata = (char*)realloc(m_data, m_mallocsize);
            if(newdata == NULL)
            {
               MessageBox(NULL,L"realloc failed",L"oops",MB_OK);
            }
            else
            {
               m_data = newdata;
            }
         }

         memcpy( m_data + m_datasize, m_headers.data(), (int)responsesize );

         m_datasize += responsesize;
         m_data[m_datasize] = 0;

         if(m_stlstring != NULL)
         {
            size_t size_before = m_stlstring->length();
            (*m_stlstring) += m_data;
            if(m_stlstring->length() - size_before != m_datasize)
            {
               MessageBox(NULL,L"stdstring resize failed",L"oops",MB_OK);
            }

            m_datasize = 0;
         }
      }

      // Write whatever content we already have to output.
      size_t responsesize = response_.size();
      if (responsesize > 0)
      {
         if(m_data == NULL)
         {
            m_datasize = 0;
            m_mallocsize = __max(responsesize + 1, 1024 * 10);
            m_data = (char*)malloc(m_mallocsize);
         }
         else if(m_datasize + responsesize > m_mallocsize)
         {
            size_t extra = __max(responsesize + 1, 1024 * 10);
            m_mallocsize += extra;
            char *newdata = (char*)realloc(m_data, m_mallocsize);
            if(newdata == NULL)
            {
               MessageBox(NULL,L"realloc failed",L"oops",MB_OK);
            }
            else
            {
               m_data = newdata;
            }
         }

         char *thisresponse = m_data + m_datasize;
         response_stream.read( thisresponse, (int)responsesize );

         m_datasize += responsesize;
         m_data[m_datasize] = 0;

         if(m_stlstring != NULL)
         {
            size_t size_before = m_stlstring->length();
            (*m_stlstring) += m_data;
            if(m_stlstring->length() - size_before != m_datasize)
            {
               MessageBox(NULL,L"stdstring resize failed",L"oops",MB_OK);
            }

            m_datasize = 0;
         }
      }

      // Start reading remaining data until EOF.
      boost::asio::async_read(socket_, response_,
         boost::asio::transfer_at_least(1),
         boost::bind(&client::handle_read_content, this,
         boost::asio::placeholders::error));
   }
   else
   {
      std::cout << "Error: " << err << "\n";
   }
}

void client::handle_read_content(const boost::system::error_code& err)
{
   if (!err)
   {
      // Write all of the data that has been read so far.
      size_t responsesize = response_.size();
      if (responsesize > 0)
      {
         std::istream response_stream(&response_);
         if(m_data == NULL)
         {
            m_datasize = 0;
            m_mallocsize = __max(responsesize + 1, 1024 * 10);
            m_data = (char*)malloc(m_mallocsize);
         }
         else if(m_datasize + responsesize > m_mallocsize)
         {
            size_t extra = __max(responsesize + 1, 1024 * 10);
            m_mallocsize += extra;
            char *newdata = (char*)realloc(m_data, m_mallocsize);
            if(newdata == NULL)
            {
               MessageBox(NULL,L"realloc failed",L"oops",MB_OK);
            }
            else
            {
               m_data = newdata;
            }
         }

         char *thisresponse = m_data + m_datasize;
         response_stream.read( thisresponse, (int)responsesize );

         m_datasize += responsesize;

         m_data[m_datasize] = 0;

         if(m_stlstring != NULL)
         {
            size_t size_before = m_stlstring->length();
            (*m_stlstring) += m_data;
            if(m_stlstring->length() - size_before != m_datasize)
            {
               MessageBox(NULL,L"stdstring resize failed",L"oops",MB_OK);
            }

            m_datasize = 0;
         }
      }

      // Continue reading remaining data until EOF.
      boost::asio::async_read(socket_, response_,
            boost::asio::transfer_at_least(1),
            boost::bind(&client::handle_read_content, this,
            boost::asio::placeholders::error));
   }
   else if (err != boost::asio::error::eof)
   {
      std::cout << "Error: " << err << "\n";
   }
}
