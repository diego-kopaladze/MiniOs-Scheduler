#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <fstream>
#include <sstream>

struct Process {
    std::string name;
    int arrival_time;
    int burst_time;
    int priority;
    int remaining_burst_time;
    int start_time;
    int finish_time;
    int waiting_time;
    int turnaround_time;
    bool started;
};

std::vector<Process> processQueue;
int selectedAlgorithm = 1;
int timeQuantum = 4;

void mainMenu();
void addNewProcess();
void chooseSchedulingAlgorithm();
void runSimulation();
void viewProcessQueue();
void exportSimulationLogToFile(const std::string& logContent);
void helpAboutScheduling();
void exitProgram();
int getIntegerInput();
void printResults(const std::vector<Process>& finishedProcesses, const std::string& algorithmName);

int main() {
    int choice = 0;
    do {
        mainMenu();
    } while (choice != 7);
    return 0;
}

void mainMenu() {
    const int width = 55;
    int choice = 0;

    std::cout << "\n+" << std::setfill('-') << std::setw(width) << "" << "+\n";
    std::cout << "|" << std::setfill(' ') << std::left << std::setw(width - 1) << " Mini OS Scheduler - Main Menu" << "|\n";
    std::cout << "+" << std::setfill('-') << std::setw(width) << "" << "+\n";

    std::cout << std::setfill(' ');
    std::cout << "|" << std::left << std::setw(width - 1) << " 1. Add New Process" << "|" << "\n";

    std::string algoLine = std::string(" 2. Choose Scheduling Algorithm (Current: ") +
                           (selectedAlgorithm == 1 ? "FCFS" : selectedAlgorithm == 2 ? "SJF" : selectedAlgorithm == 3 ? "Round Robin" : "Priority") + ")";
    std::cout << "|" << std::left << std::setw(width - 1) << algoLine << "|" << "\n";

    std::cout << "|" << std::left << std::setw(width - 1) << " 3. Run Simulation" << "|" << "\n";
    std::cout << "|" << std::left << std::setw(width - 1) << " 4. View Process Queue" << "|" << "\n";
    std::cout << "|" << std::left << std::setw(width - 1) << " 5. Help / About Scheduling Algorithms" << "|" << "\n";
    std::cout << "|" << std::left << std::setw(width - 1) << " 6. Exit" << "|" << "\n";
    std::cout << "+" << std::setfill('-') << std::setw(width) << "" << "+\n";

    std::cout << "Enter your choice: ";
    choice = getIntegerInput();

    switch (choice) {
        case 1:
            addNewProcess();
            break;
        case 2:
            chooseSchedulingAlgorithm();
            break;
        case 3:
            runSimulation();
            break;
        case 4:
            viewProcessQueue();
            break;
        case 5:
            helpAboutScheduling();
            break;
        case 6:
            exitProgram();
            break;
        default:
            std::cout << "\nInvalid choice. Please enter a number between 1 and 6.\n";
            break;
    }
}

void addNewProcess() {
    const int width = 45;
    Process p;

    std::cout << "\n+" << std::setfill('-') << std::setw(width) << "" << "+\n";
    std::cout << "|" << std::setfill(' ') << std::setw(width - 1) << std::left << " Add New Process" << "|\n";
    std::cout << "+" << std::setfill('-') << std::setw(width) << "" << "+\n";

    std::cout << "Enter process name (without spaces): ";
    std::cin >> p.name;

    std::cout << "Enter arrival time (e.g., 0): ";
    p.arrival_time = getIntegerInput();

    std::cout << "Enter burst time (e.g., 5): ";
    p.burst_time = getIntegerInput();

    std::cout << "Enter priority (lower number = higher priority): ";
    p.priority = getIntegerInput();

    p.remaining_burst_time = p.burst_time;
    p.start_time = -1;
    p.finish_time = -1;
    p.waiting_time = 0;
    p.turnaround_time = 0;
    p.started = false;

    processQueue.push_back(p);

    std::cout << "\nProcess '" << p.name << "' added successfully.\n";
}

