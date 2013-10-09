#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)

    
AC_INIT(libpal, 0.2.0, maxence.laurent@heig-vd.ch)
AC_CONFIG_SRCDIR([configure.ac])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([ \
        Makefile \
        src/Makefile \
        src/pal/Makefile \
        src/jpal/Makefile \
        doc/Makefile \
        doc/Doxyfile \
        tests/Makefile \
        tests/problems/Makefile \
        tests/problems/DenseMap/Makefile \
        tests/problems/DenseRect/Makefile \
        tests/problems/HardGrid/Makefile \
        tests/problems/MunichDrillholes/Makefile \
        tests/problems/RailwayStation/Makefile \
        tests/problems/RandomMap/Makefile \
        tests/problems/RandomRect/Makefile \
        tests/problems/RegularGrid/Makefile \
        tests/problems/SwissCities/Makefile \
        tests/problems/SwissCities/p4/Makefile \
        tests/problems/SwissCities/p8/Makefile \
        tests/problems/VariableDensity/Makefile \
        libpal.pc \
     ])

VERSION_MAJOR=0
VERSION_MINOR=2
VERSION_PATCH=0
VERSION="$VERSION_MAJOR.$VERSION_MINOR.$VERSION_PATCH"

LIBPAL_VERSION_NUM="$VERSION_MAJOR$VERSION_MINOR$VERSION_PATCH"
AC_SUBST(LIBPAL_VERSION_NUM)

AM_INIT_AUTOMAKE([dist-bzip2])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_LIBTOOL_DLOPEN
AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL

dnl Process custom command-line options
AC_ARG_ENABLE([svgmap],
             AS_HELP_STRING([--enable-svgmap],
                            [pal will create a pal-map.svg in the current directory (tesing purpose)]),
             AC_DEFINE_UNQUOTED([_EXPORT_MAP_],[1],[Add svg map generation]))

AC_ARG_ENABLE([test],
             AS_HELP_STRING([--enable-test],
                            [build a test programm]),
             TESTING=yes, TESTING=no)

AM_CONDITIONAL(ENABLE_TEST, [test "$TESTING" == yes])


dnl Process custom command-line options
AC_ARG_ENABLE([loglevel],
             AS_HELP_STRING([--enable-loglevel=LVL],
                            [set default log level to one of none, info, ]
                            [debug or full (default=info)]),
             default_loglevel=$enableval, default_loglevel=info)

if test x$default_loglevel = xdebug; then
   AC_DEFINE_UNQUOTED([_VERBOSE_],[1],[Set pal verbose])
   AC_DEFINE_UNQUOTED([_DEBUG_],[1],[ add debug information])
elif test x$default_loglevel = xinfo; then
   AC_DEFINE_UNQUOTED([_VERBOSE_],[1],[Set pal verbose])
elif test x$default_loglevel = xfull; then
   AC_DEFINE_UNQUOTED([_VERBOSE_],[1],[Set pal verbose])
   AC_DEFINE_UNQUOTED([_DEBUG_],[1],[ add debug information])
   AC_DEFINE_UNQUOTED([_DEBUG_FULL_],[1],[ add all debug information (a lot)])
elif test x$default_loglevel != xnone; then
   AC_MSG_WARN([invalid loglevel, using default (verbose)])
   AC_DEFINE_UNQUOTED([_VERBOSE_],[1],[Set pal verbose])
fi

AC_ARG_WITH([java],
              [AS_HELP_STRING([--with-java=<path>],
                             [build JNI wrap against JDK at <path>])],
              JDK_PREFIX=$with_java, JDK_PREFIX=no)


AM_CONDITIONAL(ENABLE_JAVA, [test "$JDK_PREFIX" != no])

