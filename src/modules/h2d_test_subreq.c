#include "h2d_main.h"

struct h2d_test_subreq_conf {
	bool	enable;
};

extern struct h2d_module h2d_test_subreq_module;

static int h2d_test_subreq_filter_response_headers(struct h2d_request *r)
{
	struct h2d_test_subreq_conf *conf = r->conf_path->module_confs[h2d_test_subreq_module.index];
	if (!conf->enable) {
		return H2D_OK;
	}

	r->resp.content_length = H2D_CONTENT_LENGTH_CHUNKED;
	return H2D_OK;
}
static int h2d_test_subreq_filter_response_body(struct h2d_request *r, uint8_t *buffer, int buf_len)
{
	struct h2d_test_subreq_conf *conf = r->conf_path->module_confs[h2d_test_subreq_module.index];
	if (!conf->enable) {
		return buf_len;
	}

	struct h2d_request *subr = r->module_ctxs[h2d_test_subreq_module.request_ctx.index];
	if (subr == NULL) {
		subr = h2d_request_subreq_new(r);
		subr->req.url = subr->req.buffer;
		subr->req.next = h2d_header_add(subr->req.next, ":url", 4, (char *)buffer, buf_len-1);
		printf("subr: %s\n", h2d_header_value(subr->req.url));

		r->module_ctxs[h2d_test_subreq_module.request_ctx.index] = subr;
		return H2D_AGAIN;
	} else {
		int data_len = subr->c->send_buf_pos - subr->c->send_buffer;
		memcpy(buffer, subr->c->send_buffer, data_len);
		subr->c->send_buf_pos = subr->c->send_buffer;
		return data_len;
	}
}

/* configuration */

static void h2d_test_subreq_ctx_free(struct h2d_request *r)
{
}

static struct wuy_cflua_command h2d_test_subreq_conf_commands[] = {
	{	.type = WUY_CFLUA_TYPE_BOOLEAN,
		.offset = offsetof(struct h2d_test_subreq_conf, enable),
		.flags = WUY_CFLUA_FLAG_UNIQ_MEMBER,
	},
	{ NULL }
};

struct h2d_module h2d_test_subreq_module = {
	.name = "test_subreq",
	.command_path = {
		.name = "test_subreq",
		.type = WUY_CFLUA_TYPE_TABLE,
		.u.table = &(struct wuy_cflua_table) {
			.commands = h2d_test_subreq_conf_commands,
			.size = sizeof(struct h2d_test_subreq_conf),
		}
	},

	.request_ctx = {
		.free = h2d_test_subreq_ctx_free,
	},

	.filters = {
		.response_headers = h2d_test_subreq_filter_response_headers,
		.response_body = h2d_test_subreq_filter_response_body,
	},
};
