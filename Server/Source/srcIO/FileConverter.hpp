#ifndef FILE_CONVERTER_H_INCLUDED
#define FILE_CONVERTER_H_INCLUDED

#include <string>
#include <vector>

#include "Server/Source/srcKnnClassifier/Flower.hpp"

class FileConverter {
private:
    std::string types[4] = { "Iris-versicolor", "Iris-virginica", "Iris-setosa", "!!Undefined!!"};
    std::vector<Flower> content;


public:
    Flower& flowerFromLine(char *st);
    const char* getType(int type);
    /**
     * Returns the content of the given path in a Flower-Vector format.
     * Also stores the decoded vector in the object's "content" member.
     * @param path input file.
     * @return decoded flower-vector from input file.
     */
    std::vector<Flower>& updateFromFile(const std::string& path);

    /**
     * Update the currently stored content to the given text file path.
     * @param path Output file.
     */
    void updateToFile(const std::string& path) const;

    /**
     * @return currently stored content.
     */
    std::vector<Flower>& getContent();

    /**
     * Sets the currently stored content to the input vector.
     * @param content new content to store.
     */
    void setContent(const std::vector<Flower>& content);
};

#endif