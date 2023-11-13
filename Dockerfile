FROM ubuntu:23.04 AS builder

RUN apt-get update && apt-get install -y \
    build-essential \
    gfortran \
    git \
    cmake \
    liblapack-dev \
    pkg-config \
    qtbase5-dev \
    qtdeclarative5-dev \
    qt3d5-dev \
    coinor-libipopt-dev \
    && rm -rf /var/lib/apt/lists/*


RUN git clone https://github.com/casadi/casadi.git
WORKDIR /casadi
RUN mkdir build
WORKDIR /casadi/build
RUN cmake .. -DWITH_IPOPT=ON -DWITH_EXAMPLES=OFF
RUN make
RUN make install

FROM theshadowx/qt5:16.04
COPY --from=builder /casadi /casadi

ENV CASADI_PATH /casadi
ENV PATH $PATH:$CASADI_PATH

WORKDIR /muscle
COPY . .
WORKDIR /muscle/build
RUN cmake .. 
RUN make
RUN chmod +x muscle_path
WORKDIR /
CMD ["./muscle_path"]

#RUN apt-get autoremove -y && apt-get cleans