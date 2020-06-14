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
  const TasksInfo &GetPersonTasksInfo(const string &person) const {
    auto copy = TasksInfo(staff.at(person));
    for (auto &[key, value] : copy) {
      if (value == 0) {
        copy.erase(key);
      }
    }
    return copy;
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

  // Обновить статусы по данному количеству задач конкретного разработчика,
  // подробности см. ниже
  tuple<TasksInfo, TasksInfo> PerformPersonTasks(const string &person, int task_count) {
    if (staff.count(person) < 1) {
      return {{},
              {}};
    }

    int updated_tasks_amount = 0;
    TasksInfo new_tasks, old_tasks;
    for (auto &[key, value] : staff[person]) {
      if (task_count == 0 && updated_tasks_amount == 0) {
        break;
      }
      if (task_count > value) {
        task_count -= value;
        int tmp = value;
        new_tasks[key] = value = updated_tasks_amount;
        updated_tasks_amount = tmp;
      } else {
        value -= task_count;
        if (value != 0) {
          old_tasks[key] = value;
        }
        value += updated_tasks_amount;
        new_tasks[key] = updated_tasks_amount;

        updated_tasks_amount = task_count;
        task_count = 0;
      }
    }

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


    return tie(new_tasks, old_tasks);
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
  tasks.AddNewTask("Ilia");
  for (int i = 0; i < 3; ++i) {
    tasks.AddNewTask("Ivan");
  }
  cout << "Ilia's tasks: ";
  PrintTasksInfo(tasks.GetPersonTasksInfo("Ilia"));
  cout << "Ivan's tasks: ";
  PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"));

  TasksInfo updated_tasks, untouched_tasks;

  tie(updated_tasks, untouched_tasks) =
      tasks.PerformPersonTasks("Ivan", 2);
  cout << "Updated Ivan's tasks: ";
  PrintTasksInfo(updated_tasks);
  cout << "Untouched Ivan's tasks: ";
  PrintTasksInfo(untouched_tasks);

  tie(updated_tasks, untouched_tasks) =
      tasks.PerformPersonTasks("Ivan", 2);
  cout << "Updated Ivan's tasks: ";
  PrintTasksInfo(updated_tasks);
  cout << "Untouched Ivan's tasks: ";
  PrintTasksInfo(untouched_tasks);

  return 0;
}