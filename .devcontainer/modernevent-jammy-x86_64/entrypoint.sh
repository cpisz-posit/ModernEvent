#!/bin/bash
set -ex
echo "Entrypoint started"

cd /workspace

# Even though we set permissions in the Dockerfile, we need to make sure the user has permissions to the build directory
sudo chown -R $(id -u):$(id -g) /build

# Activate the python venv
source /opt/venv/bin/activate

# Conan
echo "Getting dependencies via Conan for all cmake presets..."

# Detect profile if not already done
conan profile detect --force

# Get the dependencies for all our presets
conan install . --output-folder=/build --build=missing --update -s compiler.cppstd=gnu20 --settings=build_type=Debug
conan install . --output-folder=/build --build=missing --update -s compiler.cppstd=gnu20 --settings=build_type=Release
conan install . --output-folder=/build --build=missing --update -s compiler.cppstd=gnu20 --settings=build_type=RelWithDebInfo

exec "$@"