includes("cpu")

target("main")
  set_kind("binary")
  add_files("main.cpp")
  add_deps("cpu")