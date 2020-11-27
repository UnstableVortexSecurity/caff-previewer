FROM alpine:latest AS build

ADD . /build/
WORKDIR /build

RUN apk update && apk add make cmake gcc musl-dev && cmake . && make -j 8

FROM alpine:latest

COPY --from=build /build/caff_previewer /usr/local/bin/

CMD "/usr/local/bin/caff_previewer"