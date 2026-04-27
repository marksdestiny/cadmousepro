APP_DAEMON := cadmousepro
APP_GUI := cadmousepro-gui

IMAGE_NAME := cadmousepro-dev
CONTAINER_WORKDIR := /workspace

UID := $(shell id -u)
GID := $(shell id -g)

PREFIX ?= /usr/local
BINDIR := $(PREFIX)/bin
DBUSCONFIG := /etc/dbus-1/system.d/info.globalcode.mouse.cadMousePro.conf

.PHONY: image build build-daemon build-gui rebuild clean shell install uninstall

image:
	docker build -t $(IMAGE_NAME) .

build-daemon:
	docker run --rm \
		-u $(UID):$(GID) \
		-v "$(PWD):$(CONTAINER_WORKDIR)" \
		-w $(CONTAINER_WORKDIR) \
		$(IMAGE_NAME) \
		bash -c 'cd daemon && mkdir -p build && cd build && cmake .. && make'
	cp daemon/build/src/$(APP_DAEMON) $(APP_DAEMON)

build-gui:
	docker run --rm \
		-u $(UID):$(GID) \
		-v "$(PWD):$(CONTAINER_WORKDIR)" \
		-w $(CONTAINER_WORKDIR) \
		$(IMAGE_NAME) \
		bash -c 'cd gui && mkdir -p build && cd build && cmake .. && make'
	cp gui/build/src/$(APP_GUI) $(APP_GUI)

build: build-daemon build-gui

rebuild: clean build-daemon build-gui

clean:
	rm -rf daemon/build
	rm -rf gui/build
	rm -f $(APP_DAEMON)
	rm -f $(APP_GUI)

shell:
	docker run --rm -it \
		-u $(UID):$(GID) \
		-v "$(PWD):$(CONTAINER_WORKDIR)" \
		-w $(CONTAINER_WORKDIR) \
		$(IMAGE_NAME)

install: 
	install $(APP_DAEMON) $(BINDIR)/$(APP_DAEMON)
	install $(APP_GUI) $(BINDIR)/$(APP_GUI)
	install -m 644 config$(DBUSCONFIG) $(DBUSCONFIG)

uninstall:
	rm -f $(BINDIR)/$(APP_DAEMON)
	rm -f $(BINDIR)/$(APP_GUI)
	rm -f $(DBUSCONFIG)