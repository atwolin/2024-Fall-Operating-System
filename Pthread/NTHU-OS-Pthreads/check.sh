make clean

scl enable devtoolset-8 'make'

####################################################
####        n = 200 gen transfomer              ####
####################################################
# ./scripts/auto_gen_transformer --input ./tests/00_spec.json --output transformer.cpp


####################################################
####             n = 200 verify                 ####
####################################################
# ./main 200 ./tests/00.in ./tests/00.out
./scripts/verify --output ./tests/00.out --answer ./tests/00.ans


####################################################
####        n = 40000 gen transfomer            ####
####################################################
# ./scripts/auto_gen_transformer --input ./tests/01_spec.json --output transformer.cpp


####################################################
####           n = 40000 verify                 ####
####################################################
# ./main 4000 ./tests/01.in ./tests/01.out
# ./scripts/verify --output ./tests/01.out --answer ./tests/01.ans






####################################################
####                     exp                    ####
####################################################

# ./main 200 ./tests/00.in ./tests/00.out 0 > ex00.txt
# ./main 4000 ./tests/01.in ./tests/01.out 0 > ex10.txt


####   1. CONSUMER_CONTROLLER_CHECK_PERIOD
# ./main 200 ./tests/00.in ./tests/00.out 1 200 200 4000 20 80 10000 > ex01.1.txt
# ./main 200 ./tests/00.in ./tests/00.out 1 200 200 4000 20 80 100 > ex01.2.txt
# ./main 200 ./tests/00.in ./tests/00.out 1 200 200 4000 20 50 100 > ex01.2.5.txt
# ./main 200 ./tests/00.in ./tests/00.out 1 200 200 4000 20 80 100000000 > ex01.3.txt

####   2. CONSUMER_CONTROLLER_LOW_THRESHOLD_PERCENTAGE
# ./main 200 ./tests/00.in ./tests/00.out 1 200 200 4000 5 80 1000000 > ex02.1.txt
# ./main 200 ./tests/00.in ./tests/00.out 1 200 200 4000 35 80 1000000 > ex02.2.txt


####   3. CONSUMER_CONTROLLER_HIGH_THRESHOLD_PERCENTAGE
# ./main 200 ./tests/00.in ./tests/00.out 1 200 200 4000 20 50 1000000 > ex03.1.txt


####   4. WORKER_QUEUE_SIZE
# ./main 200 ./tests/00.in ./tests/00.out 1 200 100 4000 20 80 1000000 > ex04.1.txt
# ./main 200 ./tests/00.in ./tests/00.out 1 200 400 4000 20 80 1000000 > ex04.2.txt
# ./main 4000 ./tests/01.in ./tests/01.out 1 200 4000 4000 20 80 1000000 > ex04.3.txt


####   5. WRITER_QUEUE_SIZE very small
# ./main 200 ./tests/00.in ./tests/00.out 1 200 200 1 20 80 1000000 > ex05.1.txt
# ./main 4000 ./tests/01.in ./tests/01.out 1 200 200 1 20 80 1000000 > ex05.2.txt


####   6. READER_QUEUE_SIZE very small
# ./main 200 ./tests/00.in ./tests/00.out 1 1 200 4000 20 80 1000000 > ex06.1.txt
# ./main 4000 ./tests/01.in ./tests/01.out 1 1 200 4000 20 80 1000000 > ex06.2.txt
