!#/bin.bash

echo "Updating...."
sudo apt update


echo "SDL Libraries."

sudo apt install -y libsdl2-dev libsdl2-ttf-dev libsdl2-image-dev git

echo "Installing WiringPi.h"
git clone https://github.com/WiringPi/WiringPi
cd WiringPi
./build

echo
"Finished!"

#to use either:
#
#chmod x+ setup.sh, then sudo ./setup.sh
OR sudo bash setup.sh