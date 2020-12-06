BOARD_NAME := arduino:sam:arduino_due_x_dbg
ROOT_DIR = $(dir $(abspath $(firstword $(MAKEFILE_LIST))))../../src

sam3x:
	arduino-cli compile --fqbn ${BOARD_NAME} --libraries ${ROOT_DIR}

upload: 
	arduino-cli upload --port /dev/ttyACM0 --fqbn ${BOARD_NAME} sam3x
