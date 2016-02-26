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
LDAR := $(PIC) -L$(OUTDIR)ext $(foreach dll,$(LDDLLS),-l$(subst $(PREDLL),,$(dll)))
SOURCES := $(wildcard src/*.cpp) $(wildcard src/**/*.cpp) $(wildcard src/**/**/*.cpp) $(wildcard src/**/**/**/*.cpp)


.PHONY : clean all release git cppformat exe


all : cppformat exe

exe : $(patsubst $(SRCDIR)%.cpp,$(OBJDIR)%$(OBJ),$(SOURCES))
	$(CXX) $(CXXAR) -o$(OUTDIR)Cwepper$(EXE) $(subst $(SRCDIR),$(OBJDIR),$^) $(LDAR)
	@cp -ur $(ASSETDIR) $(OUTDIR)

clean :
	rm -rf $(OUTDIR) $(RELEASEDIR)

release : clean all
	@$(MKDIR) $(RELEASEDIR)
	cp $(OUTDIR)Cwepper$(EXE) $(RELEASEDIR)
	cp --target-directory=$(RELEASEDIR) $(foreach lib,$(filter-out $(foreach custdll,$(CUSTOM_DLLS),$(basename $(notdir $(custdll)))), \
	                                    $(foreach dll,$(LDDLLS) $(OSDLL),$(PREDLL)$(subst $(PREDLL),,$(dll)))), $(DLLDIR)$(lib)$(DLL)) $(CUSTOM_DLLS)
	$(STRIP) $(STRIPAR) $(RELEASEDIR)/*$(EXE) $(RELEASEDIR)/*$(DLL)
	cp -ur $(ASSETDIR) $(RELEASEDIR)
	tar -cjv $(RELEASEDIR)/* > release.tar.bz2

cppformat : $(OUTDIR)ext/libcppformat$(ARCH)


$(OUTDIR)ext/libcppformat$(ARCH) : $(patsubst %.cc,$(OBJDIR)%$(OBJ),$(wildcard ext/cppformat/cppformat/*.cc))
	@mkdir -p $(dir $@)
	$(AR) cr $@ $^


$(OBJDIR)%$(OBJ) : $(SRCDIR)%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXAR) -Iext/cppformat -Iext/Eigen -Iext/cereal/include -c -o$@ $^

$(OBJDIR)ext/cppformat/%$(OBJ) : ext/cppformat/%.cc
	@mkdir -p $(dir $@)
	$(CXX) $(CXXAR) -c -o$@ $^
