/*

mydownlib
by Luigi Auriemma
e-mail: aluigi@autistici.org
web:    aluigi.org

    Copyright 2006,2007,2008,2009 Luigi Auriemma

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

    http://www.gnu.org/licenses/gpl-2.0.txt
*/
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stdint.h"
#include <stdarg.h>
#include <sys/stat.h>
#include <time.h>
#include <ctype.h>
#include "../../lamp/zlib/zlib.h"
#include "mydownlib.h"
#include "winsock.h"


#ifndef stristr
#define stristr strstr
#endif

#define VISDELAY    500
#define BUFFSZ      8192
#define MAXTIMEOUT  60          // one minute
#define MAXARGS     16          // modify it if you need more args in mydown_scanhead
#define MAXDNS      320
#define VERPRINTF   if(verbose >= 0) fprintf(stderr,
#define VERPRINTF2  if(verbose > 0)  fprintf(stderr,
#define TEMPOZ(x)   TEMPOZ1;    \
                    x = TEMPOZ2


typedef struct {                // lame DNS caching implementation
    u8          *host;
    in_addr_t   ip;
} dns_db_t;

int         dns_db_max      = 0,
            dns_db_add      = 0;
dns_db_t    dns_db[MAXDNS];



void mydown_get_host(u8 *url, u8 **hostx, u16 *portx, u8 **urix, u8 **userx, u8 **passx, int verbose);
u8 *mydown_http_delimit(u8 *data);
u8 *mydown_uri2hex(u8 *uri);
u8 *mydown_hex2uri(u8 *uri);
void mydown_scanhead(u8 *data, int datalen, ...);
u8 *mydown_tmpnam(void);
void mydown_free_sock(int *sock);
u8 *mydown_http_skip(u8 *buff, int len, int *needed, int *remain);
void mydown_free(u8 **buff);
int mydown_chunked_skip(u8 *buff, int chunkedsize);
int mydown_unzip(z_stream z, u8 *in, int inlen, u8 **outx, int *outxlen);
int mydown_sscanf_hex(u8 *data, int datalen);
int mydown_timeout(int sock, int secs);
int mydown_recv(int sd, u8 *data, int len, int timeout);
u8 *mydown_showhttp80(u16 port);
void mydown_showstatus(u32 fsize, u32 ret, u32 oldret, int timediff);
u8 *mydown_base64_encode(u8 *data, int *length);



u32 mydown(u8 *myurl, u8 *filename, mydown_options *opt) {
    FILE    *fd          = NULL;
    u32     from         = 0,
            tot          = 0,
            filesize     = MYDOWN_ERROR;
    int     showhead     = 0,
            resume       = 0,
            onlyifdiff   = 0,
            verbose      = 0,
            *keep_alive  = 0,
            *ret_code    = 0,
            timeout      = 0,
            onflyunzip   = 0,
            contentsize  = 0;
    u16     port         = 0;
    u8      *url         = NULL,
            *uri         = NULL,
            *host        = NULL,
            *user        = NULL,
            *pass        = NULL,
            *referer     = NULL,
            *useragent   = NULL,
            *cookie      = NULL,
            *more_http   = NULL,
            **filedata   = NULL,
            *content     = NULL,
            *get         = NULL;

#ifdef WIN32
    WSADATA wsadata;
    WSAStartup(MAKEWORD(1,0), &wsadata);
#endif

    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);

    if(opt) {
        from         = opt->from;
        tot          = opt->tot;
        showhead     = opt->showhead;
        resume       = opt->resume;
        onlyifdiff   = opt->onlyifdiff;
        user         = opt->user;
        pass         = opt->pass;
        referer      = opt->referer;
        useragent    = opt->useragent;
        cookie       = opt->cookie;
        more_http    = opt->more_http;
        verbose      = opt->verbose;
        filedata     = opt->filedata;
        keep_alive   = opt->keep_alive;
        timeout      = opt->timeout;
        ret_code     = opt->ret_code;
        onflyunzip   = opt->onflyunzip;
        content      = opt->content;
        contentsize  = opt->contentsize;
        get          = opt->get;

        if(user) {
            VERPRINTF2
                "  user   %s\n"
                "  pass   %s\n",
                user,
                pass);
        }
    }

    if(!myurl) return(MYDOWN_ERROR);
    url = (u8*)_strdup((const char *)myurl);
    if(!url) return(MYDOWN_ERROR);
    mydown_get_host(url, &host, &port, &uri, &user, &pass, verbose);

    if(opt != NULL)
    {
      if(user == NULL && opt->user != NULL)
      {
         user = opt->user;
      }

      if(pass == NULL && opt->pass != NULL)
      {
         pass = opt->pass;
      }
    }

    VERPRINTF2"  start download\n");

    filesize = mydown_http2file(
        keep_alive,         // int *sock
        timeout,            // int timeout
        host,               // u8 *host
        port,               // u16 port
        user,               // u8 *user
        pass,               // u8 *pass
        referer,            // u8 *referer
        useragent,          // u8 *useragent
        cookie,             // u8 *cookie
        more_http,          // u8 *more_http
        verbose,            // int verbose
        uri,                // u8 *getstr
        fd,                 // FILE *fd
        filename,           // u8 *filename
        showhead,           // int showhead
        onlyifdiff,         // int onlyifdiff
        resume,             // int resume
        from,               // u32 from
        tot,                // u32 tot
        NULL,               // u32 *filesize
        filedata,           // u8 **filedata
        ret_code,           // int *ret_code
        onflyunzip,         // int onflyunzip
        content,            // u8 *content
        contentsize,        // int contentsize
        get                 // u8 *get
    );

    if(fd && (fd != stdout)) fclose(fd);
    if(url) free(url);
    if(uri) free(uri);
    return(filesize);
}



