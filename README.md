# DevContainerCppEnv
A devcontainer environment with example app, lib, and test projects using conan and cmake, that can serve as a starting point for a C++ project.

# Prerequisites
- You must have VSCode, and the "Microsoft Remote Development" extension installed.
- You must have docker installed

Clone this repo into any directory on your host machine.

# Using the Devcontainer
- On your host machine, open VSCode
- Open the command pallete and choose "Dev Container: Open folder in container"

Choose the location containing this repo.

After the container is built, vs-code should automatically attach to it, and perform a cmake configure.
Conan should have a default profile setup for you via the "postCreateCommand" in devcontainer.json and have installed the dependencies.
You should be ready to hit the build button and build the project. Build files will be in /build

# Building outside the IDE
In a terminal connected to the container:

Make sure you delete the `/build` directorty if it exists, from any previous build and recreate it
```
rm -rf /build
mkdir -p /build
```

Make sure you have a Conan profile
```
conan profile detect
```

Install dependencies before building with the following command:
```
conan install . --output-folder=/build --build=missing -s compiler.cppstd=gnu20 --settings=build_type=Debug
```

Configure cmake
```
cd /build
cmake /workspace -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug
```

Build the project
```
cmake --build .
```

# Run test suite outside IDE
from the build dir
```
test/eventtest
```