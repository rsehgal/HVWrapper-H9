# - Finds Vc installation ( the wrapper library to SIMD intrinsics )
# This module sets up Vc information 
# It defines:
# CAEN_FOUND          If the ROOT is found
# CAEN_INCLUDE_DIR    PATH to the include directory
# CAEN_LIBRARIES      Most common libraries
# CAEN_LIBRARY_DIR    PATH to the library directory 

# look if an environment variable CAENROOT exists
set(CAENHVROOT $ENV{CAENHVROOT})

find_library(CAENHV_LIBRARIES  libcaenhvwrapper.so PATHS ${CAENHVROOT}/lib)
if (CAENHV_LIBRARIES) 
   set(CAEN_FOUND TRUE)	
   #string(REPLACE "/lib/libVc.a" "" CAENROOT  ${CAEN_LIBRARIES})
   set(CAENHV_INCLUDE_DIR ${CAENHVROOT}/include)
   set(CAENHV_LIBRARY_DIR ${CAENHVROOT}/lib)
   message(STATUS "Found CAEN library in ${CAENHV_LIBRARIES}")		
else()
   message(STATUS "CAEN library not found; try to set a CAENHVROOT environment variable to the base installation path or add -DCAENHVROOT= to the cmake command")	
endif()

