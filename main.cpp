#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <fenv.h>
#include "window.h"
#include <stdio.h>

int main (int argc, char *argv[])
{
    //создание объекта приложения
  QApplication app (argc, argv);

    //обработка ошибок в операциях с плавающей точкой
  feenableexcept (FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW);

    //создание объекта главного окна приложения
  QMainWindow *window = new QMainWindow;
  window->setWindowTitle ("Graph");

    //создание окна с графикой
  Scene3D draw_area;
  
  //number of points menu
  QWidget *num_menu = new QWidget();
  QLabel *numb_p = new QLabel("Number of apr's points",num_menu);
  QLineEdit *delta_p= new QLineEdit(num_menu);
  delta_p->setFocusPolicy(Qt::ClickFocus);
  //QObject::connect(delta_p,SIGNAL(textChanged(const QString &)),graph_area,SLOT(change_numb_of_p(const QString &))); 
  
  //number of point layout
  QVBoxLayout *numb_layout = new QVBoxLayout;
  numb_layout->addWidget(numb_p);
  numb_layout->addWidget(delta_p);
  num_menu->setLayout(numb_layout);
  num_menu->setStyleSheet("background-color: #FFFFFF");
  
  //разметка для виджетов
  QGridLayout *rlayont = new QGridLayout;
  rlayont->setRowStretch(0,1);
  rlayont->setColumnStretch(0,1);
  rlayont->setRowMinimumHeight(1,70);
  rlayont->setColumnMinimumWidth(1,300);
  rlayont->addWidget(num_menu,1,1);
  draw_area.setLayout(rlayont);
  

    //обработка входных данных
  if (draw_area.input_values (argc, argv) != 1)
    return -1;

    //начальные размеры окна с графикой
  draw_area.resize (1000, 1000);
    //отобразить окно с графикой
  draw_area.show ();
    //запуск приложения (начало ожидания событий: нажатие кнопок, resize и т.д.)
  app.exec ();
    //удаление окна при завершении приложения
  delete window;
  return 0;
}
