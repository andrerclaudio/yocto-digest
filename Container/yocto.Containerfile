# Use Ubuntu 22.04 LTS as a stable base image
FROM ubuntu:22.04

# Metadata labels following OCI conventions (replaces the old MAINTAINER instruction)
LABEL org.opencontainers.image.authors="Andre Ribeiro <andre.ribeiro.srs@gmail.com>" \
      org.opencontainers.image.license="MIT" \
      org.opencontainers.image.version="1.0"

# Ensure apt is non‑interactive
ARG DEBIAN_FRONTEND=noninteractive
ENV DEBIAN_FRONTEND=$DEBIAN_FRONTEND

# Enable 32‑bit architecture support for Yocto cross‑builds
RUN dpkg --add-architecture i386

RUN apt-get update && \
    apt-get full-upgrade && \
    apt-get install -y --no-install-recommends \
    build-essential chrpath cpio debianutils diffstat file gawk gcc git \
    iputils-ping libacl1 liblz4-tool python3 python3-git \
    python3-jinja2 python3-pexpect python3-pip python3-subunit socat \
    bsdmainutils gcc-multilib git-lfs libegl1-mesa libgmp-dev libmpc-dev \
    libsdl1.2-dev libssl-dev libusb-1.0-0 pylint xterm \
    texinfo unzip wget xz-utils zstd locales && \
    # Remove apt caches immediately (in the same layer) to avoid bloating the image
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Replace /bin/sh (dash) with bash so scripts relying on 'source' work correctly
RUN rm /bin/sh && ln -s /bin/bash /bin/sh

# Generate and set a UTF‑8 locale (Yocto builds often break without a locale)
RUN locale-gen en_US.UTF-8 && \
    update-locale LC_ALL=en_US.UTF-8 LANG=en_US.UTF-8
ENV LANG=en_US.UTF-8 \
    LC_ALL=en_US.UTF-8

# Configure the container timezone (useful for logs, build tools, etc.)
ENV TZ=America/Argentina/Buenos_Aires
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && \
    echo $TZ > /etc/timezone

# Create a non‑root user 'builder' with explicit UID/GID for deterministic ownership
ARG host_uid=1001
ARG host_gid=1001
ENV USER_NAME=builder
RUN groupadd -g ${host_gid} ${USER_NAME} && \
    useradd --no-log-init -r -g ${host_gid} -u ${host_uid} -m -s /bin/bash ${USER_NAME}

# Switch to the non‑root user for all subsequent steps
USER ${USER_NAME}

# Set the working directory inside the container
WORKDIR /home/${USER_NAME}/host

# Configure Git for the 'builder' user
RUN git config --global user.email "builder@example.com" && \
    git config --global user.name "Builder"
