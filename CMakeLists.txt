cmake_minimum_required(VERSION 3.15)
project(TPFinalEDyA_master C)

set(CMAKE_C_STANDARD 11)

include_directories(.)
include_directories(lists)
include_directories(lists/dlist)

add_executable(TPFinalEDyA_master
    arbolDeIntervalos/lists/dlist/dlist.c
    arbolDeIntervalos/lists/dlist/dlist.h
    arbolDeIntervalos/lists/queue.c
    arbolDeIntervalos/lists/queue.h
    arbolDeIntervalos/lists/stack.c
    arbolDeIntervalos/lists/stack.h
    arbolDeIntervalos/avltree.c
    arbolDeIntervalos/avltree.h
    manejoAlias/interfaz.h
    manejoAlias/interfaz.c
    arbolDeIntervalos/intervalo.h
    arbolDeIntervalos/intervalo.c
    manejoAlias/hash.c
    manejoAlias/hash.h
    main.c)
