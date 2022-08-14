#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstring>

#include "FileConverter.hpp"

std::vector<Flower> &FileConverter::getContent() {
    return this->content;
}

void FileConverter::setContent(const std::vector<Flower> &newContent) {
    // copies the given vector to current content vector.
    content.clear();
    for(const Flower& flower : newContent) {
        this->content.push_back(flower);
    }
}

std::vector<Flower>& FileConverter::updateFromFile(const std::string& path) {
    content.clear();
    std::ifstream file(path);
    std::string line;
    while (std::getline(file, line)) {
        char* info = const_cast<char *>(line.c_str());
        content.push_back(flowerFromLine(info));
    }
    return content;
}

// not very flexible as
void FileConverter::updateToFile(const std::string& path) const {

    std::ofstream file(path);
    for (auto& flower : content) {
        file << types[flower.getTypeOfIris()] << std::endl;
    }
}

//for the specific input type "num,num,num,num,type"
Flower& FileConverter::flowerFromLine(char *st) {
    char * val;
    double parameters[4];
    val = strtok(st, ",");
    for(double& param : parameters) {
        param = std::stod(val);
        val = strtok(nullptr, ",");
    }
    Flower* output = new Flower(parameters[0], parameters[1], parameters[2], parameters[3]);
    if (val != nullptr) {
        std::string temp(val);

        //converts from given type to the typeIris enum
        for (int i = 0; i < types->length() - 1; i++) {
            if (temp == types[i]) {
                output->setType((typeIris) i);
            }
        }
    }
    return *output;
}