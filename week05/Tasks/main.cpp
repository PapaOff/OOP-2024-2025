#include <iostream>
#include "Task.hpp"
#include "MonthlyPlanner.hpp"

int main()
{
    MonthlyPlanner planner(January);

    std::cout << "Adding tasks...\n";
    planner.addTask(5, Task("Workout", 7, 60, 800)); // 08:00 – 09:00
    planner.addTask(5, Task("Meeting", 9, 45, 930)); // 09:30 – 10:15
    planner.addTask(5, Task("Lunch", 5, 30, 1230));  // 12:30 – 13:00

    planner.addTask(10, Task("Doctor appointment", 8, 30, 1015));
    planner.addTask(10, Task("Project deadline", 10, 120, 1400));

    std::cout << "\nUncompleted tasks for day 5 before marking as done:\n";
    planner.printUncompletedTasks(5);

    planner.markTaskAsCompleted(5, "Workout");

    std::cout << "\nUncompleted tasks for day 5 after marking 'Workout' as done:\n";
    planner.printUncompletedTasks(5);

    std::cout << "\nSaving planner to binary file...\n";
    planner.saveToBinaryFile("planner.bin");

    std::cout << "\nLoading planner from binary file...\n";
    MonthlyPlanner loadedPlanner(January);
    loadedPlanner.loadFromBinaryFile("planner.bin");

    std::cout << "\nLoaded planner - Uncompleted tasks for day 5:\n";
    loadedPlanner.printUncompletedTasks(5);

    std::cout << "\nLoaded planner - Uncompleted tasks for day 10:\n";
    loadedPlanner.printUncompletedTasks(10);

    // Този таск се припокрива с Workout (08:00 – 09:00)
    planner.addTask(5, Task("Overlapping Task", 6, 60, 830)); // 08:30 – 09:30
    planner.printUncompletedTasks(5);                         // не би трябвало да е добавен

    return 0;
}
