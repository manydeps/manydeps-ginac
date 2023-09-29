load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

package(
    default_visibility = ["//visibility:public"],
)

# DOCUMENTATION ON SELECT:
# https://bazel.build/reference/be/functions?hl=pt-br#select

# MPIR instructions
# https://stackoverflow.com/questions/47359417/how-to-compile-gmp-for-windows-using-visual-studio

# manual thirdparty GMP library
cc_library(
    name = "lib",
    #srcs = glob(
    #    ["vcpkg_installed/x64-linux/lib/**/*.a"],
    #    ["vcpkg_installed/x64-linux/lib/**/*.lib"]
    #),
    # thirdparty\mpir\msvc\vs22\lib_mpir_cxx\x64\Release\mpirxx.lib
    srcs = select({
        "@bazel_tools//src/conditions:windows": glob(["mpir/lib/**/mpir.lib", "mpir/lib/**/mpirxx.lib"]),
        "@bazel_tools//src/conditions:darwin": glob(["gmp/.libs/**/libgmp*.a", "gmp/.libs/**/libgmp*.dylib"]),
        "//conditions:default": glob(["gmp/.libs/**/libgmp.a", "gmp/.libs/**/libgmpxx.a"]),
    }),
    #hdrs = glob(["vcpkg_installed/x64-linux/include/**/*.h"]),
    hdrs = select({
        "@bazel_tools//src/conditions:windows": glob(["mpir/lib/x64/Release/gmp.h", "mpir/lib/x64/Release/gmpxx.h"]),
        "@bazel_tools//src/conditions:darwin": glob(["gmp/gmp.h", "gmp/gmpxx.h"]),
        "//conditions:default": glob(["gmp/gmp.h", "gmp/gmpxx.h"]),
    }),
    #includes = ["vcpkg_installed/x64-linux/include/"],
    includes = select({
        "@bazel_tools//src/conditions:windows": ["mpir/lib/x64/Release/"],
        "@bazel_tools//src/conditions:darwin": ["gmp"],
        "//conditions:default": ["gmp"],
    }),
    visibility = ["//visibility:public"],
    linkstatic = 1
)
