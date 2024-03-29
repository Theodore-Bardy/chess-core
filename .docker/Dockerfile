FROM ubuntu:22.04

# Working directory
WORKDIR /app

# 'noninteractive' mode
ENV DEBIAN_FRONTEND noninteractive

# Update and installation of required tools
RUN apt-get update && apt-get install --no-install-recommends -y \
    build-essential \
    bzip2 \
    ca-certificates \
    clang-format \
    cmake \
    git \
    make \
    ssh
