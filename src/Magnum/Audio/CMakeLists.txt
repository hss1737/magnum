#
#   This file is part of Magnum.
#
#   Copyright © 2010, 2011, 2012, 2013, 2014, 2015, 2016
#             Vladimír Vondruš <mosra@centrum.cz>
#
#   Permission is hereby granted, free of charge, to any person obtaining a
#   copy of this software and associated documentation files (the "Software"),
#   to deal in the Software without restriction, including without limitation
#   the rights to use, copy, modify, merge, publish, distribute, sublicense,
#   and/or sell copies of the Software, and to permit persons to whom the
#   Software is furnished to do so, subject to the following conditions:
#
#   The above copyright notice and this permission notice shall be included
#   in all copies or substantial portions of the Software.
#
#   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
#   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
#   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
#   DEALINGS IN THE SOFTWARE.
#

find_package(OpenAL REQUIRED)

set(MagnumAudio_SRCS
    AbstractImporter.cpp
    Audio.cpp
    Buffer.cpp
    Context.cpp
    Renderer.cpp
    Source.cpp)

set(MagnumAudio_HEADERS
    AbstractImporter.h
    Audio.h
    Buffer.h
    Context.h
    Extensions.h
    Renderer.h
    Source.h

    visibility.h)

if(WITH_SCENEGRAPH)
    list(APPEND MagnumAudio_HEADERS
        Listener.h
        Playable.h
        PlayableGroup.h)

    list(APPEND MagnumAudio_SRCS
        Listener.cpp)
endif()

# Audio library
add_library(MagnumAudio ${SHARED_OR_STATIC}
    ${MagnumAudio_SRCS}
    ${MagnumAudio_HEADERS})
target_include_directories(MagnumAudio PUBLIC ${OPENAL_INCLUDE_DIR})
set_target_properties(MagnumAudio PROPERTIES DEBUG_POSTFIX "-d")
if(BUILD_STATIC_PIC)
    set_target_properties(MagnumAudio PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()
target_link_libraries(MagnumAudio Magnum Corrade::PluginManager ${OPENAL_LIBRARY})
if(WITH_SCENEGRAPH)
    target_link_libraries(MagnumAudio MagnumSceneGraph)
endif()

install(TARGETS MagnumAudio
    RUNTIME DESTINATION ${MAGNUM_BINARY_INSTALL_DIR}
    LIBRARY DESTINATION ${MAGNUM_LIBRARY_INSTALL_DIR}
    ARCHIVE DESTINATION ${MAGNUM_LIBRARY_INSTALL_DIR})
install(FILES ${MagnumAudio_HEADERS} DESTINATION ${MAGNUM_INCLUDE_INSTALL_DIR}/Audio)

if(BUILD_TESTS)
    add_subdirectory(Test)
endif()

# Magnum Audio target alias for superprojects
add_library(Magnum::Audio ALIAS MagnumAudio)
