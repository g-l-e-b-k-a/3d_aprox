#include <QtGui>
#include <fenv.h>
#include "window.h"
#include <stdio.h>


MainWindow::MainWindow() // конструктор
{ 
   draw_area = new Scene3D; // создаю динамический объект класса Scene3D
   setCentralWidget(draw_area); // обозначаю scene1 центральным виджетом в главном окне
     
   this->setWindowTitle(tr("Graph3D")); // название главного окна
   
}

void MainWindow::keyPressEvent(QKeyEvent* pe)
{
   draw_area->keyPressEvent(pe);
}

int main (int argc, char *argv[])
{
    //создание объекта приложения
  QApplication app (argc, argv);

    //обработка ошибок в операциях с плавающей точкой
 // feenableexcept (FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW);

    //создание объекта главного окна приложения
  MainWindow *window = new MainWindow;
  QMenuBar *tool_bar = new QMenuBar (window);
   //QPushButton *al1 = new QPushButton("x2",window);
   //QPushButton *al2 = new QPushButton("/2",window);
   //создание окна с графикой
  //Scene3D draw_area;
  setlocale(LC_ALL, "POSIX");
  tool_bar->addAction ("Function/Appr_func/Error Func", window->draw_area, SLOT (change_func ()));
 
  QMenu *ch_f = new QMenu("Functions",tool_bar);
  tool_bar->addMenu(ch_f);
  ch_f->addAction("z=exp(x*x-y*y)",window->draw_area,SLOT (func1 ()));
  ch_f->addAction("z=1",window->draw_area,SLOT (func2 ()));
  ch_f->addAction("z=x+y",window->draw_area,SLOT (func3 ()));
  ch_f->addAction("z=cos(x)*sin(y)",window->draw_area,SLOT (func4 ()));
  ch_f->addAction("z=x*y",window->draw_area,SLOT (func5 ()));


  tool_bar->setMaximumHeight (30);
  tool_bar->setNativeMenuBar (false);
  
    //обработка входных данных
  if (window->draw_area->input_values (argc, argv) != 1)
    return -1;
  char buf[10];
  sprintf(buf, "x%d", window->draw_area->alpha);
  sprintf(buf+5, "/%d", window->draw_area->alpha);
  //number of points menu
  QWidget *num_menu = new QWidget(window->draw_area);
  QLabel *numb_p = new QLabel("Number of points",num_menu);
   QPushButton *al1 = new QPushButton(buf,window->draw_area);
   QPushButton *al2 = new QPushButton(buf+5,window->draw_area);
 // QLineEdit *delta_p= new QLineEdit(num_menu);
  //delta_p->setFocusPolicy(Qt::ClickFocus);
  //QObject::connect(delta_p,SIGNAL(textChanged(const QString &)),window->draw_area,SLOT(change_numb_of_p(const QString &))); 
  
  QObject::connect(al1,SIGNAL(clicked()),window->draw_area,SLOT(act_al1()));
  QObject::connect(al2,SIGNAL(clicked()),window->draw_area,SLOT(act_al2()));
  
  //number of point layout
 // QVBoxLayout *numb_layout = new QVBoxLayout;
  QHBoxLayout *hlimlayont = new QHBoxLayout;
  hlimlayont->addWidget(al1);
  hlimlayont->addWidget(al2);
  
  QVBoxLayout *numb_layout = new QVBoxLayout;
  //QGridLayout *reslay = new QGridLayout;
  //reslay->addWidget(num_menu,1,1);
  numb_layout->addWidget(numb_p);
  //numb_layout->addWidget(delta_p);
  numb_layout->addLayout(hlimlayont);
  num_menu->setLayout(numb_layout);
  num_menu->setStyleSheet("background-color: #F0FF87");
  
  //разметка для виджетов
  QGridLayout *rlayont = new QGridLayout;
  rlayont->setRowStretch(0,1);
  rlayont->setColumnStretch(0,1);
  rlayont->setRowMinimumHeight(1,70);
  rlayont->setColumnMinimumWidth(1,300);
  rlayont->addWidget(num_menu,1,1);
  window->draw_area->setLayout(rlayont);
  
 
 window->setMenuBar (tool_bar);
    //начальные размеры окна с графикой
  window->resize (1000, 1000);
    //отобразить окно с графикой
  //draw_area.show ();
  window->show();
    //запуск приложения (начало ожидания событий: нажатие кнопок, resize и т.д.)
  app.exec ();
    //удаление окна при завершении приложения
  delete window;
  return 0;
}
