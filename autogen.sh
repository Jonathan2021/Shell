#!/bin/sh


if [ "$1" != "clean" ] ; then
    cp src/build/configure.ac .
    cp src/build/Makefile.am .
    autoreconf -vi
    if [ "$1" = "all" ] ; then
        ./configure
        make
    fi
else
    make distclean
    if [ -f "configure.ac" ] ; then
        rm configure.ac
    fi
    if [ -f "Makefile.am" ] ; then
        rm Makefile.am
    fi
    if [ -f "output.gv" ] ; then
        rm output.gv
    fi
    if [ -f *.ps ] ; then
        rm *.ps
    fi

    if [ -f ".42sh_history" ] ; then
        rm .42sh_history
    fi
    
    if [ -d "doc/html" ] ; then
        rm -rf doc/html
    fi
    rm -rf autom4te.cache/ compile config.h.in Makefile missing \
    aclocal.m4 install-sh depcomp Makefile.in .pytest_cache configure
fi
