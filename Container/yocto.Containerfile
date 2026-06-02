# Use Ubuntu 24.04 LTS as a stable base image
FROM ubuntu:24.04

# Metadata labels following OCI conventions
LABEL org.opencontainers.image.authors="Andre Ribeiro <andre.ribeiro.srs@gmail.com>" \
      org.opencontainers.image.license="MIT" \
      org.opencontainers.image.version="1.1"

# Ensure apt is non-interactive
ARG DEBIAN_FRONTEND=noninteractive
ENV DEBIAN_FRONTEND=$DEBIAN_FRONTEND

# Enable 32-bit architecture support for Yocto cross-builds
RUN dpkg --add-architecture i386

RUN apt-get update && \
    apt-get upgrade -y && \
    apt-get install -y --no-install-recommends \
    build-essential chrpath cpio debianutils diffstat file gawk gcc git \
    iputils-ping libacl1 libcrypt-dev locales lz4 \
    python3 python3-git python3-jinja2 python3-pexpect python3-pip python3-subunit \
    python3-websockets \
    socat texinfo unzip wget xz-utils zstd efitools && \
    # Remove apt caches immediately (in the same layer) to avoid bloating the image
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Replace /bin/sh (dash) with bash so scripts relying on 'source' work correctly
RUN rm /bin/sh && ln -s /bin/bash /bin/sh

# Generate and set a UTF-8 locale (Yocto builds often break without a locale)
RUN locale-gen en_US.UTF-8 && update-locale LC_ALL=en_US.UTF-8 LANG=en_US.UTF-8
ENV LANG=en_US.UTF-8 \
    LC_ALL=en_US.UTF-8

# Configure the container timezone (parameterized for easy overriding)
ARG TZ=America/Argentina/Buenos_Aires
ENV TZ=${TZ}
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# Ubuntu 24.04 reserves UID 1000 for a default 'ubuntu' user.
ENV USER_NAME=builder
RUN usermod -l ${USER_NAME} -d /home/${USER_NAME} -m ubuntu && groupmod -n ${USER_NAME} ubuntu

# Switch to the non-root user for all subsequent steps
USER ${USER_NAME}

ENV BUILD_DIR /home/${USER_NAME}/host
RUN mkdir -p ${BUILD_DIR}

# Set the working directory inside the container
WORKDIR ${BUILD_DIR}

# Configure Git
RUN git config --global user.email "builder@example.com" && \
    git config --global user.name "Builder"
