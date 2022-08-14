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
You can run the code with our default input files, or alternatively put in your own, in the <code>input</code> folder, and the output will be found in a similar <code>output</code> folder.

```
KNNClassifier
│   ...
│
└───input
│   │   classified.csv
│   │   unclassified.csv
│   │   ...
│
└───output
│   │   chebyshev_output.csv
│   │   euclidean_output.csv
│   │   manhattan_output.csv
│   │   ...
```

<p>You can run the project using our provided <code>CMakeLists.txt</code> file:</p>

```console
$ mkdir -p build && cd build
$ cmake
$ make -j && make KNNClassifier {K}
```
<p>Replacing {k} with an integer of your choosing.</p>

----

## Implementation
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

The output given by the classifier class is written to an output folder, in a .csv file with an appropriate name thanks to our FileConverter class, which manages this project's input-output. Currently, the file converting system is hyper-specific and not very flexible, but this should be a problem since the UI is bound to change drastically, so the current system is only temporary.

```c++
class FileConverter {
public:
    std::vector<Flower>& updateFromFile(std::string path);
    void updateToFile(std::string path);
    ...
};
```

----
## Authors

* [Ohad Heines](https://github.com/bananabajanana)
* [Michal Iakobashvili](https://github.com/michaliakobashvili)

----
## Acknowledgements

For a better understanding of the algorithm we researched in [Wikipedia](https://www.wikipedia.org/), and used [Stack Overflow](https://stackoverflow.com/) to understand c++ syntax and debug more efficiently.

[1]: https://en.wikipedia.org/wiki/K-nearest_neighbors_algorithm