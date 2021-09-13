mkdir build
cd build && cmake ..
make

./MyTests --gtest_output=xml

gcovr -r .. --output coverage.xml --xml

mkdir coverage
gcovr -r .. --html --html-details -o coverage/example-html-details.html