void mydown_get_host(u8 *url, u8 **hostx, u16 *portx, u8 **urix, u8 **userx, u8 **passx, int verbose) {
    u16     port  = 80;
    u8      *host = NULL,
            *uri  = NULL,
            *user = NULL,
            *pass = NULL,
            *p;

    host = url;

    p = (u8*)strstr((const char *)host, "://");    // handle http://
    if(!p) p = (u8*)strstr((const char *)host, ":\\\\");
    if(p) {
        for(p += 3; *p; p++) {  // in case of http:////
            if((*p != '/') && (*p != '\\')) break;
        }
        host = p;
    }

    for(p = host; *p; p++) {    // search the uri
        if((*p == '/') || (*p == '\\')) {
            uri = p;
            break;
        }
    }
    if(uri) {
        *uri++ = 0;
        uri = mydown_uri2hex(uri);
    }
    if(!uri) uri = (u8*)_strdup("");  // in case of no uri or mydown_uri2hex fails

    p = (u8*)strrchr((const char *)host, '@');
    if(p) {
        *p = 0;

        user = host;

        pass = (u8*)strchr((const char *)host, ':');
        if(pass) {
            *pass++ = 0;
        } else {
            pass = (u8*)"";
        }

        host = p + 1;
    }

    p = (u8*)strchr((const char *)host, ':');
    if(p) {
        *p = 0;
        port = atoi((const char *)p + 1);
    }

    VERPRINTF"  %s\n", url);
    VERPRINTF2
        "  host   %s : %hu\n"
        "  uri    %s\n",
        host, port,
        uri);

    if(user) {
        VERPRINTF2
            "  user   %s\n"
            "  pass   %s\n",
            user,
            pass);
    }

    *hostx = host;
    *portx = port;
    *urix  = uri;
    *userx = user;
    *passx = pass;
}



u8 *mydown_http_delimit(u8 *data) {
    u8      *p;

    if(!data || !data[0]) return(NULL);

    p = data;
    while(*p && (*p != '\r') && (*p != '\n')) p++;
    *p = 0;
    for(p++; *p && ((*p == '\r') || (*p == '\n')); p++);
    return(p);
}



u8 *mydown_uri2hex(u8 *uri) {
    static const u8 hex[17] = "0123456789abcdef";
    u8      *ret,
            *p,
            c;

    ret = (u8*)malloc((strlen((const char *)uri) * 3) + 1);
    if(!ret) return(NULL);

    for(p = ret; *uri; uri++) {
        c = *uri;
        if(isprint(c) && !strchr(" \"<>#%{}|\\^~[]`", c)) {
            *p++ = c;
        } else {
            *p++ = '%';
            *p++ = hex[c >> 4];
            *p++ = hex[c & 15];
        }
    }
    *p = 0;

    return(ret);
}



u8 *mydown_hex2uri(u8 *uri) {
    int     t,
            n;
    u8      *ret,
            *p;

    ret = (u8*)_strdup((const char *)uri);
    if(!ret) return(NULL);

    for(p = ret; *uri; uri++, p++) {
        t = *uri;
        if((*uri == '%') && (*(uri + 1) == 'u')) {
            if(sscanf_s((const char *)uri + 1, "u%04x", &t) == 1) uri += 5;
        } else if(*uri == '%') {
            if(sscanf_s((const char *)uri + 1, "%02x",  &t) == 1) uri += 2;
        } else if(*uri == '&') {
            if(*(uri + 1) == '#') {
                if((sscanf_s((const char *)uri + 1, "#%d;%n", &t, &n) == 1) && (n > 2)) {
                    uri += n;
                } else {
                    t = *uri;
                }
            } else if(!_strnicmp((const char *)uri, "&quot;",   6)) {
                t = '\"';
                uri += 5;
            } else if(!_strnicmp((const char *)uri, "&amp;",    5)) {
                t = '&';
                uri += 4;
            } else if(!_strnicmp((const char *)uri, "&frasl;",  7)) {
                t = '/';
                uri += 6;
            } else if(!_strnicmp((const char *)uri, "&lt;",     4)) {
                t = '<';
                uri += 3;
            } else if(!_strnicmp((const char *)uri, "&gt;",     4)) {
                t = '>';
                uri += 3;
            } else if(!_strnicmp((const char *)uri, "&nbsp;",   6)) {
                t = 160;
                uri += 5;
            } else if(!_strnicmp((const char *)uri, "&middot;", 8)) {
                t = 183;
                uri += 7;
            }
        }
        *p = t;
    }
    *p = 0;

    return(ret);
}