void chooseSchedulingAlgorithm() {
    const int width = 45;
    int choice_local = 0;

    std::cout << "\n+" << std::setfill('-') << std::setw(width) << "" << "+\n";
    std::cout << "|" << std::setfill(' ') << std::setw(width - 1) << std::left << " Select Scheduling Algorithm" << "|\n";
    std::cout << "+" << std::setfill('-') << std::setw(width) << "" << "+\n";
    std::cout << std::setfill(' ');
    std::cout << "|" << std::left << std::setw(width - 1) << " 1. First Come First Serve (FCFS)" << "|\n";
    std::cout << "|" << std::left << std::setw(width - 1) << " 2. Shortest Job First (SJF, Non-Preemptive)" << "|\n";
    std::cout << "|" << std::left << std::setw(width - 1) << " 3. Round Robin" << "|\n";
    std::cout << "|" << std::left << std::setw(width - 1) << " 4. Priority (Non-Preemptive)" << "|\n";
    std::cout << "+" << std::setfill('-') << std::setw(width) << "" << "+\n";

    std::cout << "Enter choice (1-4): ";
    choice_local = getIntegerInput();

    if (choice_local >= 1 && choice_local <= 4) {
        selectedAlgorithm = choice_local;
        std::cout << "Algorithm set successfully.\n";
        if (selectedAlgorithm == 3) {
            std::cout << "Enter time quantum for Round Robin: ";
            timeQuantum = getIntegerInput();
        }
    } else {
        std::cout << "Invalid choice. Please enter a number between 1 and 4.\n";
    }
}

void viewProcessQueue() {
    std::cout << std::setfill(' ');
    std::cout << "\n--- Current Process Queue ---\n";
    if (processQueue.empty()) {
        std::cout << "Process queue is empty.\n";
        return;
    }

    const int nameW = 15, arrivalW = 10, burstW = 8, priorityW = 10;
    std::cout << std::left
              << std::setw(nameW) << "Name"
              << std::setw(arrivalW) << "Arrival"
              << std::setw(burstW) << "Burst"
              << std::setw(priorityW) << "Priority" << "\n";
    std::cout << std::string(nameW + arrivalW + burstW + priorityW, '-') << "\n";

    for (const auto& p : processQueue) {
        std::cout << std::left
                  << std::setw(nameW) << p.name
                  << std::setw(arrivalW) << p.arrival_time
                  << std::setw(burstW) << p.burst_time
                  << std::setw(priorityW) << p.priority << "\n";
    }
}

void helpAboutScheduling() {
    std::cout << "\n--- Help: Scheduling Algorithms ---\n\n";
    std::cout << "1. First Come First Serve (FCFS):\n";
    std::cout << "   Processes are executed in the order they arrive. It's simple but can lead to long waiting times for short processes.\n\n";
    std::cout << "2. Shortest Job First (SJF, Non-Preemptive):\n";
    std::cout << "   When the CPU is free, it's assigned to the process with the shortest burst time among all available processes. Once a process starts, it runs to completion.\n\n";
    std::cout << "3. Round Robin:\n";
    std::cout << "   Each process gets a small unit of CPU time (time quantum). If a process doesn't finish in its quantum, it's moved to the back of the ready queue. This provides better responsiveness.\n\n";
    std::cout << "4. Priority (Non-Preemptive):\n";
    std::cout << "   The CPU is assigned to the process with the highest priority (represented by the lowest priority number). Once a process starts, it runs to completion.\n\n";
}

void exitProgram() {
    std::cout << "Exiting program. Goodbye!\n";
    exit(0);
}

