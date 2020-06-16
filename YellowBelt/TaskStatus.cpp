//
// Created by ilya on 14.06.20.
//

#include <iostream>
#include <string>
#include <map>

using namespace std;

// Перечислимый тип для статуса задачи
enum class TaskStatus {
  NEW,          // новая
  IN_PROGRESS,  // в разработке
  TESTING,      // на тестировании
  DONE          // завершена
};

// Объявляем тип-синоним для map<TaskStatus, int>,
// позволяющего хранить количество задач каждого статуса
using TasksInfo = map<TaskStatus, int>;

class TeamTasks {
public:
  // Получить статистику по статусам задач конкретного разработчика
  const TasksInfo &GetPersonTasksInfo(const string &person) {
    if (staff.count(person) < 1) {
      return {};
    }

    for (const auto &key : {TaskStatus::NEW, TaskStatus::IN_PROGRESS, TaskStatus::TESTING, TaskStatus::DONE}) {
      if (staff.at(person).count(key) > 0 && staff.at(person).at(key) == 0) {
        staff[person].erase(key);
      }
    }

    return staff.at(person);
  }

  // Добавить новую задачу (в статусе NEW) для конкретного разработчитка
  void AddNewTask(const string &person) {
    if (staff.count(person) < 1) {
      staff[person][TaskStatus::NEW] = 0;
      staff[person][TaskStatus::IN_PROGRESS] = 0;
      staff[person][TaskStatus::TESTING] = 0;
      staff[person][TaskStatus::DONE] = 0;
    }
    ++staff[person][TaskStatus::NEW];
  }

  void CleanMaps(TasksInfo &new_tasks, TasksInfo &old_tasks, const string &person) {
    auto copy_new_tasks = new_tasks;
    for (auto &[key, value] : copy_new_tasks) {
      if (value == 0) {
        new_tasks.erase(key);
      }
    }
    auto copy_old_tasks = old_tasks;
    for (auto &[key, value] : copy_old_tasks) {
      if (value == 0) {
        old_tasks.erase(key);
      }
    }

    for (const auto &key : {TaskStatus::NEW, TaskStatus::IN_PROGRESS, TaskStatus::TESTING, TaskStatus::DONE}) {
      if (staff.at(person).count(key) > 0 && staff.at(person).at(key) == 0) {
        staff[person].erase(key);
      }
    }
  }

