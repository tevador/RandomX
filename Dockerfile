FROM alpine:3.10.3 AS base

FROM base AS builder

RUN apk add --no-cache build-base make cmake

WORKDIR /randomx

COPY CMakeLists.txt .
COPY src ./src

WORKDIR /randomx/build

RUN cmake -DARCH=native ..

RUN make

FROM base

RUN apk add --no-cache libstdc++

COPY --from=builder /randomx/build/randomx-benchmark .

ENTRYPOINT ["./randomx-benchmark"]
