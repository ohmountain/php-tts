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
#include "php_tts.h"

/* If you declare any globals in php_tts.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(tts)
*/

/* True global resources - no need for thread safety here */
static int le_tts;

zend_class_entry *tts_ce;

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("tts.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_tts_globals, tts_globals)
    STD_PHP_INI_ENTRY("tts.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_tts_globals, tts_globals)
PHP_INI_END()
*/
/* }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_tts_compiled(string arg)
   Return a string to confirm that the module is compiled in */

ZEND_METHOD(tts, __construct)
{
    char *username, *password, *appid;

    size_t u_len, p_len, a_len;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "sss", &username, &u_len, &password, &p_len, &appid, &a_len) == FAILURE) {
		return;
	}

    if (strlen(username) > 0) {
        zend_update_property_string(tts_ce, getThis(), "username", 8, username TSRMLS_CC);
    }


    if (strlen(password) > 0) {
        zend_update_property_string(tts_ce, getThis(), "password", 8, password TSRMLS_CC);
    }

    if (strlen(appid) > 0) {
        zend_update_property_string(tts_ce, getThis(), "appid", 5, appid TSRMLS_CC);
    }
}

ZEND_METHOD(tts, setTxt)
{
    char *text;
    size_t len;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &text, &len) == FAILURE) {
        return;
    }

    zend_update_property_string(tts_ce, getThis(), "text", 4, text TSRMLS_CC);
}


ZEND_METHOD(tts, getTxt)
{
    zval *rv;
	zval *text = zend_read_property(tts_ce, getThis(), "text", 4, 1, rv);

    if (Z_TYPE_P(text) == IS_NULL) {
        RETURN_NULL();
    }

	RETURN_STR(strpprintf(0, "%s", text->value.str->val));
}


ZEND_METHOD(tts, setDest)
{
    char *dest;
    size_t len;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &dest, &len) == FAILURE) {
        return;
    }

    zend_update_property_string(tts_ce, getThis(), "dest", 4, dest TSRMLS_CC);
}

ZEND_METHOD(tts, getDest)
{
    zval *rv;
    zval *dest = zend_read_property(tts_ce, getThis(), "dest", 4, 1, rv);

    if (Z_TYPE_P(dest) == IS_NULL) {
        RETURN_NULL();
    }

    RETURN_STR(strpprintf(0, "%s", dest->value.str->val));
}

ZEND_METHOD(tts, setVoice)
{
    char *voice;
    size_t len;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &voice, &len) == FAILURE) {
        return;
    }

    zend_update_property_string(tts_ce, getThis(), "voice", 5, voice TSRMLS_CC);
}

ZEND_METHOD(tts, getVoice)
{
    zval *rv;
    zval *voice = zend_read_property(tts_ce, getThis(), "voice", 5, 1, rv);

    if (Z_TYPE_P(voice) == IS_NULL) {
        RETURN_NULL();
    }

    RETURN_STR(strpprintf(0, "%s", voice->value.str->val));
}

ZEND_METHOD(tts, run)
{
    zval *rev;

    zval *username;
    zval *password;
    zval *appid;

    zval *text;
    zval *dest;
    zval *voice;

    username = zend_read_property(tts_ce, getThis(), "username", 8, 1, rev);
    password = zend_read_property(tts_ce, getThis(), "password", 8, 1, rev);
    appid    = zend_read_property(tts_ce, getThis(), "appid", 5, 1, rev);

    text  = zend_read_property(tts_ce, getThis(), "text", 4, 1, rev);
    dest  = zend_read_property(tts_ce, getThis(), "dest", 4, 1, rev);
    voice = zend_read_property(tts_ce, getThis(), "voice", 5, 1, rev);

    if (Z_TYPE_P(username) == IS_NULL ||
        Z_TYPE_P(password) == IS_NULL ||
        Z_TYPE_P(appid)    == IS_NULL ||
        Z_TYPE_P(text)     == IS_NULL ||
        Z_TYPE_P(dest)     == IS_NULL ||
        Z_TYPE_P(voice)    == IS_NULL) {
        RETURN_LONG(-1);
    }

    int         ret                  = MSP_SUCCESS;
    /* const char* login_params         = "appid = 59351e35, work_dir = .";//登录参数,appid与msc库绑定,请勿随意改动 *\/ */
    char login_params[Z_STRLEN_P(appid) + 22];//         = "appid = 59351e35, work_dir = .";//登录参数,appid与msc库绑定,请勿随意改动 */
    sprintf(login_params, "appid = %s, work_dir = .", Z_STRVAL_P(appid));//登录参数,appid与msc库绑定,请勿随意改动

    /*
     * rdn:           合成音频数字发音方式
     * volume:        合成音频的音量
     * pitch:         合成音频的音调
     * speed:         合成音频对应的语速
     * voice_name:    合成发音人
     * sample_rate:   合成音频采样率
     * text_encoding: 合成文本编码格式
     *
     * 详细参数说明请参阅《讯飞语音云MSC--API文档》
     */
    /* const char* session_begin_params = "voice_name = xiaoyan, text_encoding = utf8, sample_rate = 16000, speed = 50, volume = 50, pitch = 50, rdn = 2"; */
    char session_begin_params[Z_STRLEN_P(voice) + 102];
    sprintf(session_begin_params, "voice_name = %s, text_encoding = utf8, sample_rate = 16000, speed = 50, volume = 50, pitch = 50, rdn = 2", voice->value.str->val);

    char _text[Z_STRLEN_P(text)];//                 = strpprintf(0, "%s", text->value.str->val);
    //sprintf(_text, "%s", Z_STRVAL_P(text));
    sprintf(_text, "%s", Z_STRVAL_P(text));//登录参数,appid与msc库绑定,请勿随意改动

    /* const char* filename             = "tts_sample.wav"; //合成的语音文件名称 */
    char filename[Z_STRLEN_P(dest)];
    sprintf(filename, "%s", Z_STRVAL_P(dest));

    /* 用户登录 */
    ret = MSPLogin(NULL, NULL, login_params);//第一个参数是用户名，第二个参数是密码，第三个参数是登录参数，用户名和密码可在http://www.xfyun.cn注册获取
    if (MSP_SUCCESS != ret)
        {
            MSPLogout(); //退出登录
            RETURN_LONG(ret);
        }

    ret = text_to_speech(_text, filename, session_begin_params);
	if (MSP_SUCCESS != ret)
        {
            MSPLogout(); //退出登录
            RETURN_LONG(ret);
        }


	MSPLogout(); //退出登录
	RETURN_LONG(ret);
}

