#ifndef GAMEOFLIFEGUI_H
#define GAMEOFLIFEGUI_H

#include <QMainWindow>
#include <QTimer>

#include "gameoflife.h"

class QGraphicsRectItem;

namespace Ui {
    class GameOfLifeGui;
}

class GameOfLifeGui : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameOfLifeGui(QWidget *parent = 0);
    ~GameOfLifeGui();

  void initializeBoard();
  bool color(int i, int j) const;
  void setColor(int i, int j, bool value);
  void fromString(const std::string &board);
  void open(const QString &filename);

public Q_SLOTS:
  void open();
  void updateBoard();

private:
    Ui::GameOfLifeGui *ui;
    GameOfLife m_game;
    QVector<QVector<QGraphicsRectItem*> > m_cells;
    QTimer m_timer;
};

#endif // GAMEOFLIFEGUI_H