void mydown_scanhead(u8 *data, int datalen, ...) {
    va_list ap;
    int     i,
            vals;
    u8      *par[MAXARGS],
            **val[MAXARGS],
            *l,
            *p,
            *limit;

    va_start(ap, datalen);
    for(i = 0; i < MAXARGS; i++) {
        par[i] = va_arg(ap, u8 *);
        if(!par[i]) break;
        val[i] = va_arg(ap, u8 **);
        if(!val[i]) break;
        *val[i] = NULL;
    }
    vals = i;
    va_end(ap);

    for(limit = data + datalen; (l = mydown_http_delimit(data)); data = l) {
        if(l > limit) break;
        p = (u8*)strchr((const char *)data, ':');
        if(!p) continue;
        *p++ = 0;
        for(i = 0; i < vals; i++) {
            if(_stricmp((const char *)data, (const char *)par[i])) continue;
            while(*p && ((*p == ' ') || (*p == '\t'))) p++;
            *val[i] = p;
            break;
        }
    }
}



u32 mydown_http2file(int *sock, int timeout, u8 *host, u16 port, u8 *user, u8 *pass, u8 *referer, u8 *useragent, u8 *cookie, u8 *more_http, int verbose, u8 *getstr, FILE *fd, u8 *filename, int showhead, int onlyifdiff, int resume, u32 from, u32 tot, u32 *filesize, u8 **filedata, int *ret_code, int onflyunzip, u8 *content, int contentsize, u8 *get) {
#ifndef WIN32
    struct  timeb   timex;
#endif
    z_stream    z;
    FILE    *oldfd          = fd;
    struct  sockaddr_in peer;
    struct  stat    xstat;
    time_t  oldtime         = 0,
            newtime;
    u32     ret             = 0,
            httpret         = 0,
            vishttpret      = 0,
            fsize           = 0;
    int     sd              = 0,
            t,
            err,
            len,
            malloclen,
            code            = 0,
            b64len,
            filedatasz      = 0,
            httpcompress    = 0,
            httpgzip        = 0,
            httpdeflate     = 0,
            httpz           = 0,
            chunked         = 0,
            chunkedsize     = 0,
            chunkedlen      = 0,
            wbits,
            zbufflen        = 0,
            httpskipbytes   = 0;
    u8      *buff           = NULL,
            *query          = NULL,
            *data           = NULL,
            *p              = NULL,
            *s              = NULL,
            *userpass       = NULL,
            *b64            = NULL,
            *conttype       = NULL,
            *contlen        = NULL,
            *contdisp       = NULL,
            *icyname        = NULL,
            *transenc       = NULL,
            *contenc        = NULL,
            *connection     = NULL,
            *location       = NULL,
            *filedatatmp    = NULL,
            *zbuff          = NULL,
            *ztmp           = NULL,
            *chunkedbuff    = NULL,
            *chunkedtmp     = NULL,
            *filenamemalloc = NULL,
            *redir_host     = NULL,
            *redir_getstr   = NULL,
            httpgzip_flags  = 0;

    

#define GOTOQUIT    { ret = MYDOWN_ERROR; goto quit; }
    
    if(!sock || !*sock) {
        peer.sin_addr.s_addr = mydown_resolv((char *)host);
        if(peer.sin_addr.s_addr == INADDR_NONE) GOTOQUIT;
        peer.sin_port        = htons(port);
        peer.sin_family      = AF_INET;

        sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if(sd < 0) GOTOQUIT;
        if(sock) *sock = sd;

        VERPRINTF2"  connect to %s:%hu...", inet_ntoa(peer.sin_addr), port);
        if(connect(sd, (struct sockaddr *)&peer, sizeof(peer)) < 0) {
            fprintf(stderr, "\nError: connection refused\n");
            GOTOQUIT;
        }
        VERPRINTF2"done\n");
    } else {
        sd = *sock;
    }

    if(user && pass) {
        size_t allocsize = strlen((const char *)user) + 1 + strlen((const char *)pass) + 1;
        userpass = (u8*)malloc(allocsize);
        if(!userpass) GOTOQUIT;
        b64len = sprintf_s((char *)userpass, allocsize, "%s:%s", user, pass);
        b64 = mydown_base64_encode(userpass, &b64len);
    }

    if(!get) get = (u8*)"GET";
    if(content && !contentsize) contentsize = strlen((const char *)content);

    malloclen =
        300                                       + // my format strings and HTTP parameters
        strlen((const char *)get)                               +
        strlen((const char *)getstr)                            +
        strlen((const char *)host)                              +
        5                                         +
        ((from || tot)   ? 20 + 20           : 0) +
        (b64             ? strlen((const char *)b64)       : 0) +
        (referer         ? strlen((const char *)referer)   : 0) +
        (useragent       ? strlen((const char *)useragent) : 0) +
        (cookie          ? strlen((const char *)cookie)    : 0) +
        (more_http       ? strlen((const char *)more_http) : 0) +
        (content         ? 100 : 0) +
        100;// extra room, just in case

    query = (u8*)malloc(malloclen);
    if(!query) GOTOQUIT;

    len = sprintf_s(
        (char *)query,malloclen,
        "%s /%s HTTP/1.1\r\n"
        "Host: %s%s\r\n"
        "Connection: %s\r\n",
        (const char *)get, (const char *)getstr,
        (const char *)host, (const char *)mydown_showhttp80(port),
        sock ? "keep-alive" : "close");

