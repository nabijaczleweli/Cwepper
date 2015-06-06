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


SUBMODULES_GIT := $(shell git submodule status --recursive | sed "s/[ +-][0-9a-f]* //g" | sed "s/ .*//g")
SUBSYSTEMS_SFML := system window graphics
CUSTOM_DLLS := $(foreach submod,$(SUBMODULES_GIT),$(wildcard $(submod)/$(OUTDIR)*$(DLL)))
LDDLLS := audiere $(foreach subsystem,$(SUBSYSTEMS_SFML),sfml-$(subsystem)-2) $(foreach custdll,$(CUSTOM_DLLS),$(basename $(notdir $(custdll))))
LDAR := -fpic $(foreach custdll,$(CUSTOM_DLLS),-L"$(dir $(custdll))") $(foreach dll,$(LDDLLS),-l$(dll))
SOURCES := $(sort $(filter-out ./ext/%,$(shell find src -name *.cpp)))


.PHONY : clean all release git travis-ext-check


travis-ext-check :
	ls || :
	ls ext  || :
	ls ext/all || :

all : $(subst $(SRCDIR),$(OBJDIR),$(subst .cpp,$(OBJ),$(SOURCES)))
	$(CXX) $(CPPAR) -o$(OUTDIR)Cwepper$(EXE) $(subst $(SRCDIR),$(OBJDIR),$^) $(LDAR)
	@cp -ur $(ASSETDIR) $(OUTDIR)

clean :
	rm -rf $(OUTDIR) $(RELEASEDIR)

release : clean git travis-ext-check all
	@$(MKDIR) $(RELEASEDIR)
	cp $(OUTDIR)Cwepper$(EXE) $(RELEASEDIR)
	cp --target-directory=$(RELEASEDIR) $(foreach lib,$(filter-out $(foreach custdll,$(CUSTOM_DLLS),$(basename $(notdir $(custdll)))), \
	                                              $(LDDLLS) libgcc_s_dw2-1 libstdc++-6), $(DLLDIR)$(lib)$(DLL)) $(CUSTOM_DLLS)
	$(STRIP) $(STRIPAR) $(RELEASEDIR)/*$(EXE) $(RELEASEDIR)/*$(DLL)
	7z a -r -y $(RELEASEDIR)/release.zip $(RELEASEDIR)/*

git :
	git submodule    update  --recursive --init --remote
	git submodule -q foreach --recursive "make --silent --no-print-directory dll"
	@rm -rf "ext/all/*"
	@$(MKDIR) "ext/all" 1>$(devnull) 2>$(devnull) || :
	@echo $(SUBMODULES_GIT)
	@echo $(foreach submod,$(SUBMODULES_GIT),$(notdir $(submod)))
	$(foreach submod,$(SUBMODULES_GIT),ln -s "$(subst \,/,$(shell pwd))/$(submod)/src" "$(subst \,/,$(shell pwd))/ext/all/$(notdir $(submod))" 1>$(devnull) \
	                                                                                                                                           2>$(devnull) &) :


$(OBJDIR)%$(OBJ) : $(SRCDIR)%.cpp
	@$(MKDIR) -p $(dir $@) || :
	$(CXX) $(CPPAR) -c -o$@ $^
