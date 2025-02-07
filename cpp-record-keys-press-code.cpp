#include <iostream>
#include <windows.h>
#include <unordered_map>
#include <chrono>
#include <unordered_set>
#include <fstream>
#include <sstream>
#include <vector>

std::string param1;
std::string param2;

struct Coordinates{
    int x;
    int y;
};

void MoveMouse(const Coordinates& coords){
    SetCursorPos(coords.x, coords.y);
}

void WriteFile(std::unordered_map<std::string, Coordinates>& mapToWrite){
    std::ofstream outputFile("Output.txt");
    if (outputFile.is_open()) {
        for (const auto& pair : mapToWrite) {
            outputFile << pair.first << " : " << pair.second.x << " : " << pair.second.y << std::endl;
        }

        outputFile.close();
    } else {
        std::cerr << "Unable to open file." << std::endl;
    }
}

int main() {
    std::unordered_map<int, std::string> keyMap = {
        {8, "Backspace"}, {9, "Tab"}, {13, "Enter"}, {16, "Shift"}, {17, "Ctrl"}, {18, "Alt"},
        {19, "Pause"}, {20, "Caps Lock"}, {27, "Esc"}, {32, "Space"}, {33, "Page Up"}, {34, "Page Down"},
        {35, "End"}, {36, "Home"}, {37, "Left Arrow"}, {38, "Up Arrow"}, {39, "Right Arrow"}, {40, "Down Arrow"},
        {45, "Insert"}, {46, "Delete"}, {48, "0"}, {49, "1"}, {50, "2"}, {51, "3"}, {52, "4"}, {53, "5"}, {54, "6"}, {55, "7"}, {56, "8"}, {57, "9"},
        {65, "A"}, {66, "B"}, {67, "C"}, {68, "D"}, {69, "E"}, {70, "F"}, {71, "G"}, {72, "H"}, {73, "I"}, {74, "J"}, {75, "K"}, {76, "L"}, {77, "M"}, {78, "N"}, {79, "O"}, {80, "P"}, {81, "Q"}, {82, "R"}, {83, "S"}, {84, "T"}, {85, "U"}, {86, "V"}, {87, "W"}, {88, "X"}, {89, "Y"}, {90, "Z"},
        {91, "Left Windows"}, {92, "Right Windows"}, {93, "Menu"}, {96, "Num 0"}, {97, "Num 1"}, {98, "Num 2"}, {99, "Num 3"}, {100, "Num 4"}, {101, "Num 5"}, {102, "Num 6"}, {103, "Num 7"}, {104, "Num 8"}, {105, "Num 9"},
        {106, "Num *"}, {107, "Num +"}, {109, "Num -"}, {110, "Num ."}, {111, "Num /"}, {144, "Num Lock"}, {145, "Scroll Lock"},
        {186, ";"}, {187, "="}, {188, ","}, {189, "-"}, {190, "."}, {191, "/"}, {192, "`"}, {219, "["}, {220, "\\"}, {221, "]"}, {222, "'"}
    };
    std::unordered_map<std::string, Coordinates> buttonMap = {
        {"Start", {250, 860}}, //Start Coordinates
        {"Worlds", {254, 767}}, //World Switch Coordinates
        {"Back", {1776, 982}},
        {"Exit", {1335, -102}},
        {"ReturnToLobby", {1517, -85}},
        {"ConfirmExit", {1517, -85}}
    };
    std::vector<std::string> words;
    std::string word;
    std::unordered_set<int> pressedKeys;
    auto lastTime = std::chrono::high_resolution_clock::now();
    bool recording = false;

    while (true) {
        // Check for the backtick (`) key press to toggle recording
        if (GetAsyncKeyState(192) & 0x8000) {
            if (pressedKeys.find(192) == pressedKeys.end()) {
                recording = !recording;
                std::cout << (recording ? "Recording started" : "Recording stopped") << std::endl;
                WriteFile(buttonMap);
                pressedKeys.insert(192);
            }
        } else {
            pressedKeys.erase(192);
        }

        if (recording) {
            for (int key = 8; key <= 190; key++) {
                if (GetAsyncKeyState(key) & 0x8000) {
                    if (pressedKeys.find(key) == pressedKeys.end()) {
                        auto currentTime = std::chrono::high_resolution_clock::now();
                        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime);
                        lastTime = currentTime;

                        if (keyMap.find(key) != keyMap.end()) {
                            std::cout << "Key pressed: " << keyMap[key] << " - Time since last key press: " << elapsed.count() << " ms" << std::endl;
                        } else {
                            std::cout << "Unknown key pressed: " << key << " - Time since last key press: " << elapsed.count() << " ms" << std::endl;
                        }
                        pressedKeys.insert(key);
                    }
                } else {
                    pressedKeys.erase(key);
                }
            }

            // Check for mouse clicks
            if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
                POINT p;
                if (GetCursorPos(&p)) {
                    std::cout << "Mouse Left Button Clicked at (" << p.x << ", " << p.y << ")" << std::endl;
                    Sleep(100); // Sleep to avoid multiple detections for a single click
                }
            }
            if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) {
                POINT p;
                if (GetCursorPos(&p)) {
                    std::cout << "Mouse Right Button Clicked at (" << p.x << ", " << p.y << ")" << std::endl;
                    Sleep(100); // Sleep to avoid multiple detections for a single click
                }
            }
            if (GetAsyncKeyState(VK_MBUTTON) & 0x8000) {
                POINT p;
                if (GetCursorPos(&p)) {
                    std::cout << "Mouse Middle Button Clicked at (" << p.x << ", " << p.y << ")" << std::endl;
                    Sleep(100); // Sleep to avoid multiple detections for a single click
                }
            }
        }

        Sleep(100);
    }
    return 0;
}
