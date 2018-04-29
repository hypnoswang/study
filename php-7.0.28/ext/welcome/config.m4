dnl $Id$
dnl config.m4 for extension welcome

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(welcome, for welcome support,
Make sure that the comment is aligned:
[  --with-welcome             Include welcome support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(welcome, whether to enable welcome support,
dnl Make sure that the comment is aligned:
dnl [  --enable-welcome           Enable welcome support])

if test "$PHP_WELCOME" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-welcome -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/welcome.h"  # you most likely want to change this
  dnl if test -r $PHP_WELCOME/$SEARCH_FOR; then # path given as parameter
  dnl   WELCOME_DIR=$PHP_WELCOME
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for welcome files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       WELCOME_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$WELCOME_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the welcome distribution])
  dnl fi

  dnl # --with-welcome -> add include path
  dnl PHP_ADD_INCLUDE($WELCOME_DIR/include)

  dnl # --with-welcome -> check for lib and symbol presence
  dnl LIBNAME=welcome # you may want to change this
  dnl LIBSYMBOL=welcome # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $WELCOME_DIR/$PHP_LIBDIR, WELCOME_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_WELCOMELIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong welcome lib version or lib not found])
  dnl ],[
  dnl   -L$WELCOME_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(WELCOME_SHARED_LIBADD)

  PHP_NEW_EXTENSION(welcome, welcome.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
