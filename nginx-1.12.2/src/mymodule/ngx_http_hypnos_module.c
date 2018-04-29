#include "ngx_http_hypnos_module.h"
#include "ngx_string.h"
#include "ngx_list.h"
#include "ngx_hash.h"


char* ngx_http_hypnos_string(ngx_conf_t *cf, ngx_command_t *cmd, void *conf) {
    ngx_http_hypnos_loc_conf_t* loc_conf = conf;

    char* rv = ngx_conf_set_str_slot(cf, cmd, conf);
    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
            "secret:%s", loc_conf->secret.data);

    return rv;
}

ngx_int_t ngx_http_hypnos_init(ngx_conf_t *cf) {
    ngx_http_handler_pt *h;
    ngx_http_core_main_conf_t  *cmcf;
    cmcf = ngx_http_conf_get_module_main_conf(cf, ngx_http_core_module);

    /*
     * ngx_array_push(ngx_array_t *a)
     * 函数表示在数组a上新追加一个元素
     * 并返回指向新元素的指针。
     * */
    h = ngx_array_push(&cmcf->phases[NGX_HTTP_ACCESS_PHASE].handlers);
    if (h == NULL) {
        return NGX_ERROR;
    }
    *h = ngx_http_hypnos_handler;

    return NGX_OK;
}

void* ngx_http_hypnos_create_loc_conf(ngx_conf_t *cf) {
    ngx_http_hypnos_loc_conf_t* loc_conf = NULL;

    loc_conf = ngx_pcalloc(cf->pool, sizeof(ngx_http_hypnos_loc_conf_t));
    if (loc_conf == NULL) {
        return NULL;
    }

    ngx_str_null(&loc_conf->secret);

    return loc_conf;
}


ngx_int_t ngx_http_hypnos_handler(ngx_http_request_t *r) {
    ngx_http_hypnos_loc_conf_t* my_conf;

    ngx_log_error(NGX_LOG_EMERG, r->connection->log,
            0, "ngx_http_hypnos_handler is called!");

    /* 获取指令配置 */
    my_conf = ngx_http_get_module_loc_conf(r, ngx_http_hypnos_module);
    if (my_conf->secret.len == 0) {
        ngx_log_error(NGX_LOG_EMERG, r->connection->log,
                0, "secret is empty!");

        return NGX_DECLINED;
    }

    ngx_http_headers_in_t* headers = &r->headers_in;
    ngx_list_part_t* part = &(headers->headers.part);
    ngx_table_elt_t* eh = (ngx_table_elt_t*)(part->elts);
    ngx_int_t ok = 0;
    for (ngx_uint_t i = 0; ;i ++ ) {
        if (i >= part->nelts) {
            part = part->next;
            if (part == NULL) break;
            eh = (ngx_table_elt_t*)(part->elts);
            i = 0;
        }

        if (eh[i].hash == 0) continue;

        if (ngx_strcasecmp((u_char*)"Hypnos",
                    eh[i].key.data) == 0) {
            if (ngx_strcasecmp(my_conf->secret.data,
                        eh[i].value.data) == 0) {
                ok = 1;
            } 

            break;
        }

    }

    if (!ok) {
        return NGX_HTTP_FORBIDDEN;
    }

    ngx_str_t key = {7, (u_char *)"Hypnos"};
    ngx_str_t val = {8, (u_char *)"Welcome"};
    ngx_table_elt_t* rh = ngx_list_push(&r->headers_out.headers);
    rh->hash = 1;
    rh->key = key;
    rh->value = val;

    return NGX_DECLINED;
}
