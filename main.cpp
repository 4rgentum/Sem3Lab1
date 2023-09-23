#include <iostream>
#include <string>
#include <limits>
#include <cstring>

/*Из входного потока вводится прямоугольная сильно разреженная матрица целых чисел 
[aij], i = 1, …, m, j = 1, …, n. Значения m и n заранее не известны 
и вводятся из входного потока.
Сформировать новую матрицу, поместив в ее i-ую строку элементы из i-ой строки исходной матрицы,
начиная с минимального элемента и до конца строки.
Исходную и полученную матрицы вывести в выходной поток с необходимыми комментариями*/


namespace Lab1 {
  // Структура координаты элемента
  struct Coordinates {
    long long line = -1;
    long long column = -1;
  };
  // Структура самого элемнта матрицы
  struct Elem {
    int data = -1;
    Coordinates position;
    Elem *next = nullptr;
  };
  // Хранение самой матрицы в виде списка в котором известны голова и хвост
  struct Matrix {
    Coordinates size;
    Elem *head = nullptr;
    Elem *tail = nullptr;
  };

  // шаблонная функция ввода одного числа
  template<class T>
  T getNum(T min = std::numeric_limits<T>::min(), T max = std::numeric_limits<T>::max()) {
    T a;
    while(true) {
      std::cin >> a;
      if(std::cin.eof()) { // обнаружен конец файла
        throw std::runtime_error("Failed to read number: EOF");
      }
      else if(std::cin.bad()) { // обнаружена невосстановимая ошибка входного потока
        throw std::runtime_error(std::string("Failed to read number: ") + strerror(errno));
      }
      else if(std::cin.fail()) { // прочие ошибки (неправильный формат ввода)
        std::cin.clear(); // очищаем флаги состояния потока
        // игнорируем все символы до конца строки
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "You are wrong; repeat please!" << std::endl;
      }
      else if(a >= min && a <= max) { // если число входит в заданный диапазон
        return a;
      }
      else {
        std::cerr << "Number out of range. Please enter a valid number." << std::endl;
      }
    }
  }

  // Прототипы функций
  void addElementToMatrix(Matrix &matrix, int data, const Coordinates &position); // Функция для добавления элемента в матрицу
  void initializeMatrix(Matrix &matrix); // Функция для инициализации и заполнения разреженной матрицы
  void clearMatrix(Matrix &matrix); // Функция для освобождения памяти, выделенной под элементы матрицы
  void printMatrix(const Matrix &matrix); // Функция для вывода матрицы
  void printListContents(const Matrix &matrix); // Вывод содержимого списка
  Matrix createEmptyMatrix(const Coordinates &size); // Функция для создания новой пустой матрицы
  Elem *findMinElementInRow(long long row_index, Elem *start_elem); // Функция для нахождения минимального элемента в своей строке
  Matrix createNewMatrix(const Matrix& sourceMatrix); // Функция создания новой матрицы где строки содержат тольок числа начиная с минимального и до конца стркои
  
  // Функция для добавления элемента в матрицу
  void addElementToMatrix(Matrix &matrix, int data, const Coordinates &position) {
    if (data != 0) {
      // Создаем новый элемент матрицы
      Elem *newElem = new Elem;
      newElem->data = data;
      newElem->position = position;
      newElem->next = nullptr;

      // Если матрица пуста, устанавливаем как головной и хвостовой элемент
      if (matrix.head == nullptr) {
        matrix.head = newElem;
        matrix.tail = newElem;
      } else {
        // Добавляем элемент в конец списка и обновляем хвостовой элемент
        matrix.tail->next = newElem;
        matrix.tail = newElem;
      }
    }
  }

  // Функция для инициализации и заполнения разреженной матрицы
  void initializeMatrix(Matrix &matrix) {
    try {
      do {
        std::cout << "Enter number of lines (m): ";
        matrix.size.line = getNum<long long>(1); // Минимальное значение - 1
        if (matrix.size.line < 1) {
          std::cerr << "The string size must be greater than or equal to 1." << std::endl;
        }
      } while (matrix.size.line < 1);

      do {
        std::cout << "Enter the number of columns (n): ";
        matrix.size.column = getNum<long long>(1); // Минимальное значение - 1
        if (matrix.size.column < 1) {
          std::cerr << "Column size must be greater than or equal to 1." << std::endl;
        }
      } while (matrix.size.column < 1);

      for (long long i = 0; i < matrix.size.line; ++i) {
        std::cout << "Enter elements for row " << i + 1 << ":" << std::endl;
        for (long long j = 0; j < matrix.size.column; ++j) {
          int data;
          std::cout << "Element in position [" << i << "][" << j << "]: ";
          data = getNum<int>();
          addElementToMatrix(matrix, data, Coordinates{i, j});
        }
      }
    } catch (const std::exception &e) {
      std::cerr << "A matrix input error occurred: " << e.what() << std::endl;
    }
  }

  // Функция для освобождения памяти, выделенной под элементы матрицы
  void clearMatrix(Matrix &matrix) {
    Elem *current = matrix.head;
    while (current != nullptr) {
      Elem *next = current->next;
      delete current;
      current = next;
    }
    matrix.head = nullptr; // Установка указателя на начало матрицы в nullptr
  }

