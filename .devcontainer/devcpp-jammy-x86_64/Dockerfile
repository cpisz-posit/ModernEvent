# To build the docker image:
# docker build -t devenvcpp/ubuntu:0.1 .
#
# To create the docker container and open the terminal
# This will create a volume for us to persist our files. We can mount this volume in other containers later if we like.
#   The volume differs from mounting a directory from the host OS, in that docker will manage it and we won't access it from the host. 
#   That way we don't have to worry about line endings and such.
# docker run --name devenvcpp --user developer -it --mount type=volume,src=vol_devenvcpp,dst=/home/developer/volume devenvcpp/ubuntu:0.1 bash
# 
# When done, just 'exit'
# 
# To start the already created docker container and open the terminal
# docker start --attach --interactive devenvcpp
#
# When done, just 'exit'

FROM ubuntu:22.04
SHELL ["/bin/bash", "-c"]
MAINTAINER ChristopherPisz
LABEL Description="Chris' Ubuntu based development environment for C++"

ARG USERNAME=developer
ARG USER_UID=999
ARG USER_GID=$USER_UID

# Install all the things we'll need
#    I am installing git on the container rather than the host, because we are using the volume rather than mounting a directory we share with the host
#    in my opinion, doing the latter invites all kinds of problems.
RUN apt update && apt -y --no-install-recommends install \
    sudo \
    bash \
    vim \
    git \
    ssh \
    build-essential \
    clang \
    cmake \
    ninja-build \
    gdb \
    wget \
    python3 \
    python3-pip \
    python3-venv

# Create a virtual environment for python
RUN python3 -m venv /opt/venv

# Install conan, the package manager for C++ that everyone needs in thier life, in the virtual environment
RUN /opt/venv/bin/pip install conan

# Create a non-root user and give them sudo
RUN groupadd --gid $USER_GID $USERNAME && \
    useradd --uid $USER_UID --gid $USER_GID -m $USERNAME && \
    echo $USERNAME ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/$USERNAME && \
    chmod 0440 /etc/sudoers.d/$USERNAME && \
    echo "$USERNAME:$USERNAME" | sudo chpasswd

# Create user home directory
RUN mkdir -p /home/${USERNAME} && chown ${USER_UID}:${USER_GID} /home/${USERNAME}

# Create build directory
RUN mkdir -p /build && chmod a+rwX /build

# Change to the user
USER $USERNAME
WORKDIR /workspace

# Activate venv for interactive shells (for the user)
RUN echo 'source /opt/venv/bin/activate' >> /home/${USERNAME}/.bashrc

