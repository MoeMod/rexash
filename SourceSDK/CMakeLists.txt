
set(HL2SDK_PATH "." CACHE PATH "Path to hl2sdk")

set(CMAKE_CXX_STANDARD 17)
if(MSVC)
	add_compile_options(/wd4091 /wd4819 /wd4828 /wd5033 /permissive- /utf-8)
	add_compile_definitions(_CRT_SECURE_NO_WARNINGS=1)
elseif(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
	add_compile_options(-Wno-register)
endif()

if(CMAKE_SIZEOF_VOID_P MATCHES "8")
    set(HL2SDK_64BIT ON)
    add_compile_options(-DPLATFORM_64BITS=1 -DX64BITS=1)
    message(STATUS "Building for 64 Bit")
else()
    set(HL2SDK_64BIT OFF)
    message(STATUS "Building for 32 Bit")
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Darwin")
    set(OSX ON)
    add_compile_options(-DOSX -D_OSX)
    set(HL2SDK_LIB_STATIC_EXT ".a")
    set(HL2SDK_LIB_SHARED_EXT ".dylib")
    set(HL2SDK_LIB_PREFIX "lib")

    if(HL2SDK_64BIT)
        set(HL2SDK_LIB_DIR ${HL2SDK_PATH}/lib/osx64)
        set(HL2SDK_LIB_STATIC_SUFFIX "")
    else()
        set(HL2SDK_LIB_DIR ${HL2SDK_PATH}/lib/mac)
        set(HL2SDK_LIB_STATIC_SUFFIX "_i486")
    endif()
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(LINUX ON)
    add_compile_options(-D_LINUX -DLINUX)
    set(HL2SDK_LIB_STATIC_EXT ".a")
    set(HL2SDK_LIB_SHARED_EXT ".so")
    set(HL2SDK_LIB_PREFIX "lib")

    if(HL2SDK_64BIT)
        set(HL2SDK_LIB_DIR ${HL2SDK_PATH}/lib/linux64)
        set(HL2SDK_LIB_STATIC_SUFFIX "")
    else()
        set(HL2SDK_LIB_DIR ${HL2SDK_PATH}/lib/linux)
        set(HL2SDK_LIB_STATIC_SUFFIX "_i486")
    endif()
elseif(WIN32)
    set(HL2SDK_LIB_DIR ${HL2SDK_PATH}/lib/public)
    set(HL2SDK_LIB_STATIC_EXT ".lib")
    set(HL2SDK_LIB_SHARED_EXT ".dll")
    set(HL2SDK_LIB_STATIC_SUFFIX "")
endif()

link_directories(${HL2SDK_LIB_DIR})

if(MSVC)
    add_compile_options(-DCOMPILER_MSVC)
    if(HL2SDK_64BIT)
        add_compile_options(-DCOMPILER_MSVC64)
     else()
        add_compile_options(-DCOMPILER_MSVC32)
     endif()
else()
    add_compile_options(-DCOMPILER_GCC) ## clang or gcc
endif()

# for mingw
if(MINGW)
    add_compile_options(
            -Dmalloc_usable_size=_msize
            -DUSE_STDC_FOR_SIMD=1
    )
    add_compile_options(-fpermissive)
endif()

if(NOT WIN32)
    set(POSIX ON)

    add_compile_options(
            -DVPROF_LEVEL=1 -DSWDS -D_finite=finite -Dstricmp=strcasecmp -D_stricmp=strcasecmp
            -D_strnicmp=strncasecmp -Dstrnicmp=strncasecmp -D_vsnprintf=vsnprintf -D_alloca=alloca -Dstrcmpi=strcasecmp
    )
endif()

if(POSIX)
    add_compile_options(-DPOSIX)
endif()

if(MSVC)
    add_compile_options(/wd4819 /wd4828 /wd5033 /permissive- /utf-8)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS=1)