  // Функция для вывода матрицы
  void printMatrix(const Matrix &matrix) {
    std::cout << "Matrix (" << matrix.size.line << "x" << matrix.size.column << "):" << std::endl;
    for (long long i = 0; i < matrix.size.line; ++i) {
      for (long long j = 0; j < matrix.size.column; ++j) {
        Elem *current = matrix.head;
        bool found = false;
        while (current != nullptr) {
          if (current->position.line == i && current->position.column == j) {
            std::cout << current->data << ' ';
            found = true;
            break;
          }
          current = current->next;
        }
        if (!found) {
          std::cout << "0 "; // Если элемент не найден, выводим 0
        }
      }
      std::cout << std::endl;
    }
  }

  // Вывод содержимого списка
  void printListContents(const Matrix &matrix) {
    Elem *current = matrix.head;

    while (current != nullptr) {
      std::cout << "Element at [" << current->position.line << "][" << current->position.column << "]: " << current->data << std::endl;
      current = current->next;
    }
  }

  // Функция для создания новой пустой матрицы
  Matrix createEmptyMatrix(const Coordinates &size) {
    Matrix matrix;
    matrix.size = size;
    matrix.head = nullptr; // Инициализируем пустой указатель на голову
    matrix.tail = nullptr;
    return matrix;
  }

  // Функция для нахождения минимального элемента в своей строке
  Elem *findMinElementInRow(long long row_index, Elem *start_elem) {
    Elem *current = start_elem;
    Elem *min_elem = nullptr;
    int minData = std::numeric_limits<int>::max();

    while (current) {
      if (current->position.line > row_index) { // Переход на другуб строку -> закончить пробежку
        break;
      }
      if (current->position.line == row_index && current->data < minData) { // Проверка условия минимальности значения и получение его координат
        minData = current->data;
        min_elem = current;
      }
      current = current->next;
    }

    return min_elem;
  }

  // Функция создания новой матрицы где строки содержат тольок числа начиная с минимального и до конца стркои
  Matrix createNewMatrix(const Matrix& sourceMatrix) {
    Coordinates newSize = sourceMatrix.size;
    Matrix newMatrix = createEmptyMatrix(newSize);

    Elem* currentRowElement = sourceMatrix.head;

    for (long long i = 0; i < newSize.line; ++i) {
      Elem* minElement = findMinElementInRow(i, currentRowElement); // Находим место где содержится минимальный элемент

      if (!minElement) {
        // Если в строке нет минимальных элементов, переходим к следующей строке
        currentRowElement = nullptr;
        continue;
      }

      Elem* current = minElement;
      Coordinates currentPosition{0, 0};

      while (current) {
        if (current->position.line > i) { // Если перешли на дуругую строку матрицы закончим заполненение текущей строки
          break;
        }
        currentPosition.line = current->position.line;
        currentPosition.column = current->position.column;
        addElementToMatrix(newMatrix, current->data, currentPosition);
        current = current->next;
      }
      currentRowElement = current; // Устанавливаем новое значение строки
    }

    return newMatrix;
  }
}

using namespace Lab1;

int main() {
  try {
    while (true) {
      // Инициализация и ввод матрицы
      std::cout << "Enter the matrix:" << std::endl;
      Matrix inputMatrix;
      initializeMatrix(inputMatrix);
  
      // Вывод исходной матрицы
      std::cout << "Input matrix:" << std::endl;
      printListContents(inputMatrix);
      printMatrix(inputMatrix);
  
      // Создание новой матрицы с минимальными элементами
      Matrix resultMatrix = createNewMatrix(inputMatrix);
  
      // Вывод результирующей матрицы
      std::cout << "Result matrix:" << std::endl;
      printListContents(resultMatrix);
      printMatrix(resultMatrix);
  
      // Очистка памяти
      clearMatrix(inputMatrix);
      clearMatrix(resultMatrix);

      // Проверка на конец файла (EOF)
      if (std::cin.eof()) {
        std::cout << "EOF reached. Exiting..." << std::endl;
        break;
      }
      
      // Спросить пользователя, хочет ли он продолжить
      std::string response;
      do {
        std::cout << "Continue? (y/n): ";
        std::cin >> response;
        if (response != "y" && response != "Y" && response != "n" && response != "N") {
          std::cerr << "Invalid response. Please enter 'y' or 'n'." << std::endl;
        }
      } while (response != "y" && response != "Y" && response != "n" && response != "N");
  
      if (response != "y" && response != "Y") {
        break; // Выход из цикла, если пользователь не хочет продолжать
      }
    }
  } catch (const std::exception &ex) {
    std::cerr << "An error occurred: " << ex.what() << std::endl;
    return 1;
  }
}


/*
# установка версии CMake
cmake_minimum_required(VERSION 3.16)

# название проекта (обязательно)
project(oopprog1)

# установка стандарта языка - C++20
set(CMAKE_CXX_STANDARD 20)

# установка флагов компилятора: CMAKE_CXX_FLAGS += -Wall -Wextra
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# создание библиотеки prog1
add_library(prog1 Prog1.h Prog1.cpp)

# подключение библиотеки prog1 ко всем таргетам, создаваемым далее
# альтернатива: target_link_libraries(main prog1)
link_libraries(prog1)

# создание исполняемого файла
add_executable(main Prog1main.cpp)

# для сборки из консоли:
#
# mkdir build   # создание директории для файлов сборки
# cd build      # переход в директорию сборки
# cmake ..      # генерация файлов сборки на основе CMakeLists.txt
# make          # сборка проекта
*/