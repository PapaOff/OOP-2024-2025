#pragma once
#include <iostream>
#include <fstream>
#include "Task.hpp"

const int MAX_TASKS = 10;

enum Month
{
    January = 31,
    February = 28,
    March = 31,
    April = 30,
    May = 31,
    June = 30,
    July = 31,
    August = 31,
    September = 30,
    October = 31,
    November = 30,
    December = 31
};

class MonthlyPlanner
{
private:
    Month month;
    Task schedule[31][MAX_TASKS];
    int taskCount[31];

    bool overlaps(const Task &t1, const Task &t2)
    {
        int start1 = t1.getStartHour();
        int end1 = t1.getEndHour();
        int start2 = t2.getStartHour();
        int end2 = t2.getEndHour();

        return start1 < end2 && start2 < end1;
    }

public:
    MonthlyPlanner(Month m) : month(m)
    {
        for (int i = 0; i < 31; i++)
        {
            taskCount[i] = 0;
        }
    }

    bool addTask(int day, const Task &task)
    {
        if (day < 1 || day > month)
        {
            std::cout << "Invalid day!\n";
            return false;
        }
        int index = day - 1;

        if (taskCount[index] >= MAX_TASKS)
        {
            std::cout << "Task limit reached for day " << day << "\n";
            return false;
        }

        for (int i = 0; i < taskCount[index]; i++)
        {
            if (overlaps(schedule[index][i], task))
            {
                std::cout << "Task time overlaps with existing task.\n";
                return false;
            }
        }

        schedule[index][taskCount[index]++] = task;
        return true;
    }

    bool markTaskAsCompleted(int day, const char *taskDesc)
    {
        if (day < 1 || day > month)
        {
            std::cout << "Invalid day!\n";
            return false;
        }
        int index = day - 1;

        for (int i = 0; i < taskCount[index]; i++)
        {
            if (strcmp(schedule[index][i].getDescription(), taskDesc) == 0)
            {
                schedule[index][i].taskCompleted();
                return true;
            }
        }

        return false;
    }

    void printUncompletedTasks(int day) const
    {
        if (day < 1 || day > month)
        {
            std::cout << "Invalid day!\n";
            return;
        }
        int index = day - 1;

        std::cout << "Uncompleted tasks for day " << day << ":\n";
        for (int i = 0; i < taskCount[index]; i++)
        {
            if (!schedule[index][i].getStatus())
            {
                schedule[index][i].printStartAndEndTime();
                std::cout << "\n";
                std::cout << " - " << schedule[index][i].getDescription() << "\n\n";
            }
        }
    }

    void saveToBinaryFile(const char *filename) const
    {
        std::ofstream out(filename, std::ios::binary);
        if (!out)
        {
            std::cout << "Couldn't open file for writing.\n";
            return;
        }

        out.write(reinterpret_cast<const char *>(&month), sizeof(month));
        out.write(reinterpret_cast<const char *>(taskCount), sizeof(taskCount));

        for (int i = 0; i < 31; i++)
        {
            for (int j = 0; j < taskCount[i]; j++)
            {
                schedule[i][j].writeToBinaryFile(out);
            }
        }

        out.close();
    }

    void loadFromBinaryFile(const char *filename)
    {
        std::ifstream in(filename, std::ios::binary);
        if (!in)
        {
            std::cout << "Couldn't open file for reading.\n";
            return;
        }

        in.read(reinterpret_cast<char *>(&month), sizeof(month));
        in.read(reinterpret_cast<char *>(taskCount), sizeof(taskCount));

        for (int i = 0; i < 31; i++)
        {
            for (int j = 0; j < taskCount[i]; j++)
            {
                schedule[i][j].readFromBinaryFile(in);
            }
        }

        in.close();
    }
};
