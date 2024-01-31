/* Class MainWindow
 * ----------
 * COMP.CS.110 FALL 2023
 * ----------
 * Class describing the main window.
 *
 * This class is created from stracth to make working game inc. GUI.
 *
 * Program author
 * Name: Abdullah Sagir
 * Student number: 151945467
 * UserID: nxabsa
 * E-Mail: abdulla.sagir@tuni.fi
 *
 * */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "gameengine.hh"
#include <QMainWindow>
#include <QTimer>
#include <QLineEdit>
#include <QLabel>

const QString fileName = "scoreBoard.txt";


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// gui class
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum class GuiResult;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pb_start_clicked();

    void on_pb_quit_clicked();

    void on_pb_roll_clicked();

    void on_pb_giveTurn_clicked();

    void on_spinBox_editingFinished();

    void on_pb_pause_clicked();

    void on_pb_newGame_clicked();
// to get a signal if diceanimation is finished or not. If I don't add this after last roll the information is not updated and animation does not happen.
signals:
    void diceAnimationComplete();


private:


    GuiResult setDiceBackground(uint8_t diceNo, uint8_t value, const std::function<void()>& onAnimationComplete);
    GuiResult setDiceBackground(const std::vector<int>& dices, const std::function<void()>& onAnimationComplete);
    void updateInfos() const;
    void updateScoreBoard() const;
    void clearLockCheckBoxes() const;
    void enableLockCheckBoxes() const;
    void disableLockCheckBoxes() const;



    std::vector<int> lockDicesStatus();
    void gameOver();
    GuiResult enableNameInputs(int count);
    std::vector<std::string> getNames(int count) const;
    void disableNameInputs();
    void saveWinner(const std::vector<Player> &data);
    void reset();

    //void animateDice(QLabel* diceLabel, const QString& diceBackground);
    void animateDice(QLabel* diceLabel, const QString& finalDiceBackground, const std::function<void()>& onAnimationComplete);

    //this will track as boolean if the game is over or not
    bool gameIsOver;



    Ui::MainWindow *ui;
    GameEngine eng;
    QTimer timeCounter;
    vector<QLineEdit*> names;
    std::vector<int> diceValues;
};

enum class MainWindow::GuiResult
{
    NO_ERROR,
    INVALID_PARAMETER,

};

#endif // MAINWINDOW_H
