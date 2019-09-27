FIND_LIBRARY(freenect2_LIBRARY freenect2
    PATHS C:/ApertusVR/3rdParty/multikinect/libfreenect2-master/install/lib
    NO_DEFAULT_PATH
)
SET(freenect2_LIBRARIES ${freenect2_LIBRARY} )
FIND_PATH(freenect2_INCLUDE_DIR libfreenect2/libfreenect2.hpp
    PATHS C:/ApertusVR/3rdParty/multikinect/libfreenect2-master/install/include
    NO_DEFAULT_PATH
)
SET(freenect2_INCLUDE_DIRS ${freenect2_INCLUDE_DIR})
