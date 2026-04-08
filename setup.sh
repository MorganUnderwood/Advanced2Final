!#/bin/bash

echo "Updating..."
sudo apt update

echo "Installing necessary libraries"
sudo apt install -y libsdl2-dev

echo "Finished!"

# To use, either:
#
# chmod +x setup.sh, then sudo ./setup.sh
# OR just sudo bash setup.sh