elseif(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    add_compile_options(-Wno-register -Wno-implicit-const-int-float-conversion -Wno-deprecated-volatile)
elseif(${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
    add_compile_options(-Wno-register -Wno-narrowing -Wno-volatile)
endif()

add_library(vinterface STATIC
        ${HL2SDK_PATH}/tier1/interface.cpp
)
target_include_directories(vinterface PUBLIC
        ${HL2SDK_PATH}/public
        ${HL2SDK_PATH}/public/tier1
)


add_library(vstdlib STATIC
        ${HL2SDK_PATH}/vstdlib/random.cpp
        ${HL2SDK_PATH}/vstdlib/keyvaluessystem.cpp
)
target_include_directories(vstdlib PUBLIC
        ${HL2SDK_PATH}/public
        ${HL2SDK_PATH}/public/vstdlib
        )
target_compile_definitions(vstdlib PRIVATE
    -DVSTDLIB_DLL_EXPORT
)

add_library(tier0_h INTERFACE)
target_include_directories(tier0_h INTERFACE
        ${HL2SDK_PATH}/public
        ${HL2SDK_PATH}/public/tier0
        )
target_include_directories(tier0_h INTERFACE
        ${HL2SDK_PATH}
        )
add_library(tier0_src INTERFACE)
target_sources(tier0_src INTERFACE
        ${HL2SDK_PATH}/tier0/assert_dialog.cpp
        ${HL2SDK_PATH}/tier0/cpu.cpp
        ${HL2SDK_PATH}/tier0/dbg.cpp
        ${HL2SDK_PATH}/tier0/fasttimer.cpp
        ${HL2SDK_PATH}/tier0/mem.cpp
        ${HL2SDK_PATH}/tier0/memstd.cpp
        ${HL2SDK_PATH}/tier0/memdbg.cpp
        ${HL2SDK_PATH}/tier0/mem_helpers.cpp
        ${HL2SDK_PATH}/tier0/memblockhdr.cpp
        ${HL2SDK_PATH}/tier0/memvalidate.cpp
        ${HL2SDK_PATH}/tier0/minidump.cpp
        ${HL2SDK_PATH}/tier0/platform.cpp
        ${HL2SDK_PATH}/tier0/pme.cpp
        ${HL2SDK_PATH}/tier0/threadtools.cpp
        ${HL2SDK_PATH}/tier0/testthread.cpp
        ${HL2SDK_PATH}/tier0/vcrmode.cpp
        ${HL2SDK_PATH}/commandline.cpp
)
if( POSIX )
    target_sources(tier0_src INTERFACE ${HL2SDK_PATH}/tier0/cpu_posix.cpp)
    target_sources(tier0_src INTERFACE ${HL2SDK_PATH}/tier0/platform_posix.cpp)
    target_sources(tier0_src INTERFACE ${HL2SDK_PATH}/tier0/pme_posix.cpp)
    target_sources(tier0_src INTERFACE ${HL2SDK_PATH}/tier0/vcrmode_posix.cpp)
endif()
target_link_libraries(tier0_src INTERFACE tier0_h)

add_library(tier0_dynamic SHARED)
target_link_libraries(tier0_dynamic PRIVATE tier0_src)
target_link_libraries(tier0_dynamic PUBLIC tier0_h)
target_compile_definitions(tier0_dynamic PRIVATE
    -DTIER0_DLL_EXPORT
)
if( WIN32 )
    target_link_libraries(tier0_dynamic PUBLIC ws2_32)
endif()
target_include_directories(tier0_dynamic PUBLIC ${PROJECT_BINARY_DIR})

add_library(tier0_static STATIC)
target_link_libraries(tier0_static PRIVATE tier0_src)
target_link_libraries(tier0_static PUBLIC tier0_h)
target_compile_definitions(tier0_static PUBLIC
    -DSTATIC_TIER0
)
if( WIN32 )
    target_link_libraries(tier0_static PUBLIC ws2_32)
endif()

add_library(tier0 ALIAS tier0_static)

add_library(tier1 STATIC
        ${HL2SDK_PATH}/tier1/bitbuf.cpp
        ${HL2SDK_PATH}/tier1/byteswap.cpp
        ${HL2SDK_PATH}/tier1/characterset.cpp
        ${HL2SDK_PATH}/tier1/checksum_crc.cpp
        ${HL2SDK_PATH}/tier1/checksum_md5.cpp
        ${HL2SDK_PATH}/tier1/convar.cpp
        ${HL2SDK_PATH}/tier1/datamanager.cpp
        ${HL2SDK_PATH}/tier1/diff.cpp
        ${HL2SDK_PATH}/tier1/generichash.cpp
        ${HL2SDK_PATH}/tier1/KeyValues.cpp
        ${HL2SDK_PATH}/tier1/mempool.cpp
        ${HL2SDK_PATH}/tier1/memstack.cpp
        ${HL2SDK_PATH}/tier1/NetAdr.cpp
        ${HL2SDK_PATH}/tier1/processor_detect.cpp
        ${HL2SDK_PATH}/tier1/rangecheckedvar.cpp
        ${HL2SDK_PATH}/tier1/stringpool.cpp
        ${HL2SDK_PATH}/tier1/strtools.cpp
        ${HL2SDK_PATH}/tier1/tier1.cpp
        ${HL2SDK_PATH}/tier1/utlbuffer.cpp
        ${HL2SDK_PATH}/tier1/utlstring.cpp
        ${HL2SDK_PATH}/tier1/utlsymbol.cpp
        )
target_include_directories(tier1 PUBLIC
        ${HL2SDK_PATH}/public
        ${HL2SDK_PATH}/public/tier1
        )

add_library(mathlib STATIC
        ${HL2SDK_PATH}/mathlib/3dnow.cpp
        ${HL2SDK_PATH}/mathlib/mathlib_base.cpp
        ${HL2SDK_PATH}/mathlib/sse.cpp
        )
if(NOT MSVC)
    target_compile_options(mathlib PRIVATE -Wno-c++11-narrowing)
endif()
target_include_directories(mathlib PUBLIC 
    ${HL2SDK_PATH}/public
    ${HL2SDK_PATH}/public/mathlib
)

target_link_libraries(tier1 PUBLIC vinterface tier0 mathlib)
target_link_libraries(vstdlib PUBLIC vinterface tier0)

add_library(SourceSDK STATIC 
    FileSystem_Helpers.cpp
)
target_include_directories(SourceSDK PUBLIC ./ ./public)