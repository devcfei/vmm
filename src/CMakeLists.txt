cmake_minimum_required(VERSION 3.6)

project(vmm)


include_directories(
    "${ROOT_PATH}/include"
    "${PROJECT_SOURCE_DIR}"
    )

aux_source_directory(${PROJECT_SOURCE_DIR} SRCS)


add_library(vmm_shared SHARED ${SRCS} )
set_target_properties(vmm_shared PROPERTIES OUTPUT_NAME "vmm")
set_target_properties(vmm_shared PROPERTIES PUBLIC_HEADER "${ROOT_PATH}/include/vmm.h")


add_library(vmm_static STATIC ${SRCS} )
target_compile_options(vmm_static PRIVATE -nostdlib -nostdinc -fno-pic -fno-builtin -fno-stack-protector )
set_target_properties(vmm_static PROPERTIES PUBLIC_HEADER "${ROOT_PATH}/include/vmm.h")
set_target_properties(vmm_static PROPERTIES OUTPUT_NAME "vmm")
