AUX_SOURCE_DIRECTORY(. DIR_TEST1_SRCS)
ADD_SUBDIRECTORY(study)
ADD_LIBRARY(tool ${DIR_TEST1_SRCS})
ADD_LIBRARY(readFile ${DIR_TEST1_SRCS})
ADD_LIBRARY(geom ${DIR_TEST1_SRCS})
