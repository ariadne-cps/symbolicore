

# SymboliCore

[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](https://opensource.org/licenses/MIT) [![Unix Status](https://github.com/ariadne-cps/symbolicore/workflows/Unix/badge.svg)](https://github.com/ariadne-cps/symbolicore/actions/workflows/unix.yml)
[![Windows Status](https://github.com/ariadne-cps/symbolicore/workflows/Windows/badge.svg)](https://github.com/ariadne-cps/symbolicore/actions/workflows/windows.yml) [![codecov](https://codecov.io/gh/ariadne-cps/symbolicore/branch/main/graph/badge.svg)](https://codecov.io/gh/ariadne-cps/symbolicore)

SymboliCore is a library for basic symbolic operations, namely expressions, constraints and their evaluation.

### Building

To build the library from sources in a clean way, it is preferable that you set up a build subdirectory, say:

```
$ mkdir build && cd build
```

Then you can prepare the build environment, choosing a Release build for maximum performance:

```
$ cmake .. -DCMAKE_BUILD_TYPE=Release
```

At this point, if no error arises, you can build with:

```
$ cmake --build .
```

The library is meant to be used as a dependency, in particular by disabling testing as long as the *tests* target is already defined in an enclosing project.

## Contribution guidelines ##

If you would like to contribute to SymboliCore, please contact the developer: 

* Luca Geretti <luca.geretti@univr.it>