#define HTTP_APPEND  len += sprintf_s((char *)query + len, malloclen - len,

    if(!onlyifdiff && (showhead != 2)) {   // x-compress needs unlzw???
        HTTP_APPEND "Accept-Encoding: deflate,gzip,x-gzip,compress,x-compress\r\n");
    }
    if(from || tot) {
        HTTP_APPEND "Range: bytes=");
        if(from != -1) HTTP_APPEND "%u", from);
        HTTP_APPEND "-");
        if(tot >= 1)   HTTP_APPEND "%u", (tot - 1) + ((from == -1) ? 0 : from));
        HTTP_APPEND "\r\n");
    }
    if(b64) {
        HTTP_APPEND "Authorization: Basic %s\r\n", b64);
    }
    if(referer) {
        HTTP_APPEND "Referer: %s\r\n", referer);
    }
    if(useragent) {
        HTTP_APPEND "User-Agent: %s\r\n", useragent);
    }
    if(cookie) {
        HTTP_APPEND "Cookie: %s\r\n", cookie);
    }
    if(more_http) {
        HTTP_APPEND "%s", more_http);
        if(query[len - 1] == '\r') {
            HTTP_APPEND "\n");
        } else if(query[len - 1] != '\n') {
            HTTP_APPEND "\r\n");
        }
    }
    if(content) {
        HTTP_APPEND "Content-Type: application/x-www-form-urlencoded\r\nContent-length: %d\r\n", contentsize);
    }
    HTTP_APPEND "\r\n");

