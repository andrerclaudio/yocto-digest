FROM ubuntu:22.04
MAINTAINER Andre Ribeiro <andre.ribeiro.srs@gmail.com>

ARG DEBIAN_FRONTEND=noninteractive

RUN dpkg --add-architecture i386

RUN apt-get update \
    && apt-get upgrade \
    && apt-get install -y locales sudo \
    build-essential chrpath cpio debianutils diffstat file gawk gcc git \
    iputils-ping libacl1 liblz4-tool python3 python3-git \
    python3-jinja2 python3-pexpect python3-pip python3-subunit socat \
    texinfo unzip wget xz-utils zstd

RUN apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# By default, Ubuntu uses dash as an alias for sh. Dash does not support the source command
# needed for setting up Yocto build environments. Use bash as an alias for sh.
RUN which dash &> /dev/null && (\
    echo "dash dash/sh boolean false" | debconf-set-selections && \
    dpkg-reconfigure dash) || \
    echo "Skipping dash reconfigure (not applicable)"

# Set the locale to en_US.UTF-8, because the Yocto build fails without any locale set.
RUN locale-gen en_US.UTF-8 && update-locale LC_ALL=en_US.UTF-8 LANG=en_US.UTF-8
ENV LANG en_US.UTF-8
ENV LC_ALL en_US.UTF-8

# Install the repo tool to handle git submodules (meta layers) comfortably.
ADD https://storage.googleapis.com/git-repo-downloads/repo /usr/local/bin/
RUN chmod 755 /usr/local/bin/repo

# Add user "yocto" to sudoers. Then, the user can install Linux packages in the container.
ENV USER_NAME yocto
RUN echo "${USER_NAME} ALL=(ALL) NOPASSWD: ALL" > /etc/sudoers.d/${USER_NAME} && \
    chmod 0440 /etc/sudoers.d/${USER_NAME}

# The running container writes all the build artefacts to a host directory (outside the container).
# The container can only write files to host directories, if it uses the same user ID and
# group ID owning the host directories. The host_uid and group_uid are passed to the docker build
# command with the --build-arg option. By default, they are both 1001. The docker image creates
# a group with host_gid and a user with host_uid and adds the user to the group. The symbolic
# name of the group and user is yocto.
ARG host_uid=1001
ARG host_gid=1001
RUN groupadd -g $host_gid $USER_NAME && useradd -g $host_gid -m -s /bin/bash -u $host_uid $USER_NAME

# Perform the Yocto build as user yocto (not as root).
# By default, docker runs as root. However, Yocto builds should not be run as root, but as a 
# normal user. Hence, we switch to the newly created user yocto.
USER $USER_NAME

WORKDIR /public/Work