  // Обновить статусы по данному количеству задач конкретного разработчика,
  // подробности см. ниже
//  tuple<TasksInfo, TasksInfo> PerformPersonTasks(const string &person, int task_count) {
//    if (staff.count(person) < 1) {
//      return {{}, {}};
//    }
//
//    int updated_tasks_amount;
//    TasksInfo new_tasks, old_tasks;
//    if (task_count >= staff[person][TaskStatus::NEW]) {
//      updated_tasks_amount = staff[person][TaskStatus::NEW];
//      task_count -= staff[person][TaskStatus::NEW];
//      staff[person][TaskStatus::NEW] = 0;
//    } else {
//      updated_tasks_amount = task_count;
//      staff[person][TaskStatus::NEW] -= task_count;
//      task_count = 0;
//      old_tasks[TaskStatus::NEW] = staff[person][TaskStatus::NEW];
//    }
//
//    if (task_count == 0) {
//      old_tasks[TaskStatus::IN_PROGRESS] = staff[person][TaskStatus::IN_PROGRESS];
//      staff[person][TaskStatus::IN_PROGRESS] += updated_tasks_amount;
//      new_tasks[TaskStatus::IN_PROGRESS] = updated_tasks_amount;
//      if (staff[person].count(TaskStatus::TESTING) > 0) {
//        old_tasks[TaskStatus::TESTING] = staff[person][TaskStatus::TESTING];
//      }
//      CleanMaps(new_tasks, old_tasks, person);
//      return tie(new_tasks, old_tasks);
//    }
//    if (task_count >= staff[person][TaskStatus::IN_PROGRESS]) {
//      task_count -= staff[person][TaskStatus::IN_PROGRESS];
//      int tmp = staff[person][TaskStatus::IN_PROGRESS];
//      new_tasks[TaskStatus::IN_PROGRESS] = updated_tasks_amount;
//      staff[person][TaskStatus::IN_PROGRESS] = updated_tasks_amount;
//      updated_tasks_amount = tmp;
//    } else {
//      staff[person][TaskStatus::IN_PROGRESS] -= task_count;
//      old_tasks[TaskStatus::IN_PROGRESS] = staff[person][TaskStatus::IN_PROGRESS];
//      staff[person][TaskStatus::IN_PROGRESS] += updated_tasks_amount;
//      new_tasks[TaskStatus::IN_PROGRESS] = updated_tasks_amount;
//      updated_tasks_amount = task_count;
//      task_count = 0;
//    }
//
//    if (task_count == 0) {
//      old_tasks[TaskStatus::TESTING] = staff[person][TaskStatus::TESTING];
//      staff[person][TaskStatus::TESTING] += updated_tasks_amount;
//      new_tasks[TaskStatus::TESTING] = updated_tasks_amount;
//      CleanMaps(new_tasks, old_tasks, person);
//      return tie(new_tasks, old_tasks);
//    }
//    if (task_count >= staff[person][TaskStatus::TESTING]) {
//      task_count -= staff[person][TaskStatus::TESTING];
//      int tmp = staff[person][TaskStatus::TESTING];
//      new_tasks[TaskStatus::TESTING] = updated_tasks_amount;
//      staff[person][TaskStatus::TESTING] = updated_tasks_amount;
//      updated_tasks_amount = tmp;
//    } else {
//      staff[person][TaskStatus::TESTING] -= task_count;
//      old_tasks[TaskStatus::TESTING] = staff[person][TaskStatus::TESTING];
//      staff[person][TaskStatus::TESTING] += updated_tasks_amount;
//      new_tasks[TaskStatus::TESTING] = updated_tasks_amount;
//      updated_tasks_amount = task_count;
//      task_count = 0;
//    }
//
//    old_tasks[TaskStatus::DONE] = staff[person][TaskStatus::DONE];
//    staff[person][TaskStatus::DONE] += updated_tasks_amount;
//    new_tasks[TaskStatus::DONE] = updated_tasks_amount;
//
//    CleanMaps(new_tasks, old_tasks, person);
//
////    for (auto &[key, value] : staff[person]) {
////      if (task_count == 0 && updated_tasks_amount == 0) {
////        break;
////      }
////      if (task_count > value) {
////        task_count -= value;
////        int tmp = value;
////        new_tasks[key] = value = updated_tasks_amount;
////        updated_tasks_amount = tmp;
////      } else {
////        value -= task_count;
////        if (value != 0) {
////          old_tasks[key] = value;
////        }
////        value += updated_tasks_amount;
////        new_tasks[key] = updated_tasks_amount;
////
////        updated_tasks_amount = task_count;
////        task_count = 0;
////      }
////    }
//
//    return tie(new_tasks, old_tasks);
//  }