#undef HTTP_APPEND  

    err = send(sd, (const char *)query, len, 0);
    mydown_free(&query);
    if(err != len) GOTOQUIT;
    if(content) {
        err = send(sd, (const char *)content, contentsize, 0);
        if(err != contentsize) GOTOQUIT;
    }

    buff = (u8*)malloc(BUFFSZ + 1);
    if(!buff) GOTOQUIT;

    data = p = buff;
    len  = BUFFSZ;
    while((t = mydown_recv(sd, data, len, timeout)) > 0) {
        data += t;
        len  -= t;
        *data = 0;

        p = (u8*)strstr((const char *)buff, "\r\n\r\n");
        if(p) {
            p += 4;
        } else {
            p = (u8*)strstr((const char *)buff, "\n\n");
            if(p) p += 2;
        }

        if(!p) continue;
        *(p - 1) = 0;

        if(showhead == 1) {
            if(filedata) *filedata = (u8*)_strdup((const char *)buff);
            VERPRINTF"\n%s", buff);
            goto quit;
        }
        if(verbose == 2) fprintf(stderr, "\n%s", buff);

        s = (u8*)strchr((const char *)buff, ' ');
        if(s) {
            code = atoi((const char *)s + 1);

            if((code / 100) == 3) {
                mydown_scanhead(buff, p - buff,
                    "location",     &location,
                    NULL,           NULL);
                if(!location) {
                    fprintf(stderr, "\nError: remote file is temporary unavailable (%d)\n", code);
                    GOTOQUIT;
                }
                VERPRINTF2"\n- redirect: %s\n", location);
                mydown_get_host(location, &redir_host, &port, &redir_getstr, &user, &pass, verbose);

                // don't support redirects where the host and getstr are identical
                if(strcmp((const char*)host,(const char*)redir_host) != 0 ||
                   strcmp((const char*)getstr,(const char*)redir_getstr) != 0)
                {
                   host = redir_host;
                   getstr = redir_getstr;
                   
                   if(sd && !sock) { closes(sd); sd = 0; }
                   mydown_free_sock(sock);
                   ret = mydown_http2file(sock, timeout, host, port, user, pass, referer, useragent, cookie, more_http, verbose, getstr, fd, filename, showhead, onlyifdiff, resume, from, tot, filesize, filedata, ret_code, onflyunzip, content, contentsize, get);
                }
                goto quit;
            }

                // if((code != 200) && (code != 206)) {
            if((code / 100) != 2) {
                fprintf(stderr, "\nError: remote file is temporary unavailable (%d)\n", code);
                GOTOQUIT;
            }
        }

        mydown_scanhead(buff, p - buff,
            "content-length",       &contlen,
            "content-type",         &conttype,
            "content-disposition",  &contdisp,
            "icy-name",             &icyname,
            "transfer-encoding",    &transenc,
            "content-encoding",     &contenc,
            "Set-Cookie",           &cookie,
            "Connection",           &connection,
            NULL,                   NULL);

        if(connection) {
            if(!_stricmp((const char *)connection, "close") && sock) *sock = 0;
        }

        if(contlen) {
            s = (u8*)strchr((const char *)contlen, '/');
            if(s) contlen = s + 1;
            //if(_atoi64(contlen) > (u64)0x7fffffff) {
            //    fprintf(stderr, "\nError: large files are not yet supported by mydownlib\n");
            //    GOTOQUIT;
            //}
            if(sscanf_s((const char *)contlen, "%u", &fsize) != 1) break;
        }

        if(conttype && onflyunzip) {    // needed or better
            if(stristr((const char *)conttype, "compress"))   httpcompress  = 1;
            if(stristr((const char *)conttype, "gzip"))       httpgzip      = 1;
            if(!onflyunzip) if(stristr((const char *)conttype, "x-gzip")) httpgzip = 0;   // work-around
            if(stristr((const char *)conttype, "deflate"))    httpdeflate   = 1;
        }
        if(contenc) {
            if(stristr((const char *)contenc,  "compress"))   httpcompress  = 1;
            if(stristr((const char *)contenc,  "gzip"))       httpgzip      = 1;
            if(!onflyunzip) if(stristr((const char *)contenc,  "x-gzip")) httpgzip = 0;   // work-around
            if(stristr((const char *)contenc,  "deflate"))    httpdeflate   = 1;
        }

        if(!contdisp && icyname) contdisp = icyname;

        if(transenc && stristr((const char *)transenc, "chunked")) chunked = 1;

        if(filesize) *filesize = fsize;

        if(filename && ((resume == 2) || (resume == 3))) {
            fd = (FILE *)filename;
            if(!fd) {
                fprintf(stderr, "\nError: no FILE used with resume %d\n", resume);
                GOTOQUIT;
            }
            filename = (u8*)"."; // this instruction is useless, I have added it only to skip the checks below
        }

        if(!filename || !filename[0]) {
            if(contdisp) {
                s = (u8 *)stristr((const char *)contdisp, "filename=");
                if(!s) s = (u8 *)stristr((const char *)contdisp, "file=");
                if(s) {
                    s = (u8*)strchr((const char *)s, '=') + 1;
                } else {
                    s = contdisp;
                }
                while(*s && ((*s == '\"') || (*s == ' ') || (*s == '\t'))) s++;
                filename = mydown_hex2uri(s);
                if(filename) filenamemalloc = filename; // needed for freeing it later!
                if(filename && filename[0]) {
                    for(s = filename; *s; s++) {
                        if((*s == '\\') || (*s == '/') || (*s == ';') || (*s == ':') || (*s == '\"') || (*s == '&') || (*s == '?')) break;
                    }
                    for(s--; (s >= filename) && *s && ((*s == ' ') || (*s == '\t')); s--);
                    *(s + 1) = 0;
                }
            } else {
                filename = mydown_hex2uri(getstr);
                if(filename) filenamemalloc = filename; // needed for freeing it later!
                if(filename && filename[0]) {
                    for(
                      s = filename + strlen((const char *)filename) - 1;
                      (s >= filename) && (*s != '/') && (*s != '\\') && (*s != ':') && (*s != '&') && (*s != '?') && (*s != '=');
                      s--);
                    filename = s + 1;
                }
            }

            if(filename) {                          // last useless check to avoid directory traversal
                s = (u8*)strrchr((const char *)filename, ':');
                if(s) filename = s + 1;
                s = (u8*)strrchr((const char *)filename, '\\');
                if(s) filename = s + 1;
                s = (u8*)strrchr((const char *)filename, '/');
                if(s) filename = s + 1;

                s = (u8*)strrchr((const char *)filename, '.');         // automatic gzip decompression on the fly
                if(s && !_stricmp((const char *)s, ".gz") && onflyunzip) {
                    httpgzip = 1;
                    *s = 0;
                }
            }
        }

        if(!filename || !filename[0]) {
            filename = mydown_tmpnam();
            if(filename) filenamemalloc = filename; // needed for freeing it later!
            //VERPRINTF"\nError: no filename retrieved, you must specify an output filename\n\n");
            //GOTOQUIT;
        }

        if(showhead == 2) {
            if(filedata) *filedata = (u8*)_strdup((const char *)filename);
            ret = fsize;
            goto quit;
        }

        if(!filedata && !fd) {
            if(!strcmp((const char *)filename, "-")) {
                fd = stdout;
                VERPRINTF"  file   %s\n", "stdout");
            } else {
                VERPRINTF"  file   %s\n", filename);
                err = stat((const char *)filename, &xstat);
                if(onlyifdiff && !err && (xstat.st_size == fsize)) {
                    VERPRINTF"  the remote file has the same size of the local one, skip\n");
                    GOTOQUIT;
                }
                if((err < 0) || !resume) {      // file doesn't exist or must not resume
                    fopen_s(&fd,(const char *)filename, "wb");
                } else {
                    fopen_s(&fd,(const char *)filename, "ab");
                    from = xstat.st_size;
                    VERPRINTF2"  resume %u\n", from);
                    if(sd && !sock) { closes(sd); sd = 0; }
                    mydown_free_sock(sock);
                    ret = mydown_http2file(sock, timeout, host, port, user, pass, referer, useragent, cookie, more_http, verbose, getstr, fd, filename, showhead, onlyifdiff, resume, from, tot, filesize, filedata, ret_code, onflyunzip, content, contentsize, get);
                    goto quit;
                }
                if(!fd) GOTOQUIT;
            }
        }
        break;
    }
    if(t < 0) GOTOQUIT;

    if(!p) p = buff;
    len = data - p;
    memmove(buff, p, len);

    httpz = 1;
    if(onflyunzip < 0) httpz = 0;   // disabled forcely!
    if(httpcompress) {
        VERPRINTF2"  compression: compress\n");
        wbits =  15;
    } else if(httpgzip) {
        VERPRINTF2"  compression: gzip\n");
        wbits = -15;
    } else if(httpdeflate) {
        VERPRINTF2"  compression: deflate\n");
        wbits = -15;
    } else {
        httpz = 0;
    }
    if(httpz) {
        z.zalloc = (alloc_func)0;
        z.zfree  = (free_func)0;
        z.opaque = (voidpf)0;
        if(inflateInit2(&z, wbits)) GOTOQUIT;

        zbufflen = BUFFSZ * 4;
        zbuff    = (u8*)malloc(zbufflen);
        if(!zbuff) GOTOQUIT;
    }

    if(verbose > 0) {
        fprintf(stderr, "\n");
        if(fsize) fprintf(stderr, "    ");
        fprintf(stderr, " | downloaded   | kilobytes/second\n");
        if(fsize) fprintf(stderr, "----");
        fprintf(stderr, "-/--------------/-----------------\n");
    }

    if(verbose >= 0) {
        TEMPOZ(oldtime);
        oldtime -= VISDELAY;
    }

    if(filedata) {
        filedatasz  = fsize;
        filedatatmp = (u8*)malloc(filedatasz);
        if(!filedatatmp) GOTOQUIT;
    }

    if(chunked) chunkedsize = len;

    do {
redo:
        httpret += len;

        if(chunked) {
            for(;;) {
                chunkedsize = mydown_chunked_skip(buff, chunkedsize);

                err = mydown_sscanf_hex(buff, chunkedsize);
                if(err > 0) break;
                if(!err) {
                    chunkedsize = mydown_chunked_skip(buff, chunkedsize);
                    break;
                }

                t = mydown_recv(sd, buff + chunkedsize, BUFFSZ - chunkedsize, timeout);
                if(t <= 0) GOTOQUIT;
                chunkedsize += t;
                if(chunkedsize >= BUFFSZ) GOTOQUIT;
            }

            chunkedlen = err;
            if(!chunkedlen) break;

            if(chunkedbuff) free(chunkedbuff);
            chunkedbuff = (u8*)malloc(chunkedlen);
            if(!chunkedbuff) GOTOQUIT;

            s = (u8 *)strchr((const char *)buff, '\n');
            if(!s) GOTOQUIT;
            err = (s + 1) - buff;
            chunkedsize -= err;
            memmove(buff, buff + err, chunkedsize);

            if(chunkedlen < chunkedsize) {      // we have more data than how much we need
                memcpy(chunkedbuff, buff, chunkedlen);
                chunkedsize -= chunkedlen;
                memmove(buff, buff + chunkedlen, chunkedsize);
            } else {                            // we have only part of the needed data
                memcpy(chunkedbuff, buff, chunkedsize);
                for(len = chunkedsize; len < chunkedlen; len += t) {
                    t = mydown_recv(sd, chunkedbuff + len, chunkedlen - len, timeout);
                    if(t <= 0) GOTOQUIT;
                }
                chunkedsize = 0;
            }

            chunkedtmp  = buff;
            buff        = chunkedbuff;
            len         = chunkedlen;
        }

            /* DECOMPRESSION */

        if(httpz) {
            if(httpgzip && !ret) {  // gzip is really bad to handle in my code... blah
                t = len;
                s = buff;
                if((httpgzip == 1) && !httpskipbytes && (len >= 2)) {
                    if((buff[0] == 0x1f) && (buff[1] == 0x8b)) {
                        httpskipbytes = 3;  // CM is usually 8, no need to check it
                    } else {                // in this case it's a raw deflate stream
                        httpgzip = -1;      // -1 means that it's no longer needed to check the header
                        t = 0;              // handle all the buffer
                    }
                }

                if(httpgzip > 0) {
                    for(;;) {
                        s = mydown_http_skip(s, t, &httpskipbytes, &t);
                        if(!s) {
                            t = len;
                            break;
                        }
                        if(httpgzip == 1) {
                            httpgzip_flags = *s++;      // flags
                            httpskipbytes  = 4 + 1 + 1; // mtime + xfl + os
                            httpgzip++;
                        } else if(httpgzip == 2) {      // xfl flag
                            httpskipbytes = 0;
                            if(httpgzip_flags & 4) {
                                if(t >= 2) {            // uff boring, not 100% correct
                                    httpskipbytes = s[0] | (s[1] << 8);
                                    s += 2;
                                }
                            }
                            httpgzip++;
                        } else if(httpgzip == 3) {
                            httpskipbytes = 0;
                            if(httpgzip_flags & 8) {    // name
                                while((s - buff) < len) {
                                    if(!*s++) {
                                        httpgzip++;
                                        break;
                                    }
                                }
                            } else {
                                httpgzip++;
                            }
                        } else if(httpgzip == 4) {
                            httpskipbytes = 0;
                            if(httpgzip_flags & 16) {   // comment
                                while((s - buff) < len) {
                                    if(!*s++) {
                                        httpgzip++;
                                        break;
                                    }
                                }
                            } else {
                                httpgzip++;
                            }
                        } else if(httpgzip == 5) {
                            httpskipbytes = 0;
                            if(httpgzip_flags & 2) {    // crc
                                httpskipbytes = 2;
                            }
                            httpgzip++;
                        } else {
                            t = s - buff;
                            break;
                        }
                    }
                }
                len = mydown_unzip(z, buff + t, len - t, &zbuff, &zbufflen);
            } else {
                len = mydown_unzip(z, buff, len, &zbuff, &zbufflen);
            }
            if(len < 0) GOTOQUIT;
            ztmp = buff;
            buff = zbuff;
        }

            /* UPDATE THE AMOUNT OF UNCOMPRESSED BYTES DOWNLOADED */
            // ret is the total size of the data we have downloaded (uncompressed)
            // httpret is the total size of the data we have downloaded from the server
            // len is the size of the current block of data we have downloaded (uncompressed)

        ret += len;

            /* WRITE THE DATA INTO FILE OR MEMORY */

        if(tot && (ret > tot)) {
            len = tot - (ret - len);
            ret = tot;
        }

        if(filedata) {
            if(filedatasz < (int)ret) {
                filedatasz  = ret;
                filedatatmp = (u8*)realloc(filedatatmp, filedatasz);
                if(!filedatatmp) GOTOQUIT;
            }
            memcpy(filedatatmp + ret - len, buff, len);
        } else if(fd) {
            if(fwrite(buff, 1, len, fd) != len) {
                fprintf(stderr, "\nError: I/O error. Probably your disk is full or the file is write protected\n");
                GOTOQUIT;
            }
            //fflush(fd);
        }
        if(ret_code && (resume == 3)) *ret_code = ret;

            /* VISUALIZATION */

        if(verbose >= 0) {
            TEMPOZ(newtime);
            if((newtime - oldtime) >= VISDELAY) {
                mydown_showstatus(fsize, httpret, vishttpret, (int)(newtime - oldtime));
                oldtime = newtime;
                vishttpret = httpret;
            }
        }

            /* FREE, EXCHANGE OR OTHER STUFF */

        if(httpz) {
            zbuff = buff;
            buff  = ztmp;
        }
        if(chunked) {
            chunkedbuff = buff;
            buff        = chunkedtmp;
            len         = 0;
            goto redo;
        }

            /* FSIZE CHECK */

        if(tot && (ret == tot)) break;
        if(fsize) {
            if(httpret >= fsize) break;
        }

            /* READ NEW DATA FROM THE STREAM */

    } while((len = mydown_recv(sd, buff, BUFFSZ, timeout)) > 0);

    if(verbose >= 0) {
        TEMPOZ(newtime);
        mydown_showstatus(fsize, httpret, vishttpret, (int)(newtime - oldtime));
    }

    if(fsize && (len < 0)) GOTOQUIT;

    if(filedata) {
        *filedata = filedatatmp;
    }

