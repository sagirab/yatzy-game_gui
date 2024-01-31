/* Class MainWindow
 * ----------
 * COMP.CS.110 FALL 2023
 * ----------
 * Class functions describing the main window.
 *
 * Program author
 * Name: Abdullah Sagir
 * Student number: 151945467
 * UserID: nxabsa
 * E-Mail: abdulla.sagir@tuni.fi
 *
 * */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gameengine.hh"
#include <QDebug>
#include <QDateTime>
#include <QLineEdit>
#include <QFile>

#include <QTimer>
#include <QThread>
#include <QRandomGenerator>

#include <stdlib.h>

#include <chrono>
#include <thread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    ,gameIsOver(false)
    , ui(new Ui::MainWindow)
    ,diceValues(NUMBER_OF_DICES, 0)
{
    ui->setupUi(this);

    for (auto &child : ui->scrollAreaWidgetContents->children()) {
        if (auto current = qobject_cast<QLineEdit*>(child))
            names.push_back(current);
    }
    QObject::connect(this, &MainWindow::diceAnimationComplete, this, &MainWindow::updateScoreBoard);

    QObject::connect(&timeCounter, &QTimer::timeout, [=](){

        if (eng.is_game_over())
        {
            eng.reset();
        }

        ui->timeEdit->setTime(ui->timeEdit->time().addSecs(1));


    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

MainWindow::GuiResult MainWindow::setDiceBackground(uint8_t diceNo, uint8_t value, const std::function<void()>& onAnimationComplete)
{
    if (diceNo > NUMBER_OF_DICES || value > MAX_DICE_VALUE)
        return GuiResult::INVALID_PARAMETER;

    QString diceBackground;

    if (value)
        diceBackground = "background-image: url(:/dicepictures/" + QString::number(value) + ".png);";
    else
        diceBackground = "background-image: url(:/dicepictures/empty.png);";

    QLabel* diceLabel = nullptr;

    switch (diceNo)
    {
        case 1:
            diceLabel = ui->dice_1;
            break;

        case 2:
            diceLabel = ui->dice_2;
            break;

        case 3:
            diceLabel = ui->dice_3;
            break;

        case 4:
            diceLabel = ui->dice_4;
            break;

        case 5:
            diceLabel = ui->dice_5;
            break;

        default:
            return GuiResult::INVALID_PARAMETER;
    }

    // Update the locked property of the dice label
    diceLabel->setProperty("isLocked", lockDicesStatus()[diceNo - 1]);

    // Call the modified animateDice function with the onAnimationComplete callback
    animateDice(diceLabel, diceBackground, onAnimationComplete);



    return GuiResult::NO_ERROR;
}


void MainWindow::animateDice(QLabel* diceLabel, const QString& finalDiceBackground, const std::function<void()>& onAnimationComplete)
{
    // Create a timer for animation
    QTimer* timer = new QTimer(this);

    // Set the interval for the timer (adjust the interval as needed)
    int animationInterval = 20; // 50 milliseconds **** DO NOT CHANGE ** IF YOU CHANGE GO AND CHANGE THE MILISECONDS VALUE IN GAMEOVER FUNC

    // Set up a counter to keep track of the number of animation steps
    int animationSteps = 0;

    // Retrieve the initial locked state
    bool isLocked = diceLabel->property("isLocked").toBool();

    // Connect the timer's timeout signal to a lambda function
    connect(timer, &QTimer::timeout, [=]() mutable {
        // Check if we've reached the desired number of animation steps
        if (animationSteps >= 5) { // Change the number of steps for a 2-second animation **** DO NOT CHANGE   ** IF YOU CHANGE GO AND CHANGE THE MILISECONDS VALUE IN GAMEOVER FUNC
            // Stop the timer and clean up
            timer->stop();
            timer->deleteLater();

            // Set the final dice background
            diceLabel->setStyleSheet(finalDiceBackground);

            // Execute the callback function
            onAnimationComplete();
            return;
        }

        // Increment the animation step counter
        animationSteps++;

        // If the die is locked, do not change its value during animation
        if (!isLocked) {
            // Generate a random dice value for the animation step
            int randomValue = QRandomGenerator::global()->bounded(1, 7); // Random number between 1 and 6

            // Construct the file path for the random dice image
            QString randomDiceBackground = "background-image: url(:/dicepictures/" + QString::number(randomValue) + ".png);";

            // Update the dice label with the current random dice background
            diceLabel->setStyleSheet(randomDiceBackground);
        }

        // Introduce a delay to create animation effect
        QThread::msleep(animationInterval);
    });

    // Start the timer
    timer->start();

}


MainWindow::GuiResult MainWindow::setDiceBackground(const std::vector<int>& dices, const std::function<void()>& onAnimationComplete)
{
    GuiResult result = GuiResult::NO_ERROR;

    diceValues = dices;

    if (dices.size() != NUMBER_OF_DICES)
        result = GuiResult::INVALID_PARAMETER;

    if (result == GuiResult::NO_ERROR)
    {
        for (int var = 0; var < NUMBER_OF_DICES; ++var)
        {
            result = setDiceBackground(var + 1, dices.at(var), onAnimationComplete);

            if (result != GuiResult::NO_ERROR)
                break;
        }
    }

    return result;
}

void MainWindow::updateInfos() const
{
    ui->info->setText(QString(eng.currentPlayer().name.c_str()) + " in turn, " + QString::number(eng.currentPlayer().rolls_left_) + " trials left !");
}

void MainWindow::updateScoreBoard() const
{
    ui->scoreBoard->clear();

    for (int var = 0; var < eng.playerCount(); ++var)
    {
        auto player = eng.playerInfo(var);
        auto temp = QString(player.name.c_str());

        //qDebug() << "Player Info - Name: " << player.name.c_str() << " Points: " << player.best_point_values_;

        std::string result_text;
        construe_result(player.latest_point_values_, result_text);

        temp.append(" " + QString(result_text.c_str()));
        //std::this_thread::sleep_for(std::chrono::milliseconds(100));
        ui->scoreBoard->append(temp);

    }

}

void MainWindow::clearLockCheckBoxes() const
{
    ui->lock_1->setCheckState(Qt::Unchecked);
    ui->lock_2->setCheckState(Qt::Unchecked);
    ui->lock_3->setCheckState(Qt::Unchecked);
    ui->lock_4->setCheckState(Qt::Unchecked);
    ui->lock_5->setCheckState(Qt::Unchecked);
}

void MainWindow::enableLockCheckBoxes() const
{
    ui->lock_1->setEnabled(true);
    ui->lock_2->setEnabled(true);
    ui->lock_3->setEnabled(true);
    ui->lock_4->setEnabled(true);
    ui->lock_5->setEnabled(true);
}

void MainWindow::disableLockCheckBoxes() const
{
    ui->lock_1->setDisabled(true);
    ui->lock_2->setDisabled(true);
    ui->lock_3->setDisabled(true);
    ui->lock_4->setDisabled(true);
    ui->lock_5->setDisabled(true);
}

std::vector<int> MainWindow::lockDicesStatus()
{
    std::vector<int> lock;

    lock.push_back(ui->lock_1->checkState());
    lock.push_back(ui->lock_2->checkState());
    lock.push_back(ui->lock_3->checkState());
    lock.push_back(ui->lock_4->checkState());
    lock.push_back(ui->lock_5->checkState());

    return lock;
}

void MainWindow::gameOver()
{
    //std::this_thread::sleep_for(std::chrono::milliseconds(2500)); // 250 = animationInterval×animationSteps coming from animateDice function

    std::vector<int> winners;
    std::vector<Player> winnerPlayers;

    auto result = eng.report_winner(winners);

    qDebug() << "result: " << RESULT_TEXTS.at(result).c_str();

    if (winners.size() > 1)
    {
        QString temp = "Result is tie between players ";

        for (const auto &val : winners){
            auto player = eng.playerInfo(val - 1);
            temp.append(QString(player.name.c_str()) + " ");
            winnerPlayers.push_back(player);
        }
        temp.append(" --> " + QString(RESULT_TEXTS.at(result).c_str()));

        ui->info->setText(temp);
    }
    else
    {
        auto player = eng.playerInfo(winners.front() - 1);
        ui->info->setText(QString(player.name.c_str()) + " is the winner --> " + QString(RESULT_TEXTS.at(result).c_str()));
        winnerPlayers.push_back(player);
    }

    saveWinner(winnerPlayers);

    //reset();    //this should not be active. when its active at the end of the game scoreboard gets inactive leading unreadeble info
    ui->pb_roll->setEnabled(false);
    ui->pb_giveTurn->setDisabled(true);
    ui->pb_pause->setDisabled(true);
    on_pb_pause_clicked();


    ui->centralwidget->setStyleSheet("background-color:lightblue ");
    gameIsOver = true;

    ui->pb_giveTurn->setEnabled(false);



}

MainWindow::GuiResult MainWindow::enableNameInputs(int count)
{
    if (count > 9)
        return MainWindow::GuiResult::INVALID_PARAMETER;

    for (auto & instance : names)
    {
        instance->setDisabled(true);
        auto number = instance->objectName().back().toLatin1() - 0x30;

        if (number <= count)
            instance->setEnabled(true);
    }

    return MainWindow::GuiResult::NO_ERROR;
}

std::vector<string> MainWindow::getNames(int count) const
{
    std::vector<string> vec;

    for (auto & instance : names)
    {
        auto number = instance->objectName().back().toLatin1() - 0x30;
        if (number <= count)
            vec.push_back(instance->text().toStdString());
    }
    return vec;
}

void MainWindow::disableNameInputs()
{
    for (auto & instance : names) {
        instance->clear();
        instance->setDisabled(true);
    }
}

void MainWindow::saveWinner(const std::vector<Player> &data)
{
    QFile file(fileName);

    if (file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        for (const auto& element : data)
        {
            QString toWrt = QDateTime::currentDateTime().toString() + "-- " + QString(element.name.c_str()) + "-- " + "--> winner points : ";

            for (auto val : element.best_point_values_)
                toWrt.append(QString::number(val) + " ");
            toWrt.append("\n");

            file.write(toWrt.toUtf8());
        }
        file.write("\n");

        file.close();
    }
    else
    {
        qDebug() << "Error: Unable to open the file for writing.";
    }
}

void MainWindow::reset()
{
    timeCounter.stop();

    ui->spinBox->setEnabled(true);
    ui->pb_pause->setDisabled(true);
    ui->pb_roll->setDisabled(true);
    ui->pb_giveTurn->setDisabled(true);
    ui->pb_quit->setDisabled(true);
    ui->scoreBoard->clear();
    //setDiceBackground(std::vector<int>(NUMBER_OF_DICES,0));
    clearLockCheckBoxes();
    eng.reset();
}


void MainWindow::on_pb_start_clicked()
{
    auto count = ui->spinBox->value();
    auto nameVec = getNames(count);

    /*for(int i = 0; i < count; ++i)
    {
        Player player = {i + 1, INITIAL_NUMBER_OF_ROLLS, {}, {}, nameVec.at(i)};
        eng.add_player(player);
    }*/

    for (int i = 0; i < count; ++i)
    {
        Player player = {static_cast<unsigned int>(i + 1), INITIAL_NUMBER_OF_ROLLS, {}, {}, nameVec.at(i)};
        eng.add_player(player);
    }

    ui->spinBox->setDisabled(true);
    ui->pb_start->setDisabled(true);
    ui->pb_roll->setEnabled(true);
    ui->pb_pause->setEnabled(true);
    ui->pb_giveTurn->setEnabled(true);
    ui->pb_newGame->setEnabled(true);
    ui->pb_quit->setEnabled(true);
    disableNameInputs();
    updateInfos();
    ui->pauseInfo->clear();
    timeCounter.start(1000);

}



void MainWindow::on_pb_quit_clicked()
{
    if (gameIsOver)
    {
        close();  // Close the GUI if the game is over --- a.k.a = if the user clicks 2nd time to QUIT then close the GUI
    }
    else
    {
        gameOver();
    }
}


void MainWindow::on_pb_roll_clicked()
{
    ui->pb_giveTurn->setDisabled(false);
    std::pair<std::vector<int>, Result> report;

    std::vector<int> vals = diceValues;

    enableLockCheckBoxes();

    auto val = eng.roll(vals, lockDicesStatus(), report);

    if (val == GameEngine::Status::PROGRESSING)
    {

        updateInfos();
        setDiceBackground(vals, [&]() {
            // Animation is complete
            if (eng.currentPlayer().rolls_left_ == 0) {
                ui->pb_roll->setDisabled(true);
            }
            updateScoreBoard();
            if (val == GameEngine::Status::DONE) {
                gameOver();
            }
        });
    }
    else if (val == GameEngine::Status::DONE)
    {
        setDiceBackground(vals, [&]() {
            // Animation is complete
            if (eng.currentPlayer().rolls_left_ == 0) {
                ui->pb_roll->setDisabled(true);
            }
            updateScoreBoard();
            if (val == GameEngine::Status::DONE) {
                gameOver();
            }
        });

        updateScoreBoard();
        gameOver();
    }

}




void MainWindow::on_pb_giveTurn_clicked()
{
    ui->pb_giveTurn->setDisabled(true);
    clearLockCheckBoxes();
    disableLockCheckBoxes();

    //setDiceBackground(std::vector<int>(NUMBER_OF_DICES, 0));
    if(eng.currentPlayer().rolls_left_ == 0){
        ui->pb_roll->setDisabled(false);
    }

    eng.give_turn();
    updateInfos();

}


void MainWindow::on_spinBox_editingFinished()
{
    enableNameInputs(ui->spinBox->value());
}


void MainWindow::on_pb_pause_clicked()
{
    static bool buttonState = true;

    if (buttonState)
    {
        timeCounter.stop();
        ui->pauseInfo->setText("Paused");
        ui->pb_pause->setText("CONTINUE");
        ui->pb_roll->setDisabled(true);
        ui->pb_giveTurn->setDisabled(true);
    }
    else
    {
        timeCounter.start();
        ui->pauseInfo->clear();
        ui->pb_pause->setText("PAUSE");
        ui->pb_roll->setEnabled(true);
        ui->pb_giveTurn->setEnabled(true);
    }

    buttonState ^= true;
}


void MainWindow::on_pb_newGame_clicked()
{
    ui->scrollAreaWidgetContents->setStyleSheet("background-color:"); // I added these color to prevent the screen color staying blue after clicking the new game.
    ui->centralwidget->setStyleSheet("background-color:");


    ui->centralwidget->setStyleSheet("");
    ui->timeEdit->setTime(QTime(0,0));
    ui->info->clear();
    ui->pb_start->setEnabled(true);
    reset();

    ui->pb_newGame->setDisabled(true);
    ui->pb_giveTurn->setDisabled(true);
    ui->pb_pause->setDisabled(true);



}

