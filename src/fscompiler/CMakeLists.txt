FILE(GLOB fscompiler_sources *.cpp *.hpp)
ADD_LIBRARY(fscompiler SHARED ${fscompiler_sources})