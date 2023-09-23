#ifndef LAB1_H
#define LAB1_H

#include <iostream>
#include <string>
#include <limits>
#include <cstring>

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
}

#endif //LAB1_H
