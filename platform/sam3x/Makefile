BOARD_FQBN := arduino:sam:arduino_due_x_dbg
ROOT_DIR = $(dir $(abspath $(firstword $(MAKEFILE_LIST))))../../src
PORT = $(shell arduino-cli board list | grep ${BOARD_FQBN} | awk '{print $$1;}')

sam3x:
	arduino-cli compile --fqbn ${BOARD_FQBN} --libraries ${ROOT_DIR}

upload:
ifeq (${PORT},)
	# TODO: Handle multiple boards
	$(error Failed upload: board ${BOARD_FQBN} not found!)
else
	arduino-cli upload --port ${PORT} --fqbn ${BOARD_FQBN} sam3x
endif
