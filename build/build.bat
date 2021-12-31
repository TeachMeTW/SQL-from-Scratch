# You must add all the .cpp files to the g++ command.
# note the stub.cpp here:
# g++ -std=gnu++11 -o basic_test ../_tests/_test_files/basic_test.cpp ../includes/stub/stub.cpp -Igoogletest/googletest/include -pthread -Lgoogletest/build/lib -lgtest
################################################################################


g++ -std=gnu++11 -o main       ../main.cpp ../includes/sql/sql.cpp  ../includes/table/table.cpp   ../includes/binary_files/utilities.cpp  ../includes/tokenizer/state_machine_functions.cpp       ../includes/binary_files/file_record.cpp      ../includes/parser/parser.cpp ../includes/token/token.cpp  ../includes/tokenizer/ftokenize.cpp    ../includes/tokenizer/stokenize.cpp     -Igoogletest/googletest/include -pthread -Lgoogletest/build/lib -lgtest
g++ -std=gnu++11 -o basic_test ../_tests/_test_files/basic_test.cpp  ../includes/sql/sql.cpp ../includes/table/table.cpp  ../includes/binary_files/utilities.cpp  ../includes/tokenizer/state_machine_functions.cpp   ../includes/binary_files/file_record.cpp  ../includes/parser/parser.cpp ../includes/token/token.cpp   ../includes/tokenizer/ftokenize.cpp  ../includes/tokenizer/stokenize.cpp -Igoogletest/googletest/include -pthread -Lgoogletest/build/lib -lgtest
g++ -std=gnu++11 -o testB      ../_tests/_test_files/testB.cpp   ../includes/sql/sql.cpp ../includes/table/table.cpp   ../includes/binary_files/utilities.cpp  ../includes/tokenizer/state_machine_functions.cpp ../includes/binary_files/file_record.cpp ../includes/parser/parser.cpp  ../includes/token/token.cpp  ../includes/tokenizer/ftokenize.cpp ../includes/tokenizer/stokenize.cpp -Igoogletest/googletest/include -pthread -Lgoogletest/build/lib -lgtest



