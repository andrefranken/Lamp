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


/* Internal variables */
struct hostent* hostent;


/******************************************************************************
 *
 *  comm_init
 *
 *  PARAMETERS: None.
 *  RETURNS:    Error code.
 *  PURPOSE:    Starts up Winsock.
 *  
 *****************************************************************************/
error_t comm_init(const char* host)
{
   os_sockets_init();

   hostent = gethostbyname(host);

   return OK;
}


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
                      char** buffer, 
                      unsigned int*   size,
                      const char*     post_data)
{
   SOCKET             sock;
   struct sockaddr_in addr_in;
   char*              request;
   char*              response      = NULL;
   unsigned int       response_size = 0;
   unsigned int       expected_size = 0;
   char               response_buffer[4096];
   char*              match;
   int                recv_bytes;

   /* Connect to the server. */
   sock = socket(AF_INET, SOCK_STREAM, 0);

   memset(&addr_in, 0, sizeof(addr_in));
   addr_in.sin_family      = AF_INET;
   addr_in.sin_addr.s_addr = ((struct in_addr *)(hostent->h_addr))->s_addr;
   addr_in.sin_port        = htons(80);

   if (SOCKET_ERROR == connect(sock, (struct sockaddr*) &addr_in, 
                               sizeof(addr_in)))
      return -1;

   /* Send the HTTP request. */
   if (post_data == NULL)
   {
      const char* format = "GET %s HTTP/1.1\nHost: %s\n\n";
      size_t requrestsize = strlen(format) + strlen(path) + strlen(host);
      request = (char*)malloc(requrestsize);
      sprintf_s(request, requrestsize, format, path, host);
   }
   else
   {
      const char* format = "POST %s HTTP/1.1\nHost: %s\n"
                           "Content-Type: application/x-www-form-urlencoded\n"
                           "Content-Length: %d\n\n%s";
      size_t requrestsize = strlen(format) + strlen(path) + strlen(host) + strlen(post_data) + 16;
      request = (char*)malloc(requrestsize);
      sprintf_s(request, requrestsize, format, path, host, strlen(post_data), post_data);
   }

   if (send(sock, request, (int) strlen(request), 0) == SOCKET_ERROR)
      return FAIL;

   free(request);

   bool bQuit = false;

   /* Get the response. */
   do
   {
      recv_bytes = recv(sock, response_buffer, 4096, 0);
      
      if (recv_bytes > 0)
      {
         /* Resize the response buffer and copy the newly received bytes in. */
         response_size += recv_bytes;
         response       = (char*)realloc(response, response_size + 1);
         memcpy(response + response_size - recv_bytes, 
                response_buffer, 
                recv_bytes);
         response[response_size] = 0; /* Null terminate the response. */
      }
      else if (recv_bytes < 0)
      {
         /* Something has gone wrong. */
         closesocket(sock);
         return FAIL;
      }

      /* See if we've read the headers yet. */
      if (expected_size == 0 && response != NULL)
      {
         /* If we've begun the content, then the headers have all been read. */
         match = strstr(response, "<?xml");
         if (match != NULL)
         {
            /* Get the length of the HTTP header. */
            expected_size = (unsigned int)(match - response);

            /* Add to that the length of the XML content. */
            match = strstr(response, "Content-Length: ");
            match += strlen("Content-Length: ");
            expected_size += atoi(match);
         }
         else
         {
            // don't ask, if you think we are done.
            if(post_data != NULL &&
               recv_bytes < 4096 &&
               strstr(response, "HTTP/1.1 200 OK") != NULL)
            {
               bQuit = true;
            }
         }
      }
   } while (!bQuit && recv_bytes > 0 && 
            (expected_size == 0 || response_size < expected_size));

   closesocket(sock);

   /* Check for a timeout */
   if (response_size == 0)
      return FAIL;

   /* Return the response to the caller. */
   *buffer = response;
   *size   = response_size;
   return OK;
}



/******************************************************************************
 *
 *  comm_cleanup
 *
 *  PARAMETERS: None.
 *  RETURNS:    None.
 *  PURPOSE:    Shuts down Winsock.
 *  
 *****************************************************************************/
void comm_cleanup(void)
{
   os_sockets_cleanup();
}