if test x${JDK_PREFIX} != xno; then
   if test -e ${JDK_PREFIX}/include/linux; then
      JAVA_CFLAGS="-I${JDK_PREFIX}/include -I${JDK_PREFIX}/include/linux"
   elif test -e ${JDK_PREFIX}/include/win32; then
      JAVA_CFLAGS="-I${JDK_PREFIX}/include -I${JDK_PREFIX}/include/win32"
   elif test -e ${JDK_PREFIX}/include/; then
      JAVA_CFLAGS="-I${JDK_PREFIX}/include"
   else
      AC_MSG_ERROR([Unable to find ${JDK_PREFIX}/include !])
   fi
else
   JAVA_CFLAGS=""
fi

AC_SUBST(JAVA_CFLAGS)
dnl Process custom command-line options

AC_ARG_WITH([geosconfig],
              [AS_HELP_STRING([--with-geosconfig=path>],
                             [path to geos-config])],
              GEOS_CONFIG=$with_geosconfig, GEOS_CONFIG=no)

#AC_MSG_WARN([GEOS CONFIG : ${GEOS_CONFIG}])

GEOSCONFIG="${GEOS_CONFIG}/geos-config"

if test x${GEOS_CONFIG} = xno; then
   AC_PATH_PROG(GEOSCONFIG, geos-config, [no])
   GEOS_CONFIG="ok"
elif test ! -e ${GEOSCONFIG}; then
   GEOS_CONFIG="no"
fi

if test "x$GEOS_CONFIG" = xno; then
   AC_MSG_ERROR([GEOS is requiered (try --with-geosconfig)])
fi

GEOSVERSION=`$GEOSCONFIG --version`

case ${GEOSVERSION} in
 1* | 2* )
  AC_MSG_ERROR([geos-config reports version $GEOSVERSION, need at least 3.0])
 ;;
 *) ;; 
esac


GEOS_CFLAGS=`$GEOSCONFIG --cflags`
GEOS_LIBS=`$GEOSCONFIG --prefix`
GEOS_LIBS="-L${GEOS_LIBS}/lib -lgeos_c"

AC_SUBST(GEOS_LIBS)
AC_SUBST(GEOS_CFLAGS)


# Checks for libraries.
AC_CHECK_LIB([m], [sin])

AC_CHECK_LIB(pthread, pthread_mutex_init, 
             THREAD_LIBS="-lpthread", THREAD_LIBS="none")
             
WINLDFLAGS=""

if test x${THREAD_LIBS} = xnone; then
   OLD_LIBS=${LIBS}
   LIBS="$LIBS -pthread" 
   AC_CHECK_FUNC(pthread_mutex_init, THREAD_LIBS="-pthread")

   LIBS=${OLD_LIBS}

   if test x${THREAD_LIBS} = xnone; then
      AC_CHECK_HEADERS([windows.h], THREAD_LIBS="WINDOWS",
                       AC_MSG_ERROR([pthread library (or windows.h) needed!]))
   else
      AC_DEFINE_UNQUOTED([_HAVE_PTHREAD_],[1],[Use posix-threads])
   fi 

   if test x${THREAD_LIBS} = xWINDOWS; then
     THREAD_LIBS=""
     AC_DEFINE_UNQUOTED([_HAVE_WINDOWS_H_],[1],[Use threads from windows.h])
     WINLDFLAGS="-Wl,--kill-at -no-undefined"
   fi
else
   AC_DEFINE_UNQUOTED([_HAVE_PTHREAD_],[1],[Use posix-threads])
fi


AC_SUBST(THREAD_LIBS)
AC_SUBST(WINLDFLAGS)




# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_HEADER_STDC
AC_HEADER_TIME
AC_CHECK_FUNCS([memset pow sqrt])

WARN_CFLAGS="-Wall -Wextra -Wunused-function"

CFLAGS="$CFLAGS -g -O3 -fPIC $WARN_CFLAGS"

AC_SUBST(VERSION)
AC_SUBST(VERSION_MAJOR)
AC_SUBST(VERSION_MINOR)
AC_SUBST(VERSION_PATCH)

AC_OUTPUT
