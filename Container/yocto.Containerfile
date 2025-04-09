FROM ubuntu:22.04
LABEL maintainer="Andre Ribeiro <andre.ribeiro.srs@gmail.com>"

ARG DEBIAN_FRONTEND=noninteractive

RUN dpkg --add-architecture i386

RUN apt-get update && \
    apt-get upgrade && \
    apt-get install -y locales sudo \
    build-essential chrpath cpio debianutils diffstat file gawk gcc git \
    iputils-ping libacl1 liblz4-tool python3 python3-git \
    python3-jinja2 python3-pexpect python3-pip python3-subunit socat \
    texinfo unzip wget xz-utils zstd

RUN apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# By default, Ubuntu uses dash as an alias for sh. Dash does not support the source command
# needed for setting up Yocto build environments. Use bash as an alias for sh.
RUN which dash &> /dev/null && (\
    echo "dash dash/sh boolean false" | debconf-set-selections && \
    dpkg-reconfigure dash) || \
    echo "Skipping dash reconfigure (not applicable)"

RUN groupadd build -g 1000
RUN useradd -ms /bin/bash -p build build -u 1028 -g 1000 && \
    usermod -aG sudo build && \
    echo "build:build" | chpasswd

# Set the locale to en_US.UTF-8, because the Yocto build fails without any locale set.
RUN echo "en_US.UTF-8 UTF-8" > /etc/locale.gen && locale-gen
ENV LANG en_US.utf8

USER build
WORKDIR /public/Work
RUN git config --global user.email "build@example.com" && git config --global user.name "Build"
