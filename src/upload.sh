
main(){
local TARG=$1
PORT=/dev/cu.usbmodem1411
BOARDNAME=arduino:avr:uno

arduino-cli compile --fqbn $BOARDNAME $TARG
arduino-cli upload -p $PORT --fqbn $BOARDNAME $TARG
}

main "$@"
