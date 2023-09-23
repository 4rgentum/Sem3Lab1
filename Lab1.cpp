#include <iostream>
#include "Lab1.h"

namespace Lab1 {
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
      throw;
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
