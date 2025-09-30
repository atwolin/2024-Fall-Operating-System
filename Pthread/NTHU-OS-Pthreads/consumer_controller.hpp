#include <pthread.h>
#include <unistd.h>
#include <vector>
#include <iostream>
#include "consumer.hpp"
#include "ts_queue.hpp"
#include "item.hpp"
#include "transformer.hpp"

#ifndef CONSUMER_CONTROLLER
#define CONSUMER_CONTROLLER

class ConsumerController : public Thread {
public:
	// constructor
	ConsumerController(
		TSQueue<Item*>* worker_queue,
		TSQueue<Item*>* writer_queue,
		Transformer* transformer,
		int check_period,
		int low_threshold,
		int high_threshold
	);

	// destructor
	~ConsumerController();

	virtual void start();

private:
	std::vector<Consumer*> consumers;

	TSQueue<Item*>* worker_queue;
	TSQueue<Item*>* writer_queue;

	Transformer* transformer;

	// Check to scale down or scale up every check period in microseconds.
	int check_period;
	// When the number of items in the worker queue is lower than low_threshold,
	// the number of consumers scaled down by 1.
	int low_threshold;
	// When the number of items in the worker queue is higher than high_threshold,
	// the number of consumers scaled up by 1.
	int high_threshold;

	static void* process(void* arg);
};

// Implementation start

ConsumerController::ConsumerController(
	TSQueue<Item*>* worker_queue,
	TSQueue<Item*>* writer_queue,
	Transformer* transformer,
	int check_period,
	int low_threshold,
	int high_threshold
) : worker_queue(worker_queue),
	writer_queue(writer_queue),
	transformer(transformer),
	check_period(check_period),
	low_threshold(low_threshold),
	high_threshold(high_threshold) {
}

ConsumerController::~ConsumerController() {}

void ConsumerController::start() {
	// TODO: starts a ConsumerController thread
	pthread_create(&t, 0, ConsumerController::process, (void*)this);
}

void* ConsumerController::process(void* arg) {
	// TODO: implements the ConsumerController's work
	// In the beginning, no Consumer thread is created by ConsumerController
	// Check Worker Queue status periodically
	// Worker Queue size > high_threshold -> new Consumer
	// Worker Queue size < low_threshold -> call Consumer->cancel
	ConsumerController *controller = (ConsumerController*)arg;
	while (1) {
		usleep(controller->check_period);

		if (controller->worker_queue->get_size() > controller->high_threshold) {
			Consumer *one_worker = new Consumer(controller->worker_queue, controller->writer_queue, controller->transformer);

			controller->consumers.push_back(one_worker);
			one_worker->start();


			std::cout << "Scaling up consumers from " << controller->consumers.size() - 1
					  << " to " << controller->consumers.size() << "\n";

		} else if (controller->worker_queue->get_size() < controller->low_threshold &&
		           controller->consumers.size() > 1) {
			Consumer *one_worker = controller->consumers.back();

			controller->consumers.pop_back();
			one_worker->cancel();

			std::cout << "Scaling down consumers from " << controller->consumers.size() + 1
					  << " to " << controller->consumers.size() << "\n";
		}
	}
}

#endif // CONSUMER_CONTROLLER_HPP