quit:
    if(httpz) {
        if(zbuff) inflateEnd(&z);
        if(zbuff != buff) mydown_free(&zbuff);
    }
    if(chunkedbuff != buff) mydown_free(&chunkedbuff);
    mydown_free(&userpass);
    mydown_free(&b64);
    mydown_free(&buff);
    mydown_free(&filenamemalloc);
    if(ret_code) {
        *ret_code = code;
        if(resume == 3) *ret_code = ret;
    }
    if(sd && !sock) { closes(sd); sd = 0; }
    VERPRINTF"\n");
    if(ret == MYDOWN_ERROR) mydown_free_sock(sock);
    if(filename && ((resume == 2) || (resume == 3))) {
        // do nothing
    } else {
        if(!oldfd && fd && (fd != stdout)) fclose(fd);
    }
    return(ret);

#undef GOTOQUIT
}



u8 *mydown_tmpnam(void) {
    FILE    *fd;
    int     i;
    u8      *ret;

    ret = (u8*)malloc(32);
    for(i = 1; ; i++) {
        sprintf_s((char *)ret, 32, "%u.myd", i);  // enough small for any OS (8.3 too)
        fopen_s(&fd,(const char *)ret, "rb");      // check real existence of the file
        if(!fd) break;
        fclose(fd);
    }
    return(ret);
}



