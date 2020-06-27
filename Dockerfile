# This dockerfile serves to only create the environment. It will not add any folders
# or perform any builds / executions
FROM debian:10.4
RUN apt update && apt install -y build-essential pkg-config libmodbus-dev libmodbus5 git cmake
RUN git clone https://github.com/eclipse/mraa.git /opt/mraa && cd /opt/mraa && cmake . && make install
RUN git config --global core.autocrlf input
