#UPDATE
git fetch upstream
git checkout beta
git merge upstream/beta
git checkout ina3221_enhanced
git merge beta



#COMPILE
pio --version
pip3 install --user -U platformio
~/.local/bin/pio --version
echo 'export PATH=$PATH:~/.local/bin' >> ~/.bashrc
source ~/.bashrc
pio run

