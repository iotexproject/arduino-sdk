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

# Download sdk-nrf and west dependencies to install pip requirements
# FROM base
# ARG sdk_nrf_revision=main
# RUN \
#     mkdir tmp && cd tmp && \
#     west init -m https://github.com/nrfconnect/sdk-nrf --mr ${sdk_nrf_revision} && \
#     west update --narrow -o=--depth=1 && \
#     python3 -m pip install -r zephyr/scripts/requirements.txt && \
#     python3 -m pip install -r nrf/scripts/requirements.txt && \
#     python3 -m pip install -r bootloader/mcuboot/scripts/requirements.txt && \
#     cd .. && rm -rf tmp

WORKDIR /workdir/project
ENV LC_ALL=C.UTF-8
ENV LANG=C.UTF-8
ENV XDG_CACHE_HOME=/workdir/.cache

# Add variables or paths if needed
# ENV PATH="${ZEPHYR_BASE}/scripts:${PATH}"
