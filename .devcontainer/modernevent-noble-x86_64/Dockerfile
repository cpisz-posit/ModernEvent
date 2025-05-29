FROM ubuntu:24.04
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