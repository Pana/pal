
lib_LTLIBRARIES = libpal.la

INCLUDES = -I@top_srcdir@/src/includes ${GEOS_CFLAGS} ${WINCFLAGS}

#noinst_LIBRARIES = libpal.a
libpal_la_SOURCES = feature.cpp \
					     geomfunction.cpp \
						  label.cpp \
						  labelposition.cpp \
						  layer.cpp  \
						  pal.cpp \
						  palstat.cpp \
						  pointset.cpp \
						  priorityqueue.cpp \
						  problem.cpp \
						  util.cpp \
						  rtree.hpp \
						  linkedlist.hpp \
						  hashtable.hpp \
						  feature.h \
						  geomfunction.h \
						  internalexception.h \
						  labelposition.h \
						  pointset.h \
						  priorityqueue.h \
						  problem.h \
						  simplemutex.h \
						  util.h

libpal_la_LIBADD = ${GEOS_LIBS} ${THREAD_LIBS}
libpal_la_LDFLAGS = -version-info @VERSION_MAJOR@:@VERSION_MINOR@:@VERSION_PATCH@ \
					${WINLDFLAGS}

libpal_la_includedir=@includedir@/pal
libpal_la_include_HEADERS=../includes/pal/pal.h \
                          ../includes/pal/layer.h \
								  ../includes/pal/label.h \
								  ../includes/pal/palstat.h \
								  ../includes/pal/palgeometry.h \
								  ../includes/pal/palexception.h 
