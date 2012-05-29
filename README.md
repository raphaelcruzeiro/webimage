###Setup Instructions

http://news.ycombinator.com/item?id=4034671

git clone https://github.com/raphaelcruzeiro/webimage.git
sudo aptitude install -y xvfb
sudo aptitude install -y libqtwebkit-dev
cd webimage
qmake webimage.pro
make all Makefile
xvfb-run --server-args="-screen 0, 1024x768x24" ./webimage http://news.ycombinator.com/ output.jpg 1024

