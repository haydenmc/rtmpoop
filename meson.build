project(
    'rtmpoop',
    'cpp',
    default_options: [
        'cpp_std=c++2a', # C++20 standard
        'werror=true', # Treat warnings as errors
    ],
)

# Allow unknown pragmas, since we tend to use `pragma region/endregion`
add_project_arguments('-Wno-unknown-pragmas', language: 'cpp') 

sources = files([
    'src/main.cpp',
    'src/RTMPServer.cpp',
])

fmt_wrap = subproject('fmt', default_options: 'default_library=static')
meson.override_dependency('fmt', fmt_wrap.get_variable('fmt_dep')) # Use our copy of fmt for spdlog
spdlog_wrap = subproject('spdlog', default_options: ['default_library=static', 'compile_library=true', 'external_fmt=true'] )

deps = [
    # Meson wrapped dependencies
    fmt_wrap.get_variable('fmt_dep'),
    spdlog_wrap.get_variable('spdlog_dep'),
]

executable(
    'rtmpoop',
    sources,
    cpp_pch: 'pch/rtmpoop_pch.h',
    dependencies: deps,
)