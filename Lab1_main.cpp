#include <iostream>
#include "Lab1.h"

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