void mydown_free_sock(int *sock) {
    if(sock && *sock) {
        closes(*sock);
        *sock = 0;
    }
}



u8 *mydown_http_skip(u8 *buff, int len, int *needed, int *remain) {
    int     rest;

    rest = *needed;
    if(len < rest) {
        *needed = rest - len;
        *remain = 0;
        return(NULL);
    }
    *needed = 0;
    *remain = len - rest;
    return(buff + rest);
}



void mydown_free(u8 **buff) {
    if(!*buff) return;
    free(*buff);
    *buff = NULL;
}



int mydown_chunked_skip(u8 *buff, int chunkedsize) {
    int     t;

    for(t = 0; t < chunkedsize; t++) {
        if((buff[t] != '\r') && (buff[t] != '\n')) break;
    }
    if(t) {
        chunkedsize -= t;
        memmove(buff, buff + t, chunkedsize);
    }
    return(chunkedsize);
}



int mydown_unzip(z_stream z, u8 *in, int inlen, u8 **outx, int *outxlen) {
    int     zerr,
            outsz;
    u8      *out;

    if(inlen <= 0) return(0);

    out     = *outx;
    outsz   = *outxlen;

    z.next_in   = in;
    z.avail_in  = inlen;

    for(;;) {
        z.next_out  = out   + z.total_out;
        z.avail_out = outsz - z.total_out;

        zerr = inflate(&z, Z_NO_FLUSH);

        if(zerr == Z_STREAM_END) break;
        if((zerr != Z_OK) && (zerr != Z_BUF_ERROR)) {
            fprintf(stderr, "\nError: zlib error %d\n", zerr);
            z.total_out = MYDOWN_ERROR;
            break;
        }

        if(!z.avail_in) break;

        outsz += (inlen << 1);      // inlen * 2 should be enough each time
        out = (u8*)realloc(out, outsz);
        if(!out) {
            outsz       = 0;
            z.total_out = MYDOWN_ERROR;
            break;
        }
    }

    *outx    = out;
    *outxlen = outsz;
    return(z.total_out);
}



