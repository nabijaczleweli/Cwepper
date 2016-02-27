# The MIT License (MIT)

# Copyright (c) 2014 nabijaczleweli

# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


include configMakefile


SUBSYSTEMS_SFML := system window graphics
LDDLLS := $(foreach subsystem,$(SUBSYSTEMS_SFML),sfml-$(subsystem)) cppformat
#        ^ audiere
LDAR := $(PIC) -L$(OUTDIR)ext $(foreach dll,$(LDDLLS),-l$(subst $(PREDLL),,$(dll))) $(SFML_LINKLIB)
SOURCES := $(wildcard src/*.cpp) $(wildcard src/**/*.cpp) $(wildcard src/**/**/*.cpp) $(wildcard src/**/**/**/*.cpp)


.PHONY : all exe clean cppformat


all : cppformat exe

exe : $(patsubst $(SRCDIR)%.cpp,$(OBJDIR)%$(OBJ),$(SOURCES))
	$(CXX) $(CXXAR) -o$(OUTDIR)Cwepper $(subst $(SRCDIR),$(OBJDIR),$^) $(LDAR)
	@cp -ur $(ASSETDIR) $(OUTDIR)

clean :
	rm -rf $(OUTDIR)

cppformat : $(OUTDIR)ext/libcppformat$(ARCH)


$(OUTDIR)ext/libcppformat$(ARCH) : $(patsubst %.cc,$(OBJDIR)%$(OBJ),$(wildcard ext/cppformat/cppformat/*.cc))
	@mkdir -p $(dir $@)
	$(AR) cr $@ $^


$(OBJDIR)%$(OBJ) : $(SRCDIR)%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXAR) -Iext/cppformat -Iext/Eigen -Iext/cereal/include $(SFML_HEADERS) -DCWEPPER_CEREAL_VERSION='$(CEREAL_VERSION)' -DCWEPPER_CPPFORMAT_VERSION='$(CPPFORMAT_VERSION)' -c -o$@ $^

$(OBJDIR)ext/cppformat/%$(OBJ) : ext/cppformat/%.cc
	@mkdir -p $(dir $@)
	$(CXX) $(CXXAR) -c -o$@ $^
