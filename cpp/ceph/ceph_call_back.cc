#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>
#include <condition_variable>

using namespace std;

class Context {
protected:
  virtual void finish(int r) = 0;
public:
  virtual ~Context() {};
  virtual void complete(int r) {
    finish(r);
    delete this;
  }
};

class C_SafeCond : public Context {
  mutex *lock;
  condition_variable *cond;
  bool *done;
  int *rval;
public:
  C_SafeCond(mutex *l, condition_variable *c, bool *d, int *r=0)
    : lock(l), cond(c), done(d), rval(r) {
      *done = false;
  }
  void finish(int r) override {
    lock->lock();
    if (rval) *rval = r;
    *done = true;
    cond->notify_one();
    lock->unlock();
  }
};

void do_something(Context *onack) {
  this_thread::sleep_for(chrono::seconds(10));
  onack->complete(0);
}

int main() {
  bool done;
  int r;
  mutex mtx;
  condition_variable cond;
  unique_lock<mutex> lck(mutex);
  
  Context *onack = new C_SafeCond(&mtx, &cond, &done, &r);
  thread func_thread(do_something, onack);  // 模拟调用异步业务
  
  while (!done) {
    cond.wait(lck);
  }
  func_thread.join();
  return r;
}

// clear; g++ ceph_call_back.cc -std=c++11 -pthread