void runSimulation() {
    if (processQueue.empty()) {
        std::cout << "\nProcess queue is empty. Add processes before running a simulation.\n";
        return;
    }

    std::vector<Process> simProcesses = processQueue;
    for (auto& p : simProcesses) {
        p.remaining_burst_time = p.burst_time;
        p.start_time = -1;
        p.finish_time = -1;
        p.waiting_time = 0;
        p.turnaround_time = 0;
        p.started = false;
    }

    int currentTime = 0;
    std::vector<Process> finishedProcesses;
    std::string algorithmName;

    switch (selectedAlgorithm) {
        case 1:
            {
                algorithmName = "First Come First Serve (FCFS)";
                std::sort(simProcesses.begin(), simProcesses.end(), [](const Process& a, const Process& b) {
                    return a.arrival_time < b.arrival_time;
                });

                for (auto& p : simProcesses) {
                    currentTime = std::max(currentTime, p.arrival_time);
                    p.start_time = currentTime;
                    p.finish_time = currentTime + p.burst_time;
                    currentTime = p.finish_time;
                    finishedProcesses.push_back(p);
                }
            }
            break;

        case 2:
            {
                algorithmName = "Shortest Job First (SJF)";
                int completed = 0;
                while(completed < simProcesses.size()){
                    int shortestJobIndex = -1;
                    int minBurst = std::numeric_limits<int>::max();

                    for(int i = 0; i < simProcesses.size(); ++i){
                        if(simProcesses[i].arrival_time <= currentTime && simProcesses[i].remaining_burst_time > 0){
                            if(simProcesses[i].burst_time < minBurst){
                                minBurst = simProcesses[i].burst_time;
                                shortestJobIndex = i;
                            }
                        }
                    }

                    if(shortestJobIndex != -1){
                        Process& p = simProcesses[shortestJobIndex];
                        p.start_time = currentTime;
                        currentTime += p.burst_time;
                        p.finish_time = currentTime;
                        p.remaining_burst_time = 0;
                        finishedProcesses.push_back(p);
                        completed++;
                    } else {
                        currentTime++;
                    }
                }
            }
            break;

        case 3:
            {
                algorithmName = "Round Robin (Quantum: " + std::to_string(timeQuantum) + ")";
                std::vector<int> readyQueue;
                std::vector<bool> inQueue(simProcesses.size(), false);
                int completed = 0;
                int lastArrivalCheck = -1;

                while (completed < simProcesses.size()) {
                    for (int i = 0; i < simProcesses.size(); ++i) {
                        if (simProcesses[i].arrival_time <= currentTime && !inQueue[i] && simProcesses[i].remaining_burst_time > 0) {
                            readyQueue.push_back(i);
                            inQueue[i] = true;
                        }
                    }

                    if (!readyQueue.empty()) {
                        int processIndex = readyQueue.front();
                        readyQueue.erase(readyQueue.begin());

                        Process& p = simProcesses[processIndex];
                        if (!p.started) {
                            p.start_time = currentTime;
                            p.started = true;
                        }

                        int timeSlice = std::min(timeQuantum, p.remaining_burst_time);
                        currentTime += timeSlice;
                        p.remaining_burst_time -= timeSlice;

                        for (int i = 0; i < simProcesses.size(); ++i) {
                            if (simProcesses[i].arrival_time <= currentTime && !inQueue[i] && simProcesses[i].remaining_burst_time > 0) {
                                readyQueue.push_back(i);
                                inQueue[i] = true;
                            }
                        }

                        if (p.remaining_burst_time > 0) {
                            readyQueue.push_back(processIndex);
                        } else {
                            p.finish_time = currentTime;
                            finishedProcesses.push_back(p);
                            completed++;
                            inQueue[processIndex] = false;
                        }
                    } else {
                        currentTime++;
                    }
                }
            }
            break;

        case 4:
            {
                algorithmName = "Priority (Non-Preemptive)";
                int completed = 0;
                while (completed < simProcesses.size()) {
                    int highestPriorityIndex = -1;
                    int minPriority = std::numeric_limits<int>::max();

                    for (int i = 0; i < simProcesses.size(); ++i) {
                        if (simProcesses[i].arrival_time <= currentTime && simProcesses[i].remaining_burst_time > 0) {
                            if (simProcesses[i].priority < minPriority) {
                                minPriority = simProcesses[i].priority;
                                highestPriorityIndex = i;
                            }
                        }
                    }

                    if (highestPriorityIndex != -1) {
                        Process& p = simProcesses[highestPriorityIndex];
                        p.start_time = currentTime;
                        currentTime += p.burst_time;
                        p.finish_time = currentTime;
                        p.remaining_burst_time = 0;
                        finishedProcesses.push_back(p);
                        completed++;
                    } else {
                        currentTime++;
                    }
                }
            }
            break;
    }

    printResults(finishedProcesses, algorithmName);
}

