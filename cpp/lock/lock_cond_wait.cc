#include <iostream>
#include <thread>
#include <condition_variable>

std::mutex mutex;
std::condition_variable condVar;

void consumer() {
  std::cout << "Waiting for work" << std::endl;
  std::unique_lock<std::mutex> lck(mutex);
  condVar.wait(lck); // noncompliant: can wait forever as the order between t1 and t2 is not guaranteed
  std::cout << "Doing some work" << std::endl;
}

void producer() {
  std::cout << "Work submited" << std::endl;
  condVar.notify_one(); // this can be executed before or after the wait in consumer, no guarantee
}

int main() {
  std::thread t1(consumer);
  std::thread t2(producer);

  t1.join();
  t2.join();
}