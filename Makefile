EIGEN_INCLUDE=-I/opt/local/include/eigen2 $(shell pkg-config --cflags-only-I eigen2) -I$(shell pwd)/eigen
INCLUDES= -I../iniparser/src -I../iniconfig -I../jsoncpp/include $(EIGEN_INCLUDE)
CFLAGS=$(INCLUDES) -DHRD # -DpomerolHDF5
CXXFLAGS=$(CFLAGS)

export CFLAGS
export CXXFLAGS

all : iniparser iniconfig jsoncpp libpomerol

lib:
	@mkdir -p lib

.PHONY: iniparser
iniparser: lib
	@echo "Building iniparser"
	cd iniparser && $(MAKE) libiniparser.a
	@mv iniparser/libiniparser.a ./lib
	@echo

.PHONY: iniconfig
iniconfig: lib
	@echo "Building iniconfig"
	cd iniconfig && $(MAKE)
	@mv iniconfig/libiniconfig.a ./lib
	@echo

.PHONY: jsoncpp
jsoncpp: lib
	@echo "Bulding jsoncpp"
	cd jsoncpp && $(MAKE)
	@mv jsoncpp/libs/lib_json.a ./lib
	@echo

.PHONY: libpomerol
libpomerol:	lib
	@echo "Building libpomerol"
	cd src && $(MAKE)
	cp src/libpomerol.a ./lib
	@echo

clean:
	cd src && $(MAKE) clean
	cd iniparser && $(MAKE) clean
	cd iniconfig && $(MAKE) clean
	cd jsoncpp && $(MAKE) clean

devel:
	cd src && $(MAKE) devel

.PHONY: doc-html
doc-html:
	doxygen doc/Doxyfile