void printResults(const std::vector<Process>& finishedProcesses, const std::string& algorithmName) {
    std::vector<Process> finalResults = finishedProcesses;

    std::sort(finalResults.begin(), finalResults.end(), [](const Process& a, const Process& b){
        return a.name < b.name;
    });

    double totalWaitingTime = 0;
    double totalTurnaroundTime = 0;

    for (auto& p : finalResults) {
        p.turnaround_time = p.finish_time - p.arrival_time;
        p.waiting_time = p.turnaround_time - p.burst_time;
        totalWaitingTime += p.waiting_time;
        totalTurnaroundTime += p.turnaround_time;
    }

    std::stringstream ss;

    ss << "\n--- Simulation Results: " << algorithmName << " ---\n";
    const int nameW = 12, arrivalW = 8, burstW = 7, startW = 7, finishW = 8, waitW = 8, turnW = 11;
    ss << std::left << std::setw(nameW) << "Process" << std::setw(arrivalW) << "Arrival"
       << std::setw(burstW) << "Burst" << std::setw(startW) << "Start"
       << std::setw(finishW) << "Finish" << std::setw(waitW) << "Waiting"
       << std::setw(turnW) << "Turnaround" << "\n";
    ss << std::string(nameW + arrivalW + burstW + startW + finishW + waitW + turnW, '-') << "\n";

    for (const auto& p : finalResults) {
        ss << std::left << std::setw(nameW) << p.name << std::setw(arrivalW) << p.arrival_time
           << std::setw(burstW) << p.burst_time << std::setw(startW) << p.start_time
           << std::setw(finishW) << p.finish_time << std::setw(waitW) << p.waiting_time
           << std::setw(turnW) << p.turnaround_time << "\n";
    }

    if (!finalResults.empty()) {
        ss << "\nAverage Waiting Time: " << std::fixed << std::setprecision(2) << totalWaitingTime / finalResults.size() << "\n";
        ss << "Average Turnaround Time: " << std::fixed << std::setprecision(2) << totalTurnaroundTime / finalResults.size() << "\n";
    }

    std::cout << ss.str();

    std::cout << "\nDo you want to export these results to a file? (y/n): ";
    char choice;
    std::cin >> choice;
    if (choice == 'y' || choice == 'Y') {
        exportSimulationLogToFile(ss.str());
    }
}

void exportSimulationLogToFile(const std::string& logContent) {
    std::string filename = "../simulation_log.txt";
    std::ofstream outfile(filename);

    if (!outfile) {
        std::cerr << "Error: Failed to open file " << filename << " for writing.\n";
        return;
    }

    outfile << "--- Mini OS Scheduler Log ---\n";
    outfile << "Initial Process Queue:\n";
    const int nameW = 15, arrivalW = 10, burstW = 8, priorityW = 10;
    outfile << std::left << std::setw(nameW) << "Name" << std::setw(arrivalW) << "Arrival"
            << std::setw(burstW) << "Burst" << std::setw(priorityW) << "Priority" << "\n";
    outfile << std::string(nameW + arrivalW + burstW + priorityW, '-') << "\n";
    for (const auto& p : processQueue) {
        outfile << std::left << std::setw(nameW) << p.name << std::setw(arrivalW) << p.arrival_time
                << std::setw(burstW) << p.burst_time << std::setw(priorityW) << p.priority << "\n";
    }

    outfile << "\n" << logContent;

    outfile.close();
    std::cout << "Simulation log exported successfully to " << filename << "\n";
}

int getIntegerInput() {
    int value;
    while (!(std::cin >> value)) {
        std::cout << "Invalid input. Please enter a whole number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return value;
}
