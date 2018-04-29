#ifndef NGX_HTTP_HYPNOS_MODULE_H
#define NGX_HTTP_HYPNOS_MODULE_H

#include "ngx_config.h"
#include "ngx_core.h"
#include "ngx_http.h"

typedef struct {
    ngx_str_t secret;
} ngx_http_hypnos_loc_conf_t;

char* ngx_http_hypnos_string(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

ngx_command_t ngx_http_hypnos_commands[] = {
    {
        ngx_string("secret"),
        /* The namespace of this directive, and it has only one argument */
        NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1,
        /* callback for parse the argument */
        ngx_http_hypnos_string,
        /* The store posistion of thie directive */
        NGX_HTTP_LOC_CONF_OFFSET,
        /* 存储的具体结构及位置*/
        offsetof(ngx_http_hypnos_loc_conf_t, secret),
        NULL
    },
    /* The end flag */
    ngx_null_command
};


ngx_int_t ngx_http_hypnos_init(ngx_conf_t *cf);
void* ngx_http_hypnos_create_loc_conf(ngx_conf_t *cf);
/**
 * 该指令上下文的定义。
 * ngx_http_hypnos_init：这个回调函数负责在读取完配置信息之后挂载此模块
 * ngx_http_hypnos_create_loc_conf：此回调函数负责在读取location配置之后，创建模块配置结构的存储空间
 **/
ngx_http_module_t ngx_http_hypnos_module_ctx = {
    NULL,                           /* preconfiguration 在创建和读取该模块的配置信息之前被调用*/
    ngx_http_hypnos_init,            /* postconfiguration 在创建和读取该模块的配置信息之后被调用*/
    NULL,                           /* create main configuration */
    NULL,                           /* init main configuration */
    NULL,                           /* create server configuration */
    NULL,                           /* merge server configuration */
    ngx_http_hypnos_create_loc_conf, /* create location configuration */
    NULL                            /* merge location configuration */
};


/** 模块的定义 */
ngx_module_t ngx_http_hypnos_module = {
    NGX_MODULE_V1,
    &ngx_http_hypnos_module_ctx,   /* module context */
    ngx_http_hypnos_commands,      /* module directives */
    NGX_HTTP_MODULE,               /* module type */
    NULL,                          /* init master */
    NULL,                          /* init module */
    NULL,                          /* init process */
    NULL,                          /* init thread */
    NULL,                          /* exit thread */
    NULL,                          /* exit process */
    NULL,                          /* exit master */
    NGX_MODULE_V1_PADDING
};

ngx_int_t ngx_http_hypnos_handler(ngx_http_request_t *r);


#endif
