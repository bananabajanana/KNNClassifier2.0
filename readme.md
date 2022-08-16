# KNN classifier
<em>"In statistics, the k-nearest neighbors algorithm (k-NN) is a non-parametric supervised learning method [...] k-NN is a type of classification where the function is only approximated locally and all computation is deferred until function evaluation"</em> [1]
<p>This project is an implementation of the kNN algorithm, to estimate the type of Iris flower based on four properties:</p>
<ul>
<li>Sepal length</li>
<li>Sepal width</li>
<li>Petal length</li>
<li>Petal width</li>
</ul>

----
#### This project is written in <b>c++</b>, by <b>Ohad Heines</b> and <b>Michal Iakobashvili</b>

----

## Installation

This project relies on CMake to generate the executable file. If you don't have CMake yet, you can download it [here](https://cmake.org/download/), or alternatively, in a Debian-based system, run

```console
$ sudo apt install cmake
```

<p>Then, once cmake is installed, clone this project's repository with:</p>

```console
$ git clone 'https://github.com/bananabajanana/KNNClassifier.git'
```

----

## Usage
You can run the code with our default input files, or alternatively put in your own.
The server has it's training data in <code>TrainingData.csv</code>.
In addition the client's input and output files can be given as arguments during compilation time, but it is recommended to put them as <code>Unclassified.csv</code> and <code>Classified.csv</code> folders as shown below.

```
KNNClassifier
│   ...
│
└───Client
│   │   ...
│   │   
│   └───Data
│   │   │   Unclassified.csv
│   │       
│   └───Output
│       │   Classified.csv
│
└───Server
│   │   ...
│   │   
│   └───Data
│   │   │   TrainingData.csv
```

<p>You can run the project using our provided <code>CMakeLists.txt</code> file:</p>

```console
$ mkdir -p build && cd build
$ cmake ..
$ make -j && make Server && make Client {IPath} {OPath}
$ ./Server
```
Where {IPath} should be replaced with the path to the input file to be classified, and {OPath} with the path to output the data to.
It is also possible to keep not specify {IPath} and {OPath} arguments, which will cause the program to work with the default arguments specified above.

After this, the compilation and linkage of the codes is complete and the Server is running.
All that is left is to run the Client program from a different terminal, as following:

```console
$ cd {path}/build
$ ./Client
```
Replacing {path} with the folder path to the project's repository on your personal computer.

----

## Implementation

### Server
We first created a Flower object, that will store a single flower's type and parameters. The flower's type is stored as an enum, with four options: the three possible types, and an undefined option.

```c++
enum typeIris { versicolor, virginica, setosa, undifined };

class Flower {
private:
    typeIris type;
    const NPoint character;
    ...
};
```
The NPoint is a representation of the Flower's parameters, and functions as a point in an n-dimensional coordinate system (in 4d with the current implementation example, but can easily be expanded).

Our code implements the algorithm with three different possible distance functions: Euclidean distance, Manhattan distance, and Chebyshev distance, but to allow the addition of other distance functions, we implemented generic code with an abstract distance class.

```c++
class DistanceCalc {
public:
    virtual double distance(NPoint p1, NPoint p2) = 0;
    ...
};
```

In addition, to keep track of all our different types of distances implemented, we made a DistancesData builder class.

```c++
class DistancesData {
    /**
     * @return all the types of distance calculators that can be used.
     */
    static std::vector<DistanceCalc*>& getAllTypes();
};
```

Finally, we created a Classifier class. This class identifies a given vector of flowers based on an input list of already identified Irises. This is implemented step-by-step according to the kNN algorithm, first finding the k closest neighbors to the unidentifiable flower, and then finding which category is most common among them.

The with these implemented, the server runs indefinitely in the following loop:

```mermaid
graph TD;
    Listen-->DataFromUser;
    DataFromUser-->OutputClass;
    OutputClass-->DataFromUser;
    OutputClass-->UserDisconnects;
    UserDisconnects-->Listen
```

* <code>Listen</code>: The server waits for a user to connect.
* <code>DataFromUser</code>: The server receives information about an unclassified flower from the user.
* <code>OutputClass</code>: The server sends to the user the classification of the flower.
* <code>UserDisconnects</code>: The current user disconnects, allowing the server to interact with a new user.

### Client

The client is a sort of *simple demo* for interaction with the server. It takes an input file from the user, and sends the flower information - one by one - to the server, while outputting the given classifications to an output file.

```mermaid
graph TD;
    ReadLine-.No lines left.->Exit;
    ReadLine-->SendInfo;
    SendInfo-->WriteLine;
    WriteLine-->ReadLine;
    
```

* <code>ReadLine</code>: The client program reads a line from the input file.
* <code>SendInfo</code>: The client sends the flower info previously read to the server.
* <code>WriteLine</code>: The client writes the given class to the output file.
* <code>Exit</code>: If there are no lines left, the client closes its connection with the server.

----
## Authors

* [Ohad Heines](https://github.com/bananabajanana)
* [Michal Iakobashvili](https://github.com/michaliakobashvili)

----
## Acknowledgements

For a better understanding of the algorithm we researched in [Wikipedia](https://www.wikipedia.org/), and used [Stack Overflow](https://stackoverflow.com/) to understand c++ syntax and debug more efficiently.

[1]: https://en.wikipedia.org/wiki/K-nearest_neighbors_algorithm