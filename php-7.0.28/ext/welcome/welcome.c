/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2017 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_welcome.h"

/* If you declare any globals in php_welcome.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(welcome)
*/

/* True global resources - no need for thread safety here */
static int le_welcome;

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("welcome.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_welcome_globals, welcome_globals)
    STD_PHP_INI_ENTRY("welcome.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_welcome_globals, welcome_globals)
PHP_INI_END()
*/
/* }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_welcome_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_welcome_compiled)
{
	char *arg = NULL;
	size_t arg_len, len;
	zend_string *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	strg = strpprintf(0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "welcome", arg);

	RETURN_STR(strg);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and
   unfold functions in source code. See the corresponding marks just before
   function definition, where the functions purpose is also documented. Please
   follow this convention for the convenience of others editing your code.
*/

/* {{{ proto string welcome(string str)
    */
PHP_FUNCTION(welcome)
{
	char *str = NULL;
	int argc = ZEND_NUM_ARGS();
	size_t str_len;

	if (zend_parse_parameters(argc, "s", &str, &str_len) == FAILURE) 
		return;

	size_t res_len = str_len + 11;
	char* res = (char *)emalloc(res_len + 1);
	snprintf(res, res_len, "Welcome %s!!\n", str);
	res[res_len] = '\0';

	RETURN_STRING(res);

	//php_error(E_WARNING, "welcome: not yet implemented");
}
/* }}} */

/* {{{ proto string goodbye(string str)
    */
PHP_FUNCTION(goodbye)
{
	char *str = NULL;
	int argc = ZEND_NUM_ARGS();
	size_t str_len;

	if (zend_parse_parameters(argc, "s", &str, &str_len) == FAILURE) 
		return;

	size_t res_len = str_len + 11;
	char* res = (char *)emalloc(res_len + 1);
	snprintf(res, res_len, "Goodbye %s!!\n", str);
	res[res_len] = '\0';

	RETURN_STRING(res);

	//php_error(E_WARNING, "goodbye: not yet implemented");
}
/* }}} */


/* {{{ php_welcome_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_welcome_init_globals(zend_welcome_globals *welcome_globals)
{
	welcome_globals->global_value = 0;
	welcome_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(welcome)
{
	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(welcome)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(welcome)
{
#if defined(COMPILE_DL_WELCOME) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(welcome)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(welcome)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "welcome support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ welcome_functions[]
 *
 * Every user visible function must have an entry in welcome_functions[].
 */
const zend_function_entry welcome_functions[] = {
	PHP_FE(confirm_welcome_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE(welcome,	NULL)
	PHP_FE(goodbye,	NULL)
	PHP_FE_END	/* Must be the last line in welcome_functions[] */
};
/* }}} */

/* {{{ welcome_module_entry
 */
zend_module_entry welcome_module_entry = {
	STANDARD_MODULE_HEADER,
	"welcome",
	welcome_functions,
	PHP_MINIT(welcome),
	PHP_MSHUTDOWN(welcome),
	PHP_RINIT(welcome),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(welcome),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(welcome),
	PHP_WELCOME_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_WELCOME
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(welcome)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
