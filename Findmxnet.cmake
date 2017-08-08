find_path(mxnet_ROOT_DIR
        NAMES include/mxnet/c_predict_api.h
        HINTS mxnet
        )

find_library(mxnet_LIBRARIES
        NAMES mxnet_predict
        HINTS ${CMAKE_BINARY_DIR}
        )

find_path(mxnet_INCLUDE_DIRS
        NAMES mxnet/c_predict_api.h
        HINTS ${mxnet_ROOT_DIR}/include
        )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(mxnet DEFAULT_MSG
        mxnet_LIBRARIES
        mxnet_INCLUDE_DIRS
        )

mark_as_advanced(
        mxnet_ROOT_DIR
        mxnet_LIBRARIES
        mxnet_INCLUDE_DIRS
)
