!#/bin/bash

echo "Updating..."
sudo apt update

echo "Installing necessary libraries"
sudo apt install -y libsdl2-dev

echo "Finished"

# To use either:
#
# chmod +x setup.ssh, then sudo ./setup.ssh
# OR just sudo bash setup.ssh
