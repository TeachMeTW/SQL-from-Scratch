# You must add all the .cpp files to the g++ command.
# note the stub.cpp here:
# g++ -std=gnu++11 -o basic_test ../_tests/_test_files/basic_test.cpp ../includes/stub/stub.cpp -Igoogletest/googletest/include -pthread -Lgoogletest/build/lib -lgtest
################################################################################


g++ -std=gnu++11 -o main       ../main.cpp                          -Igoogletest/googletest/include -pthread -Lgoogletest/build/lib -lgtest
g++ -std=gnu++11 -o basic_test ../_tests/_test_files/basic_test.cpp -Igoogletest/googletest/include -pthread -Lgoogletest/build/lib -lgtest
g++ -std=gnu++11 -o testB      ../_tests/_test_files/testB.cpp      -Igoogletest/googletest/include -pthread -Lgoogletest/build/lib -lgtest



