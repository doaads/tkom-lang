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

## 4. Laguange quickstart

### 4.1 Example source files

The example source files are available at `examples/`. They show the language's general syntax and demonstrate mechanisms such as decorators and bind fronts.

### 4.2 Language structure

A source file must consist of at least one function definition. A source file must contain a definition for a `main` function with the following signature:

```[int::]```

An example `main` function will look like this:

```cpp
int main {  // skip `::` operator here, as we have no parameters
...    // user code
ret 0; // return 0
}
```

### 4.3 Builtin functions

Aside from user functions, a couple of builtin functions are available:
- `increment` - `[void::mut int]` - increment by reference
- `increment_v` - `[int::int]` - increment by value
- `sqrt` - `[void::mut flt]` - square root by reference
- `sqrt` - `[flt::flt]` - square root by value
- `stdout` - `[void::string]` - print
- `stdin` - `[string::string]` - get input from user

Full language description is available [here](https://doaads.github.io/tkom-lang/md_docs_2TKOM_2dokumentacja-koncowa-tkom.html)

## 5. Testing

The project has been tested using `Google Test`. The tests can be run via `just test` (or `ctest` in `build/`)

The tests include:
- Parametrized tests for the lexical analyzer consisting of every token
- Parametrized tests for the parser consisting of every construction
- More detailed parser tests parsing full programs
- Parametrized interpreter tests for many expression combinations and their results
- Parametrized interpreter tests for exceptions
- More detailed interpreter tests for every possible statement
- More detailed interpreter tests for full programs
- More detailed interpreter tests for high-order functions

## 6. Platform support

The product is being continually tested and compiled on the following platforms:

* **Ubuntu Latest** with **GCC**
* **Ubuntu Latest** with **Clang**
* **Windows x64** with **Visual Studio**

More details can be seen [here](https://github.com/doaads/tkom-lang/actions/workflows/cmake-multi-platform.yml)

## 7. Building the documentation locally

If needed, the documentation can be built locally using

```
just docs
```

The results can be viewed at `docs/html/index.html`

