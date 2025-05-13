FROM archlinux:base-devel

ENV HOME /root

SHELL ["/bin/bash", "-c"]

RUN pacman -Syu cmake clang --noconfirm && \
    pacman -S qt6-base qt6-declarative --noconfirm

WORKDIR /app

COPY WorkoutChart ./WorkoutChart
COPY WorkoutLib ./WorkoutLib
COPY CMakeLists.txt .

WORKDIR /app/build
RUN cmake .. && \
    cmake --build . --parallel 24
