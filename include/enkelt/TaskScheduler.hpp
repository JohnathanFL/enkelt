#pragma once
#include <atomic>
#include <functional>
#include <thread>
#include <vector>


/// \struct Task
/// \brief Inherit from this task
struct Task {
   virtual ~Task() {}
   virtual void doTask() = 0;
};


class TaskScheduler {
  public:
   TaskScheduler(size_t numThreads) : threadPool{numThreads} {
      for (auto i = 0; i < numThreads; i++)
         threadPool[i] = std::thread{threadFunc, threadStatus[i]};
   }

   void addTask(Task t);
   void runTasks() {}

  private:
   enum class TaskFlags {
      Waiting,     ///>! Thread is waiting on the manager for more work.
      HasWork,     ///>! Manager has given the thread work.
      Running,     ///>! Thread has received work from manager, is now running it.
      ShouldQuit,  ///>! Manager no longer has any need for this thread, and wishes to tie up loose ends.
   };

   struct TaskState {
      // TaskState(const TaskState& rhs) : state{rhs.state}, taskList{rhs.taskList}, numTasks{rhs.numTasks} {}
      TaskState(TaskFlags f) : state{f} {}
      TaskFlags                          state;
      std::vector<std::unique_ptr<Task>> taskList;
      size_t                             numTasks;
   };

   static void threadFunc(std::atomic<TaskState>& flag) {
      TaskState state = std::atomic_load(&flag);  // Taking only once each loop, so we don't access to atomic too often.
      while ((state = std::atomic_load(&flag)).state != TaskFlags::ShouldQuit) {
         auto taskList = state.taskList;

         if (state.state == TaskFlags::Waiting)  // If we're waiting for tasks, just mark this thread as useless
            std::this_thread::yield();
         else if (state.state == TaskFlags::HasWork) {       // If we are signalled as having work...
            std::atomic_store(&flag, {TaskFlags::Running});  // ...signal that we've acknowledged this
            for (size_t i = 0; i < state.numTasks; i++)      // ...and run each task
               taskList[i]->doTask();

            // Making sure to signal that we're done afterwards
            std::atomic_store(&flag, {TaskFlags::Waiting});
         }
      }
   }
   char flags[4] = {'W', 'H', 'R', 'S'};  // Each ADDRESS, not the value itself, corresponds to the flag

   std::vector<std::thread>            threadPool;
   std::vector<std::atomic<TaskState>> threadStatus;
};


void TaskScheduler::addTask(Task t) { taskList.push_back(t); }
