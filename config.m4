dnl $Id$
dnl config.m4 for extension tts

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(tts, for tts support,
dnl Make sure that the comment is aligned:
dnl [  --with-tts             Include tts support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(tts, whether to enable tts support,
dnl Make sure that the comment is aligned:
dnl [  --enable-tts           Enable tts support])

if test "$PHP_TTS" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-tts -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/tts.h"  # you most likely want to change this
  dnl if test -r $PHP_TTS/$SEARCH_FOR; then # path given as parameter
  dnl   TTS_DIR=$PHP_TTS
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for tts files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       TTS_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$TTS_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the tts distribution])
  dnl fi

  dnl # --with-tts -> add include path
  dnl PHP_ADD_INCLUDE($TTS_DIR/include)

  dnl # --with-tts -> check for lib and symbol presence
  dnl LIBNAME=tts # you may want to change this
  dnl LIBSYMBOL=tts # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $TTS_DIR/$PHP_LIBDIR, TTS_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_TTSLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong tts lib version or lib not found])
  dnl ],[
  dnl   -L$TTS_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(TTS_SHARED_LIBADD)

  PHP_NEW_EXTENSION(tts, tts.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
