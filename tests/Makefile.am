if ENABLE_TEST

#SUBDIRS = problems

INCLUDES = -I@top_srcdir@/src/includes ${GEOS_CFLAGS} ${WINCFLAGS}

bin_PROGRAMS = pal_labeller

pal_labeller_SOURCES = pal.cpp \
					   Geom.cpp \
					   Geom.h

pal_labeller_LDFLAGS = ${GEOS_LIBS} -L@top_srcdir@/src/pal/.libs -lpal


dist_bin_SCRIPTS = \
			psql2pal.pl \
			xyn2pal.pl \
			pal_batch.pl

endif # ENABLE_TEST
