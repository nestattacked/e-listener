#ifndef ELISTENER_H
#define ELISTENER_H

#include<QWidget>
#include<QPushButton>
#include<QVideoWidget>
#include<QLineEdit>
#include<QPlainTextEdit>
#include<QMediaPlayer>
#include<QVector>
#include<QTimer>
#include"question.h"
#include<QLabel>

class EListener : public QWidget
{
    Q_OBJECT

public:
    EListener(QWidget *parent = 0);
    ~EListener();
public slots:
    void openFile();
    void play();
    void next();
    void answer();
    void checkAnswer();
    void changeDiff();
private:
    void getQuestion();
    qint64 strToInt(const QString &str);
    QPlainTextEdit *text;
    QLineEdit *input_box;
    QVideoWidget *video_widget;
    QPushButton *play_button;
    QPushButton *next_button;
    QPushButton *answer_button;
    QPushButton *open_button;
    QPushButton *diff_button;
    QMediaPlayer *player;
    QVector<Question> *questions;
    QTimer *timer;
    QLabel *message,*correct_mes,*incorrect_mes,*percent_mes;
    int present_question_index;
    int hits,correct,incorrect,difficulty;
};

#endif // ELISTENER_H
