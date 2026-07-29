# stdVector

Учебная реализация контейнера `MyVector` на C++17 с тестами GoogleTest и
benchmark-сравнением со `std::vector`.

## Требования

- CMake версии 3.14 или новее;
- компилятор с поддержкой C++17;
- GoogleTest — нужен для сборки и запуска тестов.

На macOS GoogleTest можно установить через Homebrew:

```bash
brew install googletest
```

После установки перегенерируйте каталог `build`, чтобы CMake и IntelliSense
увидели заголовки GoogleTest.

## Сборка

Из корня проекта выполните:

```bash
cmake -S . -B build
cmake --build build
```

Если GoogleTest не установлен, CMake всё равно соберёт benchmark, но цель
`vector_test` и тесты будут пропущены.

## Запуск тестов

```bash
ctest --test-dir build --output-on-failure
```

При необходимости тесты можно запустить напрямую:

```bash
./build/vector_test
```

## Запуск benchmark

```bash
./build/vector_benchmark
```

Benchmark сравнивает `MyVector<int>` и `std::vector<int>` для операций
`push_back`, произвольного доступа и вставки в середину.
