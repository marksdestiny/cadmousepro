IMAGE_NAME := cadmousepro-dev
CONTAINER_WORKDIR := /workspace

.PHONY: image build build-daemon build-gui rebuild clean shell

image:
	docker build -t $(IMAGE_NAME) .

build-daemon:
	docker run --rm \
		-v "$(PWD):$(CONTAINER_WORKDIR)" \
		-w $(CONTAINER_WORKDIR) \
		$(IMAGE_NAME) \
		bash -c 'cd daemon && mkdir -p build && cd build && cmake .. && make'

build-gui:
	docker run --rm \
		-v "$(PWD):$(CONTAINER_WORKDIR)" \
		-w $(CONTAINER_WORKDIR) \
		$(IMAGE_NAME) \
		bash -c 'cd gui && mkdir -p build && cd build && cmake .. && make'

build: build-daemon build-gui

rebuild: clean build-daemon build-gui

clean:
	rm -rf daemon/build
	rm -rf gui/build

shell:
	docker run --rm -it \
		-v "$(PWD):$(CONTAINER_WORKDIR)" \
		-w $(CONTAINER_WORKDIR) \
		$(IMAGE_NAME)