PHP_FUNCTION(tts)
{
    int         ret                  = MSP_SUCCESS;
    const char* login_params         = "appid = 59351e35, work_dir = .";//登录参数,appid与msc库绑定,请勿随意改动
    /*
     * rdn:           合成音频数字发音方式
     * volume:        合成音频的音量
     * pitch:         合成音频的音调
     * speed:         合成音频对应的语速
     * voice_name:    合成发音人
     * sample_rate:   合成音频采样率
     * text_encoding: 合成文本编码格式
     *
     * 详细参数说明请参阅《讯飞语音云MSC--API文档》
     */
    const char* session_begin_params = "voice_name = xiaoyan, text_encoding = utf8, sample_rate = 16000, speed = 50, volume = 50, pitch = 50, rdn = 2";
    const char* filename             = "tts_sample.wav"; //合成的语音文件名称
    const char* text                 = "你想讲一个故事吗？好啊，那就讲故事啊。从前有座山，山里有个庙，庙里有个老和尚。故事已经讲完"; //合成文本

    /* 用户登录 */
    ret = MSPLogin(NULL, NULL, login_params);//第一个参数是用户名，第二个参数是密码，第三个参数是登录参数，用户名和密码可在http://www.xfyun.cn注册获取
    if (MSP_SUCCESS != ret)
        {
            MSPLogout(); //退出登录
            RETURN_LONG(ret);
        }

    ret = text_to_speech(text, filename, session_begin_params);
	if (MSP_SUCCESS != ret)
        {
            MSPLogout(); //退出登录
            RETURN_LONG(ret);
        }


	MSPLogout(); //退出登录
	RETURN_LONG(ret);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and
   unfold functions in source code. See the corresponding marks just before
   function definition, where the functions purpose is also documented. Please
   follow this convention for the convenience of others editing your code.
*/


/* {{{ php_tts_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_tts_init_globals(zend_tts_globals *tts_globals)
{
	tts_globals->global_value = 0;
	tts_globals->global_string = NULL;
}
*/
/* }}} */

 /* {{{ tts_functions[]
  *
  * Every user visible function must have an entry in tts_functions[].
  */
const zend_function_entry tts_functions[] = {
	//PHP_FE(tts,	NULL)		/* For testing, remove later. */
	//PHP_FE_END	/* Must be the last line in tts_functions[] */

    ZEND_ME(tts, __construct, NULL, ZEND_ACC_PUBLIC)
    ZEND_ME(tts, setTxt, NULL, ZEND_ACC_PUBLIC)
    ZEND_ME(tts, getTxt, NULL, ZEND_ACC_PUBLIC)
    ZEND_ME(tts, setDest, NULL, ZEND_ACC_PUBLIC)
    ZEND_ME(tts, getDest, NULL, ZEND_ACC_PUBLIC)
    ZEND_ME(tts, setVoice, NULL, ZEND_ACC_PUBLIC)
    ZEND_ME(tts, getVoice, NULL, ZEND_ACC_PUBLIC)
    ZEND_ME(tts, run, NULL, ZEND_ACC_PUBLIC)

    ZEND_FE_END
};
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(tts)
{
	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/

    zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, "TTS", tts_functions);
	tts_ce = zend_register_internal_class(&ce TSRMLS_CC);

    /* 初始化一些值 */

    zend_declare_property_null(tts_ce, "username", 8, ZEND_ACC_PRIVATE TSRMLS_DC);
    zend_declare_property_null(tts_ce, "password", 8, ZEND_ACC_PRIVATE TSRMLS_DC);
    zend_declare_property_null(tts_ce, "appid", 5, ZEND_ACC_PRIVATE TSRMLS_DC);
    zend_declare_property_null(tts_ce, "text", 4, ZEND_ACC_PRIVATE TSRMLS_DC);
	zend_declare_property_null(tts_ce, "dest", 4, ZEND_ACC_PRIVATE TSRMLS_DC);

    /* 默认声音是xiaoyan */
	zend_declare_property_string(tts_ce, "voice", 5, "xiaoyan", ZEND_ACC_PRIVATE TSRMLS_DC);

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(tts)
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
PHP_RINIT_FUNCTION(tts)
{
#if defined(COMPILE_DL_TTS) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(tts)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(tts)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "tts support", "enabled");
	php_info_print_table_header(2, "author", "renshan <1005110700@qq.com>");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ tts_module_entry
 */
zend_module_entry tts_module_entry = {
	STANDARD_MODULE_HEADER,
	"tts",
	tts_functions,
	PHP_MINIT(tts),
	PHP_MSHUTDOWN(tts),
	PHP_RINIT(tts),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(tts),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(tts),
	PHP_TTS_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_TTS
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(tts)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
