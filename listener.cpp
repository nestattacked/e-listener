#include "elistener.h"
#include<QHBoxLayout>
#include<QPushButton>
#include<QPlainTextEdit>
#include<QLineEdit>
#include<QVideoWidget>
#include<QMediaPlayer>
#include<QVector>
#include<QStyle>
#include<QSizePolicy>
#include"question.h"
#include<QString>
#include<QFileDialog>
#include<QDir>
#include<QFile>
#include<QDebug>
#include<QTime>
#include<QTimer>

EListener::EListener(QWidget *parent)
    : QWidget(parent){
    QTime time = QTime::currentTime();
    qsrand(time.msec()+1000*time.second());

    text = new QPlainTextEdit();
    input_box = new QLineEdit;
    video_widget = new QVideoWidget;
    play_button = new QPushButton;
    next_button = new QPushButton;
    answer_button= new QPushButton;
    open_button = new QPushButton;
    player = new QMediaPlayer;
    questions = new QVector<Question>;
    timer = new QTimer;

    timer->setSingleShot(true);
    player->setVideoOutput(video_widget);
    setFixedSize(600,600);
    video_widget->setFixedHeight(400);
    text->setReadOnly(true);
    play_button->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    open_button->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
    next_button->setIcon(style()->standardIcon(QStyle::SP_ArrowRight));
    answer_button->setIcon(style()->standardIcon(QStyle::SP_DialogApplyButton));
    play_button->setEnabled(false);
    next_button->setEnabled(false);
    answer_button->setEnabled(false);
    input_box->setReadOnly(true);

    QHBoxLayout *control_layout = new QHBoxLayout;
    control_layout->addWidget(open_button);
    control_layout->addWidget(play_button);
    control_layout->addWidget(next_button);
    control_layout->addWidget(answer_button);

    QVBoxLayout *main_layout = new QVBoxLayout;
    main_layout->addWidget(video_widget);
    main_layout->addWidget(text);
    main_layout->addWidget(input_box);
    main_layout->addLayout(control_layout);
    setLayout(main_layout);

    connect(open_button,SIGNAL(clicked()),this,SLOT(openFile()));
    connect(play_button,SIGNAL(clicked()),this,SLOT(play()));
    connect(next_button,SIGNAL(clicked()),this,SLOT(next()));
    connect(answer_button,SIGNAL(clicked()),this,SLOT(answer()));
    connect(input_box,SIGNAL(returnPressed()),this,SLOT(checkAnswer()));
}

EListener::~EListener(){
    delete text;
    delete input_box;
    delete video_widget;
    delete play_button;
    delete next_button;
    delete answer_button;
    delete open_button;
    delete player;
    delete questions;
}

void EListener::openFile(){
    QString file_name = QFileDialog::getOpenFileName(this, tr("Open Movie"),QDir::homePath());

    if (!file_name.isEmpty()) {
        questions->clear();
        player->setMedia(QUrl::fromLocalFile(file_name));
        play_button->setEnabled(true);
        next_button->setEnabled(true);
        answer_button->setEnabled(true);
        input_box->setReadOnly(false);
        file_name.append("s");
        QFile words_file(file_name);
        if(words_file.open(QIODevice::Text|QIODevice::ReadOnly)){
            while(!words_file.atEnd()){
                QString start = words_file.readLine();
                QString end = words_file.readLine();
                QString words = words_file.readLine();
                qint64 start_int = strToInt(start);
                qint64 end_int = strToInt(end);
                Question q(start_int, end_int, words);
                questions->append(q);
            }
        }
        present_question_index = qrand()%(questions->size());
        QString hint = questions->at(present_question_index).words;
        QStringList words = hint.split(" ");
        QString word = words[qrand()%(words.size())];
        hint.replace(word,QString("___"));
        text->setPlainText(hint);
    }
}

void EListener::play(){
    timer->stop();
    player->setPosition(questions->at(present_question_index).start_time);
    player->play();
    //QString hint = questions->at(present_question_index).words;
    //QStringList words = hint.split(" ");
    //QString word = words[qrand()%(words.size())];
    //hint.replace(word,QString("___"));
    //text->setPlainText(hint);
    qint64 duration = questions->at(present_question_index).end_time - questions->at(present_question_index).start_time;
    connect(timer,SIGNAL(timeout()),player,SLOT(pause()));
    timer->start(duration);
}

void EListener::next(){
    present_question_index = qrand()%(questions->size());
    QString hint = questions->at(present_question_index).words;
    QStringList words = hint.split(" ");
    QString word = words[qrand()%(words.size())];
    hint.replace(word,QString("___"));
    text->setPlainText(hint);
    play();
}

void EListener::answer(){
    text->setPlainText(questions->at(present_question_index).words);
}

void EListener::checkAnswer(){
    QString hint = text->toPlainText();
    QString answer_word = input_box->text();
    hint.replace("___",answer_word);
    if(hint == questions->at(present_question_index).words){
        answer();
    }
    else{
        play();
    }
    input_box->clear();
}

qint64 EListener::strToInt(const QString &str){
    qint64 res = 0;
    res+=(3600000*(str.mid(0,2).toInt()));
    res+=(60000*(str.mid(3,2).toInt()));
    res+=(1000*(str.mid(6,2).toInt()));
    res+=(str.mid(9,3).toInt());
    return res;
}
