#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include "ts_queue.hpp"
#include "item.hpp"
#include "reader.hpp"
#include "writer.hpp"
#include "producer.hpp"
#include "consumer_controller.hpp"

#define READER_QUEUE_SIZE 200
#define WORKER_QUEUE_SIZE 200
#define WRITER_QUEUE_SIZE 4000
#define CONSUMER_CONTROLLER_LOW_THRESHOLD_PERCENTAGE 20
#define CONSUMER_CONTROLLER_HIGH_THRESHOLD_PERCENTAGE 80
#define CONSUMER_CONTROLLER_CHECK_PERIOD 1000000

int main(int argc, char** argv) {
	assert(argc == 4);
	// struct timespec start, end;
	// clock_gettime(CLOCK_MONOTONIC, &start);


	int n = atoi(argv[1]);
	std::string input_file_name(argv[2]);
	std::string output_file_name(argv[3]);
	// int doExperiment = atoi(argv[4]);

	// TODO: implements main function
	// Construct
	Transformer *transformer = new Transformer();
	TSQueue<Item*>* reader_queue = NULL;
	TSQueue<Item*>* worker_queue = NULL;
	TSQueue<Item*>* writer_queue = NULL;
	ConsumerController* controller = NULL;

	// if (doExperiment == 1) {
	// 	int reader_size = atoi(argv[5]);
	// 	int worker_size = atoi(argv[6]);
	// 	int writer_size = atoi(argv[7]);
	// 	int low = atoi(argv[8]);
	// 	int high = atoi(argv[9]);
	// 	int check_period = atoi(argv[10]);
	// 	std::cout << "reader_size: " << reader_size << std::endl;
	// 	std::cout << "worker_size: " << worker_size << std::endl;
	// 	std::cout << "writer_size: " << writer_size << std::endl;
	// 	std::cout << "low: " << low << std::endl;
	// 	std::cout << "high: " << high << std::endl;
	// 	std::cout << "check_period: " << check_period << std::endl;

	// 	reader_queue = new TSQueue<Item*>(reader_size);
	// 	worker_queue = new TSQueue<Item*>(worker_size);
	// 	writer_queue = new TSQueue<Item*>(writer_size);

	// 	controller = new ConsumerController(worker_queue, writer_queue, transformer,
	// 										check_period,
	// 										worker_size * low / 100,
	// 										worker_size * high / 100);
	// } else {
	// 		std::cout << "default setting.\n";
	// reader_queue = new TSQueue<Item*>(READER_QUEUE_SIZE);
	// worker_queue = new TSQueue<Item*>(WORKER_QUEUE_SIZE);
	// writer_queue = new TSQueue<Item*>(WRITER_QUEUE_SIZE);
	// controller = new ConsumerController(worker_queue, writer_queue, transformer,
	// 									CONSUMER_CONTROLLER_CHECK_PERIOD,
	// 									WORKER_QUEUE_SIZE * CONSUMER_CONTROLLER_LOW_THRESHOLD_PERCENTAGE / 100,
	// 									WORKER_QUEUE_SIZE * CONSUMER_CONTROLLER_HIGH_THRESHOLD_PERCENTAGE / 100);
	// }



	Reader* reader = new Reader(n, input_file_name, reader_queue);

	Producer* producer0 = new Producer(reader_queue, worker_queue, transformer);
	Producer* producer1 = new Producer(reader_queue, worker_queue, transformer);
	Producer* producer2 = new Producer(reader_queue, worker_queue, transformer);
	Producer* producer3 = new Producer(reader_queue, worker_queue, transformer);

	Writer* writer = new Writer(n, output_file_name, writer_queue);

	// Transfer
	reader->start();

	producer0->start();
	producer1->start();
	producer2->start();
	producer3->start();

	controller->start();

	writer->start();


	reader->join();
	writer->join();

	delete transformer;
	delete reader_queue;
	delete worker_queue;
	delete writer_queue;
	delete reader;
	delete producer0;
	delete producer1;
	delete producer2;
	delete producer3;
	delete controller;
	delete writer;

	// clock_gettime(CLOCK_MONOTONIC, &end);
	// double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
	// std::cout << "execution time: " << elapsed << " seconds\n";
	return 0;
}
