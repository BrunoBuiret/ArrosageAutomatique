# automake options
AUTOMAKE_OPTIONS = subdir-objects

# 
bin_PROGRAMS=tp_automated_watering
tp_automated_watering_SOURCES=tp_automated_watering.c src/utils.c src/automaton.c src/xmalloc.c system.h aclocal.m4 tp_automated_watering.1
tp_automated_watering_LDADD=-lwiringPi

#  uncomment the following if tp_automated_watering requires the math library
#tp_automated_watering_LDADD=-lm

EXTRA_DIST=tp_automated_watering.lsm.in tp_automated_watering.spec.in tp_automated_watering.texi

#  if you write a self-test script named `chk', uncomment the
#  following and add `chk' to the EXTRA_DIST list
#TESTS=chk

#  build and install the .info pages
info_TEXINFOS = tp_automated_watering.texi
tp_automated_watering_TEXINFOS = gpl.texinfo

#  install the man pages
man_MANS=tp_automated_watering.1
