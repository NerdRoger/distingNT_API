FROM ubuntu:20.04

# Preconfigure tzdata
ENV DEBIAN_FRONTEND=noninteractive \
    TZ=Etc/UTC

# Install required packages
RUN apt-get update && apt-get install -y \
    build-essential \
    gcc-arm-none-eabi \
    make \
    cmake \
    git \
    python3 \
    pkg-config

WORKDIR /app

RUN git clone https://github.com/grame-cncm/faust.git
RUN cd faust && make && make install
# RUN rm -rf faust

# Files created by this container have no permission restrictions
# Allows the files created to be deleted by the host, SMB shares, etc
RUN echo >> /root/.bashrc
RUN echo "umask 0000" >> /root/.bashrc

CMD ["/bin/bash"]