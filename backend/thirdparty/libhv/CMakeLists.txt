set(BASE_HEADERS
    libhv/base
)

set(SSL_HEADERS
    libhv/ssl
)

set(EVENT_HEADERS
    libhv/event
)

set(UTIL_HEADERS
    libhv/util
)

set(CPPUTIL_HEADERS
    libhv/cpputil
)

set(EVPP_HEADERS
    libhv/evpp
)

set(PROTOCOL_HEADERS
    libhv/protocol
)

set(HTTP_HEADERS
    libhv/http
)

set(HTTP_CLIENT_HEADERS
    libhv/http/client
    )

set(HTTP_SERVER_HEADERS
    libhv/http/server
)

set(MQTT_HEADERS
    libhv/mqtt
)
set(BASE_SRCS
    libhv/base/hbase.c
    libhv/base/herr.c
    libhv/base/hlog.c
    libhv/base/hmain.c
    libhv/base/hsocket.c
    libhv/base/htime.c
    libhv/base/hversion.c
    libhv/base/rbtree.c
)

set(SSL_SRCS
    libhv/ssl/appletls.c
    libhv/ssl/gnutls.c
    libhv/ssl/hssl.c
    libhv/ssl/mbedtls.c
    libhv/ssl/nossl.c
    libhv/ssl/openssl.c
    libhv/ssl/wintls.c
)

set(EVENT_SRCS
    libhv/event/evport.c
    libhv/event/kqueue.c
    libhv/event/nlog.c
    libhv/event/noevent.c
    libhv/event/rudp.c
    libhv/event/select.c
    libhv/event/epoll.c
    libhv/event/hevent.c
    libhv/event/hloop.c
    libhv/event/iocp.c
    libhv/event/nio.c
    libhv/event/overlapio.c
    libhv/event/unpack.c
)

set(UTIL_SRCS
    libhv/util/base64.c
    libhv/util/md5.c
    libhv/util/sha1.c
    libhv/util/sha1.c
)

set(CPPUTIL_SRCS
    libhv/cpputil/RAII.cpp
    libhv/cpputil/hdir.cpp
    libhv/cpputil/iniparser.cpp
    libhv/cpputil/hasync.cpp
    libhv/cpputil/ifconfig.cpp
    libhv/cpputil/ThreadLocalStorage.cpp
    libhv/cpputil/hpath.cpp
    libhv/cpputil/hstring.cpp
    libhv/cpputil/hurl.cpp
)
set(EVPP_SRCS

)
set(HTTP_SRCS
    libhv/http/Http1Parser.cpp
    libhv/http/HttpMessage.cpp
    libhv/http/HttpParser.cpp
    libhv/http/WebSocketChannel.cpp
    libhv/http/WebSocketParser.cpp
    libhv/http/http_content.cpp
    libhv/http/http_parser.c
    libhv/http/httpdef.c
    libhv/http/multipart_parser.c
    libhv/http/websocket_parser.c
    libhv/http/wsdef.c
)

set(HTTP_SERVER_SRCS
    libhv/http/server/FileCache.cpp
    libhv/http/server/HttpHandler.cpp
    libhv/http/server/HttpMiddleware.cpp
    libhv/http/server/HttpResponseWriter.cpp
    libhv/http/server/HttpServer.cpp
    libhv/http/server/HttpService.cpp
    libhv/http/server/http_page.cpp
)
set(HTTP_CLIENT_SRCS
    libhv/http/client/AsyncHttpClient.cpp
    libhv/http/client/HttpClient.cpp
)
set(LIBHV_SRCDIRS   ${BASE_SRCS}
                    ${SSL_SRCS}
                    ${EVENT_SRCS}
                    ${UTIL_SRCS}
)
 
set(LIBHV_HEADERS   libhv 
                    ${BASE_HEADERS} 
                    ${SSL_HEADERS} 
                    ${EVENT_HEADERS} 
                    ${UTIL_HEADERS}
)

option(WITH_EVPP "compile evpp" ON)
option(WITH_HTTP "compile http" ON)
option(WITH_HTTP_SERVER "compile http/server" ON)
option(WITH_HTTP_CLIENT "compile http/client" ON)
option(WITH_MQTT "compile mqtt" OFF)

option(ENABLE_UDS "Unix Domain Socket" OFF)
option(USE_MULTIMAP "MultiMap" OFF)

option(WITH_CURL "with curl library (deprecated)" OFF)
option(WITH_NGHTTP2 "with nghttp2 library" OFF)

option(WITH_OPENSSL "with openssl library" OFF)
option(WITH_GNUTLS  "with gnutls library"  OFF)
option(WITH_MBEDTLS "with mbedtls library" OFF)

option(WITH_KCP "compile event/kcp" OFF)

if(WITH_EVPP)
    set(LIBHV_HEADERS ${LIBHV_HEADERS} ${CPPUTIL_HEADERS} ${EVPP_HEADERS})
    set(LIBHV_SRCDIRS ${LIBHV_SRCDIRS} ${CPPUTIL_SRCS} ${EVPP_SRCS})
    if(WITH_HTTP)
        set(LIBHV_HEADERS ${LIBHV_HEADERS} ${HTTP_HEADERS})
        set(LIBHV_SRCDIRS ${LIBHV_SRCDIRS} ${HTTP_SRCS})
        if(WITH_HTTP_SERVER)
            set(LIBHV_HEADERS ${LIBHV_HEADERS} ${HTTP_SERVER_HEADERS})
            set(LIBHV_SRCDIRS ${LIBHV_SRCDIRS} ${HTTP_SERVER_SRCS})
        endif()
        if(WITH_HTTP_CLIENT)
            set(LIBHV_HEADERS ${LIBHV_HEADERS} ${HTTP_CLIENT_HEADERS})
            set(LIBHV_SRCDIRS ${LIBHV_SRCDIRS} ${HTTP_CLIENT_SRCS})
        endif()
    endif()
endif()


set(MY_LIST_WITH_PREFIX "")

foreach(item IN LISTS LIBHV_HEADERS)
    set(prefixed_item "${CMAKE_CURRENT_SOURCE_DIR}/${item}")
    message(${prefixed_item})
    list(APPEND MY_LIST_WITH_PREFIX "${prefixed_item}")
endforeach()

set(PROJECT_INCLUDE_DIRS ${MY_LIST_WITH_PREFIX} PARENT_SCOPE)
add_library(libhv STATIC ${LIBHV_SRCDIRS})
target_include_directories(libhv PUBLIC ${PROJECT_INCLUDE_DIRS})