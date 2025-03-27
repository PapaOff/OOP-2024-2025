#pragma once
#include <iostream>
#include <fstream>
#include <cstring>

class Task
{
private:
    static const int MAX_PRIORITY = 10;

    char *description;
    int priority;
    bool status;
    int duration;  // в минути
    int startHour; // във формат HHMM, тоест от 0000 до 2359

    void copy(const Task &other)
    {
        int length = strlen(other.description) + 1;
        description = new char[length];
        strcpy(description, other.description);
        priority = other.priority;
        status = other.status;
        duration = other.duration;
        startHour = other.startHour;
    }

    void destroy()
    {
        delete[] description;
    }

public:
    Task() : priority(0), status(false), duration(0), startHour(0)
    {
        description = new char[1];
        description[0] = '\0';
    }

    Task(const char *descr, int priority, int duration, int startHour)
    {
        int length = strlen(descr) + 1;
        description = new char[length];
        strcpy(description, descr);

        this->priority = (priority > MAX_PRIORITY) ? MAX_PRIORITY : priority;
        this->status = false;
        this->duration = duration;

        int hour = startHour / 100;
        int minutes = startHour % 100;
        if (hour >= 0 && hour <= 23 && minutes >= 0 && minutes < 60)
        {
            this->startHour = startHour;
        }
        else
        {
            std::cout << "Invalid startHour format!\n";
            this->startHour = 0;
        }
    }

    Task(const Task &other)
    {
        copy(other);
    }

    Task &operator=(const Task &other)
    {
        if (this != &other)
        {
            destroy();
            copy(other);
        }
        return *this;
    }

    ~Task()
    {
        destroy();
    }

    void taskCompleted()
    {
        status = true;
    }

    void setDuration(int duration)
    {
        if (duration > 0)
        {
            this->duration = duration;
        }
        else
        {
            std::cout << "Duration must be a positive value" << std::endl;
        }
    }

    void setPriority(int priority)
    {
        if (priority > 0 && priority <= MAX_PRIORITY)
        {
            this->priority = priority;
        }
        else
        {
            std::cout << "Priority must be between 1 and " << MAX_PRIORITY << std::endl;
        }
    }

    const char *getDescription() const
    {
        return description;
    }

    int getPriority() const
    {
        return priority;
    }

    bool getStatus() const
    {
        return status;
    }

    int getDuration() const
    {
        return duration;
    }

    int getStartHour() const
    {
        return startHour;
    }

    int getEndHour() const
    {
        int hour = startHour / 100;
        int minutes = startHour % 100;

        minutes += duration;
        hour += minutes / 60;
        minutes %= 60;

        return hour * 100 + minutes;
    }

    Task compare(const Task &task)
    {
        if (priority > task.priority)
        {
            return *this;
        }
        if (priority < task.priority)
        {
            return task;
        }
        if (duration <= task.duration)
        {
            return *this;
        }
        return task;
    }

    void writeToBinaryFile(std::ofstream &out) const
    {
        if (!out)
        {
            std::cout << "Couldn't write to file." << std::endl;
            return;
        }

        int length = strlen(description) + 1;
        out.write(reinterpret_cast<const char *>(&length), sizeof(length)); // Записваме дължината на описанието
        out.write(description, length);                                     // Записваме описанието
        out.write(reinterpret_cast<const char *>(&priority), sizeof(priority));
        out.write(reinterpret_cast<const char *>(&status), sizeof(status));
        out.write(reinterpret_cast<const char *>(&duration), sizeof(duration));
        out.write(reinterpret_cast<const char *>(&startHour), sizeof(startHour));
    }

    bool readFromBinaryFile(std::ifstream &in)
    {
        if (!in)
        {
            std::cout << "Couldn't read from file." << std::endl;
            return false;
        }

        if (in.peek() == EOF)
        {
            return false;
        }

        destroy();

        int length;
        in.read(reinterpret_cast<char *>(&length), sizeof(length));
        description = new char[length + 1];
        in.read(description, length);
        description[length] = '\0';

        in.read(reinterpret_cast<char *>(&priority), sizeof(priority));
        in.read(reinterpret_cast<char *>(&status), sizeof(status));
        in.read(reinterpret_cast<char *>(&duration), sizeof(duration));
        in.read(reinterpret_cast<char *>(&startHour), sizeof(startHour));
        return true;
    }

    void printStartAndEndTime() const
    {
        int _startHour = startHour / 100;
        int startMinutes = startHour % 100;
        int e = getEndHour();
        int endHour = e / 100;
        int endMinutes = e % 100;

        std::cout << (_startHour < 10 ? "0" : "") << _startHour << ":" << (startMinutes < 10 ? "0" : "") << startMinutes
                  << " – "
                  << (endHour < 10 ? "0" : "") << endHour << ":" << (endMinutes < 10 ? "0" : "") << endMinutes;
    }
};