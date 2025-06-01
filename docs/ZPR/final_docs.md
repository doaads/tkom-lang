# ZPR/TKOM - Interpreted Programming language implementation in C++

## 1. General Details

The project's goal was to implement a programming language that has:
* weak and static typing
* variables that are constant by default
* function arguments passed via reference
* high-order functions implemented into it

The implemented language's syntax and more details on the grammar can be found [here](https://doaads.github.io/tkom-lang/md_docs_2TKOM_2dokumentacja-koncowa-tkom.html)

The following tools have been used during implementation:
* **C++23**
* **clang-format** - for file formatting (configuration is available at `.clang-format`)
* **clang-tidy** - for linting (configuration available at `.clang-tidy`)
* **cmake** - for building/linking the project and tests
* **doxygen** - for generating the documentation
* **Google Test** - for testing
* **just** - for easy building and testing

***

## 2. Building the project

Ensure you have the following installed:
* C++ `Boost` libraries
* `just`
* `cmake`

Then, navigate to the repository's root directory, and `just build`.

The build files will be available at `build/`

## 3. Running the project

The compiled binary `./tkom` can be run from the `build/` directory via:

```sh
./tkom [filename] [-V, --verbose]
```

When toggled, the `-V` flag will enable verbose logging, and will print out the parsed syntax tree on the screen:

![](img/2025-05-31-20-25-35.png)

## 4. Example source files

The example source files are available at `examples/`. They show the language's general syntax and demonstrate mechanisms such as decorators and bind fronts.

## 5. Testing

The project has been tested using `Google Test`. The tests can be run via `just test`.


