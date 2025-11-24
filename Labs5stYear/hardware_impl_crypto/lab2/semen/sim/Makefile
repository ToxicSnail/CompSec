DESIGN_NAME=automata
SRC_PATH=../rtl

all: synth sim

# Design and testbench synthesis
synth:
	mkdir -p work
	iverilog -g2012 -o work/tb_$(DESIGN_NAME) $(SRC_PATH)/$(DESIGN_NAME).sv tb_$(DESIGN_NAME).sv

# Launch simulation
sim:
	./work/tb_$(DESIGN_NAME)

# Open waveform
wave:
	gtkwave work/wave.vcd \
		--rcvar 'fontname_signals Monospace 20' \
		--rcvar 'fontname_waves Monospace 20' \
		--rcvar 'scale_to_time_dimension n'

# Delete all artifacts
clean:
	rm -rf work