int mydown_sscanf_hex(u8 *data, int datalen) {
    int     i,
            ret;

    for(i = 0; i < datalen; i++) {
        if(data[i] == '\n') break;
    }
    if(i == datalen) return(MYDOWN_ERROR);

    if(sscanf_s((const char *)data, "%x", &ret) != 1) return(MYDOWN_ERROR);
    return(ret);
}



int mydown_timeout(int sock, int secs) {
    struct  timeval tout;
    fd_set  fdr;
    int     err;

    tout.tv_sec  = secs;
    tout.tv_usec = 0;
    FD_ZERO(&fdr);
    FD_SET(sock, &fdr);
    err = select(sock + 1, &fdr, NULL, NULL, &tout);
    if(err < 0) return(MYDOWN_ERROR); //std_err();
    if(!err) return(MYDOWN_ERROR);
    return(0);
}



int mydown_recv(int sd, u8 *data, int len, int timeout) {
    if(!timeout) timeout = MAXTIMEOUT;
    if(mydown_timeout(sd, timeout) < 0) return(MYDOWN_ERROR);
    return(recv(sd, (char *)data, len, 0));
}



u8 *mydown_showhttp80(u16 port) {
    static u8  mini[7];

    *mini = 0;
    if(port != 80) sprintf_s((char *)mini, 7, ":%hu", port);
    return(mini);
}



void mydown_showstatus(u32 fsize, u32 ret, u32 oldret, int timediff) {
    u32     vis;

    if(fsize) {
        vis = ((u64)ret * (u64)100) / (u64)fsize;
        fprintf(stderr, "%3u%%", (vis < 100) ? vis : 100);
    }
    fprintf(stderr, "   %12u", ret);
    if(ret > 0) {
        if(timediff) fprintf(stderr, "   %-10u", (u32)(((((u64)ret - (u64)oldret) * (u64)1000) / (u64)timediff) / 1024));
    }
    fprintf(stderr, "\r");
}



u8 *mydown_base64_encode(u8 *data, int *size) {
    int     len;
    u8      *buff,
            *p,
            a,
            b,
            c;
    static const u8 base[64] = {
        'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P',
        'Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f',
        'g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v',
        'w','x','y','z','0','1','2','3','4','5','6','7','8','9','+','/'
    };

    if(!size || (*size < 0)) {      // use size -1 for auto text size!
        len = strlen((const char *)data);
    } else {
        len = *size;
    }
    buff = (u8*)malloc(((len / 3) << 2) + 6);
    if(!buff) return(NULL);

    p = buff;
    do {
        a     = data[0];
        b     = data[1];
        c     = data[2];
        *p++  = base[(a >> 2) & 63];
        *p++  = base[(((a &  3) << 4) | ((b >> 4) & 15)) & 63];
        *p++  = base[(((b & 15) << 2) | ((c >> 6) &  3)) & 63];
        *p++  = base[c & 63];
        data += 3;
        len  -= 3;
    } while(len > 0);
    *p = 0;

    for(; len < 0; len++) *(p + len) = '=';

    if(size) *size = p - buff;
    return(buff);
}



in_addr_t mydown_resolv(char *host) {
    struct      hostent *hp;
    in_addr_t   host_ip;
    int         i;
    dns_db_t    *dns;

    host_ip = inet_addr(host);
    if(host_ip == htonl(INADDR_NONE)) {

        for(i = 0; i < dns_db_max; i++) {           // search
            if(!_stricmp((const char *)host, (const char *)dns_db[i].host)) 
            {
               return(dns_db[i].ip);
            }
        }

        hp = gethostbyname(host);
        if(!hp) {
            fprintf(stderr, "\nError: Unable to resolve hostname (%s)\n\n", host);
            return(INADDR_NONE);
        }
        host_ip = *(in_addr_t *)(hp->h_addr);

        if(!dns_db_max) memset(&dns_db, 0, sizeof(dns_db));
        if(dns_db_add == MAXDNS) dns_db_add = 0;    // add
        dns = &dns_db[dns_db_add];
        if(dns->host) free(dns->host);
        dns->host = (u8*)_strdup(host);
        dns->ip   = host_ip;
        dns_db_add++;
        if(dns_db_max < MAXDNS) dns_db_max++;
    }

    return(host_ip);
}

