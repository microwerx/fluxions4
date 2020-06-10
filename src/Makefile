SOURCES = $(wildcard *.vert) $(wildcard *.frag)
SPVVERTHIN = $(patsubst %.vert,%.vert.spv.h.in, $(SOURCES))
SPVFRAGHIN = $(patsubst %.frag,%.frag.spv.h.in, $(SOURCES))

.PHONY: all

all: $(SPVVERTHIN) $(SPVFRAGHIN)

vkcube.frag.spv.h.in: vkcube.frag
	glslc $< -mfmt=num -o $@

vkcube.vert.spv.h.in: vkcube.vert
	glslc $< -mfmt=num -o $@

%.frag.spv.h.in: %.frag
	glslc $< -mfmt=num -o $@

%.vert.spv.h.in: %.vert
	glslc $< -mfmt=num -o $@
