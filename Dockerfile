FROM ubuntu:23.04 AS build

RUN apt-get update && apt-get install -y \
    gcc \
    g++ \
    gfortran \
    git \
    liblapack-dev \
    pkg-config \
    qtbase5-dev \
    qtdeclarative5-dev \
    qt3d5-dev \
    coinor-libipopt-dev \
    cmake \
    libxkbcommon-x11-0 \
    libxcb-xinerama0 \
    libnss3 \
    libxss1 \
    libasound2 \
    libglib2.0-0 \
    libfontconfig1 \
    libegl1-mesa \
    libxcomposite1 \
    libxcursor1 \
    libxi6 \
    libxtst6 \
    libxrandr2 \
    libxrender1 \
    libdbus-1-3 \
    libpulse-mainloop-glib0 \
    libx11-xcb-dev \
    libxcb-icccm4 \
    libxcb-image0 \
    libxcb-keysyms1 \
    libxcb-render-util0 \
    libxcb-xkb1 \
    libxkbcommon-x11-0 \
    libxkbcommon0 \
    && rm -rf /var/lib/apt/lists/*

ENV DISPLAY=:0

RUN git clone -b 3.6.3 https://github.com/casadi/casadi.git
WORKDIR /casadi
RUN mkdir build
WORKDIR /casadi/build
RUN cmake .. -DWITH_IPOPT=ON -DWITH_EXAMPLES=OFF
RUN make
RUN make install

ENV CASADI_PATH /casadi
ENV PATH $PATH:$CASADI_PATH

WORKDIR /muscle
COPY . .
WORKDIR /muscle/build
RUN cmake .. 
RUN make
RUN chmod +x muscle_path

#WORKDIR /
#FROM theshadowx/qt5:16.04
#COPY --from=build /muscle/build/. /muscle/.

CMD ["./muscle_path"]

#RUN apt-get autoremove -y && apt-get cleans