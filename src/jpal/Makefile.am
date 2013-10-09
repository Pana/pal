#SUBDIRS = ch

if ENABLE_JAVA

lib_LTLIBRARIES = libjpal.la

INCLUDES = -I@top_srcdir@/src/includes -I@top_srcdir@/src/pal/ ${GEOS_CFLAGS} ${JAVA_CFLAGS}


libjpal_la_SOURCES = jpal.cpp \
                     jpallayer.cpp \
                     palgeometrywrapper.cpp \
	 				 jpal.h \
				 	 jpallayer.h \
					 palgeometrywrapper.h \
					 ../pal/feature.cpp \
					 ../pal/geomfunction.cpp \
					 ../pal/label.cpp \
					 ../pal/labelposition.cpp \
					 ../pal/layer.cpp \
					 ../pal/pal.cpp \
					 ../pal/palstat.cpp \
					 ../pal/pointset.cpp \
					 ../pal/priorityqueue.cpp \
					 ../pal/problem.cpp \
					 ../pal/util.cpp \
					 ../pal/rtree.hpp \
					 ../pal/linkedlist.hpp \
					 ../pal/hashtable.hpp \
					 ../pal/feature.h \
					 ../pal/geomfunction.h \
					 ../pal/internalexception.h \
					 ../pal/labelposition.h \
					 ../pal/pointset.h \
					 ../pal/priorityqueue.h \
					 ../pal/util.h \
					 ../pal/problem.h \
					 ../pal/simplemutex.h

libjpal_la_LIBADD = ${GEOS_LIBS}
libjpal_la_LDFLAGS = -version-info \
					 @VERSION_MAJOR@:@VERSION_MINOR@:@VERSION_PATCH@ \
					 ${WINLDFLAGS}

endif # ENABLE_JAVA
