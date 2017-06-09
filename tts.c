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
    char *appid;

    size_t a_len;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &appid, &a_len) == FAILURE) {
		return;
	}

    if (strlen(appid) > 0) {
        zend_update_property_string(tts_ce, getThis(), "appid", 5, appid TSRMLS_CC);
    }
}

/* }}} */

/* {{{ proto setTxt(string arg)
   设置需转换的内容 */

ZEND_METHOD(tts, setTxt)
{
    char *text;
    size_t len;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &text, &len) == FAILURE) {
        return;
    }

    zend_update_property_string(tts_ce, getThis(), "text", 4, text TSRMLS_CC);
}
/* }}} */

/* {{{ protp string getTxt
   获取转换的文字 */
ZEND_METHOD(tts, getTxt)
{
    zval *rv;
	zval *text = zend_read_property(tts_ce, getThis(), "text", 4, 1, rv);

    if (Z_TYPE_P(text) == IS_NULL) {
        RETURN_NULL();
    }

	RETURN_STR(strpprintf(0, "%s", text->value.str->val));
}
/* }}} */

/* {{{ proto setDest(arg)
   设置保存位置 */
ZEND_METHOD(tts, setDest)
{
    char *dest;
    size_t len;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &dest, &len) == FAILURE) {
        return;
    }

    zend_update_property_string(tts_ce, getThis(), "dest", 4, dest TSRMLS_CC);
}
/* }}} */

/* {{{ protp string getDest
   获取保存位置*/
ZEND_METHOD(tts, getDest)
{
    zval *rv;
    zval *dest = zend_read_property(tts_ce, getThis(), "dest", 4, 1, rv);

    if (Z_TYPE_P(dest) == IS_NULL) {
        RETURN_NULL();
    }

    RETURN_STR(strpprintf(0, "%s", dest->value.str->val));
}
/* }}} */

/* {{{ proto setVoice(arg)
   设置语音类型*/
ZEND_METHOD(tts, setVoice)
{
    char *voice;
    size_t len;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &voice, &len) == FAILURE) {
        return;
    }

    zend_update_property_string(tts_ce, getThis(), "voice", 5, voice TSRMLS_CC);
}
/* }}} */

/* {{{ proto string getVoice
   获取语音类型*/
ZEND_METHOD(tts, getVoice)
{
    zval *rv;
    zval *voice = zend_read_property(tts_ce, getThis(), "voice", 5, 1, rv);

    if (Z_TYPE_P(voice) == IS_NULL) {
        RETURN_NULL();
    }

    RETURN_STR(strpprintf(0, "%s", voice->value.str->val));
}
/* }}} */

/* {{{ protp setSPeed(arg)
   设置语速 */
ZEND_METHOD(tts, setSpeed)
{
    size_t speed;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &speed) == FAILURE) {
        return;
    }

    zend_update_property_long(tts_ce, getThis(), "speed", 5, speed);
}
/* }}} */

/* {{{ proto long getSpeed
   获取语速 */
ZEND_METHOD(tts, getSpeed)
{
    zval *rv;
    zval *speed = zend_read_property(tts_ce, getThis(), "speed", 5, 1, rv);

    if (Z_TYPE_P(speed) == IS_NULL) {
        RETURN_NULL();
    }

    RETURN_LONG(Z_LVAL_P(speed));
}
/* }}} */

/* {{{ proto setVolume(arg)
   设置音量 */
ZEND_METHOD(tts, setVolume)
{
    size_t volume;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &volume) == FAILURE) {
        return;
    }

    zend_update_property_long(tts_ce, getThis(), "volume", 7, volume);
}
/* }}} */

/* {{{ proto long getVolume
   获取音量 */
ZEND_METHOD(tts, getVolume)
{
    zval *rv;
    zval *volume = zend_read_property(tts_ce, getThis(), "volume", 7, 1, rv);

    if (Z_TYPE_P(volume) == IS_NULL) {
        RETURN_NULL();
    }

    RETURN_LONG(Z_LVAL_P(volume));
}
/* }}} */

/* {{{ proto setPitch(arg)
   设置音高 */
ZEND_METHOD(tts, setPitch)
{
    size_t pitch;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &pitch) == FAILURE) {
        return;
    }

    zend_update_property_long(tts_ce, getThis(), "pitch", 5, pitch);
}
/* }}} */

