set_project("nes-simulator-cpp")
set_version("0.0.1")

set_toolset('cc', "clang")
set_toolset('cxx', "clang++")

add_languages("c99", "cxx23")

add_requires("libsdl")
add_packages("libsdl")

add_includedirs("src")

includes("src")