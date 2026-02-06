DESIGN_NAME=mealy
SRC_PATH=../rtl

all: synth sim

# Build design + testbench
synth:
	@mkdir -p work
	@iverilog -g2012 -o work/tb_$(DESIGN_NAME) \
		$(SRC_PATH)/$(DESIGN_NAME).sv \
		tb_$(DESIGN_NAME).sv

# Run simulation
sim:
	./work/tb_$(DESIGN_NAME)

# Open waveform (requires saved GTKWave config or use File->Open VCD)
wave:
	gtkwave work/mealy2.vcd \
		--rcvar 'fontname_signals Monospace 20' \
		--rcvar 'fontname_waves Monospace 20' \
		--rcvar 'scale_to_time_dimension n'

# Cleanup
clean:
	rm -rf work
