#ifndef H2D_MAIN_H
#define H2D_MAIN_H

/* include common headers here */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include "libwuya/wuy_array.h"
#include "libwuya/wuy_dict.h"
#include "libwuya/wuy_list.h"
#include "libwuya/wuy_pool.h"
#include "libwuya/wuy_cflua.h"
#include "libwuya/wuy_tcp.h"
#include "libwuya/wuy_sockaddr.h"
#include "libwuya/wuy_http.h"

#include "libloop/loop.h"

#include "libhttp2/http2.h"

#include "h2d_module.h"
#include "h2d_conf.h"
#include "h2d_connection.h"
#include "h2d_request.h"
#include "h2d_http1.h"
#include "h2d_http2.h"
#include "h2d_header.h"
//#include "h2d_lua_api.h"
//#include "h2d_lua_thread.h"
#include "h2d_ssl.h"
#include "h2d_upstream.h"

/* return values */
#define H2D_OK			0
#define H2D_ERROR		-1
#define H2D_AGAIN		-2

/* global event-driven loop */
extern loop_t *h2d_loop;

#endif