  std::tuple<TasksInfo, TasksInfo> PerformPersonTasks(
      const std::string& person, int task_count) {
    TasksInfo upgrade;
    TasksInfo updated;
    TasksInfo untouched;
    auto& tasks_info = staff[person];
    for (auto& item : tasks_info) {
      if (item.first == TaskStatus::NEW) {
        if (item.second > 0) {
          int new_status_count = item.second;
          while (new_status_count > 0 && task_count > 0) {
            ++updated[TaskStatus::IN_PROGRESS];
            ++upgrade[TaskStatus::IN_PROGRESS];
            --new_status_count;
            --task_count;
          }
          if (new_status_count > 0) {
            untouched[TaskStatus::NEW] += new_status_count;
            upgrade[TaskStatus::NEW] += new_status_count;
          }
        }
      }
      if (item.first == TaskStatus::IN_PROGRESS) {
        if (item.second > 0) {
          int in_progress_status_count = item.second;
          while (in_progress_status_count > 0 && task_count > 0) {
            ++updated[TaskStatus::TESTING];
            ++upgrade[TaskStatus::TESTING];
            --in_progress_status_count;
            --task_count;
          }
          if (in_progress_status_count > 0) {
            untouched[TaskStatus::IN_PROGRESS] += in_progress_status_count;
            upgrade[TaskStatus::IN_PROGRESS] += in_progress_status_count;
          }
        }
      }
      if (item.first == TaskStatus::TESTING) {
        if (item.second > 0) {
          int testing_status_count = item.second;
          while (testing_status_count > 0 && task_count > 0) {
            ++updated[TaskStatus::DONE];
            ++upgrade[TaskStatus::DONE];
            --testing_status_count;
            --task_count;
          }
          if (testing_status_count > 0) {
            untouched[TaskStatus::TESTING] += testing_status_count;
            upgrade[TaskStatus::TESTING] += testing_status_count;
          }
        }
      }
      if (item.first == TaskStatus::DONE) {
        if (item.second > 0) {
          upgrade[TaskStatus::DONE] += item.second;
        }
      }
    }
    staff[person] = upgrade;
    return std::make_tuple(updated, untouched);
  }

private:
  map<string, TasksInfo> staff;
};

// Принимаем словарь по значению, чтобы иметь возможность
// обращаться к отсутствующим ключам с помощью [] и получать 0,
// не меняя при этом исходный словарь
void PrintTasksInfo(TasksInfo tasks_info) {
  cout << tasks_info[TaskStatus::NEW] << " new tasks" <<
       ", " << tasks_info[TaskStatus::IN_PROGRESS] << " tasks in progress" <<
       ", " << tasks_info[TaskStatus::TESTING] << " tasks are being tested" <<
       ", " << tasks_info[TaskStatus::DONE] << " tasks are done" << endl;
}

int main() {
  TeamTasks tasks;

//  tasks.AddNewTask("Ilia");
//  for (int i = 0; i < 3; ++i) {
//    tasks.AddNewTask("Ivan");
//  }
//  cout << "Ilia's tasks: ";
//  PrintTasksInfo(tasks.GetPersonTasksInfo("Ilia"));
//  cout << "Ivan's tasks: ";
//  PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"));
//
//  TasksInfo updated_tasks, untouched_tasks;
//
//  tie(updated_tasks, untouched_tasks) =
//      tasks.PerformPersonTasks("Ivan", 2);
//  cout << "Updated Ivan's tasks: ";
//  PrintTasksInfo(updated_tasks);
//  cout << "Untouched Ivan's tasks: ";
//  PrintTasksInfo(untouched_tasks);
//
//  tie(updated_tasks, untouched_tasks) =
//      tasks.PerformPersonTasks("Ivan", 2);
//  cout << "Updated Ivan's tasks: ";
//  PrintTasksInfo(updated_tasks);
//  cout << "Untouched Ivan's tasks: ";
//  PrintTasksInfo(untouched_tasks);

  tasks.AddNewTask("Alice");
  tasks.AddNewTask("Alice");
  tasks.AddNewTask("Alice");
  tasks.AddNewTask("Alice");
  tasks.AddNewTask("Alice");
  tasks.PerformPersonTasks("Alice", 5);
  tasks.PerformPersonTasks("Alice", 5);
  tasks.PerformPersonTasks("Alice", 1);
  tasks.AddNewTask("Alice");
  tasks.AddNewTask("Alice");
  tasks.AddNewTask("Alice");
  tasks.AddNewTask("Alice");
  tasks.AddNewTask("Alice");
  tasks.PerformPersonTasks("Alice", 2);
  tasks.GetPersonTasksInfo("Alice");
  tasks.PerformPersonTasks("Alice", 4);
  tasks.GetPersonTasksInfo("Alice");

  return 0;
}