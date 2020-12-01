ROOT_DIR = $(dir $(abspath $(firstword $(MAKEFILE_LIST))))../../src

sam3x:
	arduino-cli compile --fqbn arduino:sam:arduino_due_x_dbg --libraries ${ROOT_DIR}

# upload sketch: arduino-cli upload --port /dev/ttyACM0 --fqbn arduino:sam:arduino_due_x_dbg sam3x.ino 


