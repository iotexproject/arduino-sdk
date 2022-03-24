# Base image which contains global dependencies
FROM ubuntu:20.04 as base
WORKDIR /workdir

# System dependencies
ARG arch=amd64
RUN mkdir /workdir/project && \
    mkdir /workdir/.cache && \
    apt-get -y update && \
    apt-get -y upgrade && \
    DEBIAN_FRONTEND=noninteractive apt-get -y install \
        wget \
        python3-pip \
        ninja-build \
        gperf \
        git \
        unzip \
        python3-setuptools \
        libncurses5 libncurses5-dev \
        libyaml-dev libfdt1 \
        libusb-1.0-0-dev udev \
        device-tree-compiler=1.5.1-1 \
        ruby \
        gdb \
        debian-keyring \
        debian-archive-keyring \
        apt-transport-https \
        curl \
        lsb-release \
        protobuf-compiler \
        psmisc \
        libcurl4-gnutls-dev && \
    apt-get -y clean && apt-get -y autoremove && \
    # Latest PIP & Python dependencies
    python3 -m pip install -U pip && \
    python3 -m pip install -U setuptools && \
    python3 -m pip install cmake>=3.20.0 wheel && \
    # Newer PIP will not overwrite distutils, so upgrade PyYAML manually
    python3 -m pip install --ignore-installed -U PyYAML && \
    # ClangFormat
    python3 -m pip install -U six && \
    apt-get -y install clang-format

# Envoy - for grpc-http server
RUN curl -sL 'https://deb.dl.getenvoy.io/public/gpg.8115BA8E629CC074.key' | gpg --dearmor -o /usr/share/keyrings/getenvoy-keyring.gpg && \
    echo "deb [arch=amd64 signed-by=/usr/share/keyrings/getenvoy-keyring.gpg] https://deb.dl.getenvoy.io/public/deb/ubuntu $(lsb_release -cs) main" |  tee /etc/apt/sources.list.d/getenvoy.list && \
    apt update && \
    apt install -y getenvoy-envoy

# Protoc - required for envoy
RUN mkdir -p /workdir/google/api
WORKDIR /workdir/google/api && \
    wget https://raw.githubusercontent.com/googleapis/googleapis/master/google/api/annotations.proto  && \
    wget https://raw.githubusercontent.com/googleapis/googleapis/master/google/api/http.proto

# NodeJs and npm - for abi decoder script (tools/abiDecoder.js)
WORKDIR /workdir/project/tools
RUN DEBIAN_FRONTEND=noninteractive apt-get -y install nodejs && \
    apt-get -y update && \
    apt-get -y install npm && \
    npm install -g abi-decoder web3

WORKDIR /workdir/project
ENV LC_ALL=C.UTF-8
ENV LANG=C.UTF-8
ENV XDG_CACHE_HOME=/workdir/.cache
