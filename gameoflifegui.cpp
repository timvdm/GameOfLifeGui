#include "gameoflifegui.h"
#include "ui_gameoflifegui.h"

#include <QGraphicsRectItem>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QDebug>

GameOfLifeGui::GameOfLifeGui(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameOfLifeGui),
    m_game(1, 1)
{
    ui->setupUi(this);

    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(open()));
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(updateBoard()));
}

GameOfLifeGui::~GameOfLifeGui()
{
    delete ui;
}

bool GameOfLifeGui::color(int i, int j) const
{
  // MODIFY:
  //
  // change this line to return the color for cell i,j (e.g. return m_game.get(i, j);)
  return m_game(i, j);
}

void GameOfLifeGui::setColor(int i, int j, bool value)
{
  qDebug() << "setColor(" << i << ", " << j << ")";
  // MODIFY:
  //
  // change this line to set the color for cell i,j (e.g. m_game.set(i, j, value);)
  m_game(i, j) = value;
}

void GameOfLifeGui::fromString(const std::string &board)
{
  // convert string to 2d vector of bools
  std::vector<std::vector<bool> > brd;
  std::stringstream ss(board);
  std::string line;
  while (std::getline(ss, line)) {
    brd.resize(brd.size() + 1);
    for (std::size_t i = 0; i < line.size(); ++i) 
      if (line[i] == '.')
        brd.back().push_back(false);
      else if (line[i] == 'X')
        brd.back().push_back(true);
      else
        throw std::runtime_error("Invalid character in board, boards contain only '.' and 'X'.");
  }

  qDebug() << board.c_str();

  // set the colors
  if (brd.size()) {
    std::size_t rows = brd.size();
    std::size_t cols = brd[0].size();
    m_game = GameOfLife(rows, cols);

    qDebug() << "rows: " << m_game.rows();
    qDebug() << "cols: " << m_game.cols();
    for (std::size_t i = 0; i < rows; ++i)
      for (std::size_t j = 0; j < cols; ++j)
        setColor(i, j, brd[i][j]);
  }
}


void GameOfLifeGui::open()
{
  // ask user for a file to open
  QString filename = QFileDialog::getOpenFileName(this);
  open(filename);
}

void GameOfLifeGui::open(const QString &filename)
{
  // open the file
  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QMessageBox::warning(this, "Error", "Could not open file.");
    return;
  }

  // read the file contents
  QByteArray bytes = file.readAll();
  std::string board = QString(bytes).toStdString();
  // load board
  fromString(board);
  // initialize the scene
  initializeBoard();
}

void GameOfLifeGui::initializeBoard()
{
  // create scene if there is none
  if (!ui->graphicsView->scene())
    ui->graphicsView->setScene(new QGraphicsScene);

  // clear the scene
  QGraphicsScene *scene = ui->graphicsView->scene();
  scene->clear();

  // resize m_cells
  m_cells.resize(m_game.rows());
  for (int i = 0; i < m_cells.size(); ++i)
    m_cells[i].resize(m_game.cols());

  // create cells
  for (std::size_t i = 0; i < m_game.rows(); ++i)
    for (std::size_t j = 0; j < m_game.cols(); ++j) {
      QBrush brush(color(i, j) ? "black" : "white");
      QGraphicsRectItem *rect = scene->addRect(i * 10, j * 10, 10, 10, QPen("gray"), brush);
      m_cells[i][j] = rect;
    }

  // resize view to fit board
  double ratio = static_cast<double>(ui->graphicsView->width()) / ui->graphicsView->height();
  ui->graphicsView->fitInView(-50, -50, (m_game.rows() * 10 + 50) * ratio, m_game.cols() * 10 + 50);

  // start timer
  m_timer.start(1000);
}

void GameOfLifeGui::updateBoard()
{
  // update the colors
  m_game.next();
  for (std::size_t i = 0; i < m_game.rows(); ++i)
    for (std::size_t j = 0; j < m_game.cols(); ++j) {
      QBrush brush(color(i, j) ? "black" : "white");
      m_cells[i][j]->setBrush(brush);
    }
  m_timer.start(1000);
}