/* {{{ proto long getPitch
   获取音高 */
ZEND_METHOD(tts, getPitch)
{
    zval *rv;
    zval *pitch = zend_read_property(tts_ce, getThis(), "pitch", 5, 1, rv);

    if (Z_TYPE_P(pitch) == IS_NULL) {
        RETURN_NULL();
    }

    RETURN_LONG(Z_LVAL_P(pitch));
}
/* }}} */

/* {{{ proto setRdn(arg)
   设置数字语音合成方式 */
ZEND_METHOD(tts, setRdn)
{
    size_t rdn;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &rdn) == FAILURE) {
        return;
    }

    zend_update_property_long(tts_ce, getThis(), "rdn", 3, rdn);
}
/* }}} */

/* {{{ proto long getRdn
   获取语音合成方式*/
ZEND_METHOD(tts, getRdn)
{
    zval *rv;
    zval *rdn = zend_read_property(tts_ce, getThis(), "rdn", 3, 1, rv);

    if (Z_TYPE_P(rdn) == IS_NULL) {
        RETURN_NULL();
    }

    RETURN_LONG(Z_LVAL_P(rdn));
}
/* }}} */

ZEND_METHOD(tts, run)
{
    zval *rev;

    zval *appid;

    zval *text;
    zval *dest;
    zval *voice;

    zval *volume;
    zval *pitch;
    zval *speed;
    zval *rdn;

    appid    = zend_read_property(tts_ce, getThis(), "appid", 5, 1, rev);

    text  = zend_read_property(tts_ce, getThis(), "text", 4, 1, rev);
    dest  = zend_read_property(tts_ce, getThis(), "dest", 4, 1, rev);
    voice = zend_read_property(tts_ce, getThis(), "voice", 5, 1, rev);

    volume = zend_read_property(tts_ce, getThis(), "volume", 7, 1, rev);
    pitch  = zend_read_property(tts_ce, getThis(), "pitch", 5, 1, rev);
    speed  = zend_read_property(tts_ce, getThis(), "speed", 5, 1, rev);
    rdn    = zend_read_property(tts_ce, getThis(), "rdn", 3, 1, rev);

    convert_to_string(volume);
    convert_to_string(pitch);
    convert_to_string(speed);
    convert_to_string(rdn);

    if (Z_TYPE_P(appid)    == IS_NULL ||
        Z_TYPE_P(text)     == IS_NULL ||
        Z_TYPE_P(dest)     == IS_NULL ||
        Z_TYPE_P(volume)   == IS_NULL ||
        Z_TYPE_P(pitch)    == IS_NULL ||
        Z_TYPE_P(speed)    == IS_NULL ||
        Z_TYPE_P(rdn)      == IS_NULL ||
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
    char *session_begin_params = safe_emalloc(1, Z_STRLEN_P(voice) + Z_STRLEN_P(speed) + Z_STRLEN_P(volume) + Z_STRLEN_P(pitch) + Z_STRLEN_P(rdn) + 102, 1);
    sprintf(session_begin_params, "voice_name = %s, text_encoding = utf8, sample_rate = 16000, speed = %s, volume = %s, pitch = %s, rdn = %s",
            voice->value.str->val, Z_STRVAL_P(speed), Z_STRVAL_P(volume), Z_STRVAL_P(pitch), Z_STRVAL_P(rdn));

    char *_text = safe_emalloc(1, Z_STRLEN_P(text), 1);
    sprintf(_text, "%s", Z_STRVAL_P(text));//登录参数,appid与msc库绑定,请勿随意改动

    char *filename = safe_emalloc(1, Z_STRLEN_P(dest), 1);
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

    efree(session_begin_params);
    efree(_text);
    efree(filename);

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

    /* 注册类方法 */
    ZEND_ME(tts, __construct, NULL, ZEND_ACC_PUBLIC)
    ZEND_ME(tts, setTxt, NULL, ZEND_ACC_PUBLIC)
    ZEND_ME(tts, getTxt, NULL, ZEND_ACC_PUBLIC)
    ZEND_ME(tts, setDest, NULL, ZEND_ACC_PUBLIC)
    ZEND_ME(tts, getDest, NULL, ZEND_ACC_PUBLIC)
    ZEND_ME(tts, setVoice, NULL, ZEND_ACC_PUBLIC)
    ZEND_ME(tts, getVoice, NULL, ZEND_ACC_PUBLIC)
    ZEND_ME(tts, setSpeed, NULL, ZEND_ACC_PUBLIC)
    ZEND_ME(tts, getSpeed, NULL, ZEND_ACC_PUBLIC)
    ZEND_ME(tts, setVolume, NULL, ZEND_ACC_PUBLIC)
    ZEND_ME(tts, getVolume, NULL, ZEND_ACC_PUBLIC)
    ZEND_ME(tts, setPitch, NULL, ZEND_ACC_PUBLIC)
    ZEND_ME(tts, getPitch, NULL, ZEND_ACC_PUBLIC)
    ZEND_ME(tts, setRdn, NULL, ZEND_ACC_PUBLIC)
    ZEND_ME(tts, getRdn, NULL, ZEND_ACC_PUBLIC)
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

    /* nginx 或 apache 启动时PHP会初始化这些 */

    zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, "TTS", tts_functions);
	tts_ce = zend_register_internal_class(&ce TSRMLS_CC);

    /* 初始化一些值 */

    zend_declare_property_null(tts_ce, "appid", 5, ZEND_ACC_PRIVATE TSRMLS_DC);
    zend_declare_property_null(tts_ce, "text", 4, ZEND_ACC_PRIVATE TSRMLS_DC);
	zend_declare_property_null(tts_ce, "dest", 4, ZEND_ACC_PRIVATE TSRMLS_DC);

    /* 默认声音是xiaoyan */
	zend_declare_property_string(tts_ce, "voice", 5, "xiaoyan", ZEND_ACC_PRIVATE TSRMLS_DC);

    /* 默认语速是50 */
	zend_declare_property_long(tts_ce, "speed", 5, 50, ZEND_ACC_PRIVATE TSRMLS_DC);

    /* 默认音量是50 */
	zend_declare_property_long(tts_ce, "volume", 7, 50, ZEND_ACC_PRIVATE TSRMLS_DC);

    /* 默认音高是50 */
	zend_declare_property_long(tts_ce, "pitch", 5, 50, ZEND_ACC_PRIVATE TSRMLS_DC);

    /* 默认语音合成方式 */
	zend_declare_property_long(tts_ce, "rdn", 3, 2, ZEND_ACC_PRIVATE TSRMLS_DC);

    ///
    ///  语音合成设置的常量
    ///  用法：
    ///  $tts->setPitch(TTS::TTS_RN_AUTO_VALUE); // $tts->setPitch(0);
    ///  $tts->setPitch(TTS::TTS_RN_RN_VALUE);   // $tts->setPitch(1);
    ///  $tts->setPitch(TTS::TTS_RN_RN_DIGIT);   // $tts->setPitch(2);
    ///  $tts->setPitch(TTS::TTS_RN_AUTO_DIGIT); // $tts->setPitch(3);
    ///

    /* 自动，不确定时按照值发音*/
    zend_declare_class_constant_long(tts_ce, "TTS_RN_AUTO_VALUE ", 17, 0 TSRMLS_DC);

    /* 按照值发音 */
    zend_declare_class_constant_long(tts_ce, "TTS_RN_VALUE ", 12, 1 TSRMLS_DC);

    /* 按照 串发音*/
    zend_declare_class_constant_long(tts_ce, "TTS_RN_DIGIT ", 12, 2 TSRMLS_DC);

    /*自动，不确定时按照串发音 */
    zend_declare_class_constant_long(tts_ce, "TTS_RN_AUTO_DIGIT ", 17, 3 TSRMLS_DC);

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
	php_info_print_table_header(2, "TTS support", "enabled");
    php_info_print_table_row(2, "Default voice name", "xiaoyan");
    php_info_print_table_row(2, "Text encoding", "utf8");
    php_info_print_table_row(2, "Sample rate", "16000");
    php_info_print_table_row(2, "Default Speed", "50");
    php_info_print_table_row(2, "Volume", "50");
    php_info_print_table_row(2, "Pitch", "50");
    php_info_print_table_row(2, "Rdn", "2");
	php_info_print_table_header(2, "Version", "1.0.0");
	php_info_print_table_header(2, "Author", "renshan <1005110700@qq.com>");
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
