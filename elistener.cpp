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
#include<QTime>
#include<QTimer>

EListener::EListener(QWidget *parent)
    : QWidget(parent){
    //srand by current time
    QTime time = QTime::currentTime();
    qsrand(time.msec()+1000*time.second());

    //initialize widget
    text = new QPlainTextEdit;
    input_box = new QLineEdit;
    video_widget = new QVideoWidget;
    play_button = new QPushButton;
    next_button = new QPushButton;
    answer_button= new QPushButton;
    open_button = new QPushButton;
    diff_button = new QPushButton;
    player = new QMediaPlayer;
    questions = new QVector<Question>;
    timer = new QTimer;
    message = new QLabel("Welcom here!");
    correct_mes = new QLabel;
    incorrect_mes = new QLabel;
    percent_mes = new QLabel;

    player->setVolume(100);
    //initialize timer
    timer->setSingleShot(true);
    //set palyer output
    player->setVideoOutput(video_widget);
    //set ui size
    setFixedSize(600,600);
    video_widget->setFixedHeight(350);
    //set widget to enabled state
    text->setReadOnly(true);
    input_box->setReadOnly(true);
    play_button->setEnabled(false);
    next_button->setEnabled(false);
    answer_button->setEnabled(false);
    diff_button->setEnabled(false);
    //set button's icon
    play_button->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    open_button->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
    next_button->setIcon(style()->standardIcon(QStyle::SP_ArrowRight));
    answer_button->setIcon(style()->standardIcon(QStyle::SP_DialogApplyButton));
    diff_button->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
    //set button's shortcut
    open_button->setShortcut(QKeySequence(Qt::Key_F1));
    play_button->setShortcut(QKeySequence(Qt::Key_F2));
    next_button->setShortcut(QKeySequence(Qt::Key_F3));
    answer_button->setShortcut(QKeySequence(Qt::Key_F4));
    diff_button->setShortcut(QKeySequence(Qt::Key_F5));
    //set layout
    QHBoxLayout *control_layout = new QHBoxLayout;
    control_layout->addWidget(open_button);
    control_layout->addWidget(play_button);
    control_layout->addWidget(next_button);
    control_layout->addWidget(answer_button);
    control_layout->addWidget(diff_button);
    QHBoxLayout *label_layout = new QHBoxLayout;
    label_layout->addWidget(message);
    label_layout->addWidget(correct_mes);
    label_layout->addWidget(incorrect_mes);
    label_layout->addWidget(percent_mes);

    QVBoxLayout *main_layout = new QVBoxLayout;
    main_layout->addWidget(video_widget);
    main_layout->addLayout(label_layout);
    main_layout->addWidget(text);
    main_layout->addWidget(input_box);
    main_layout->addLayout(control_layout);
    setLayout(main_layout);

    //initialize connections
    connect(open_button,SIGNAL(clicked()),this,SLOT(openFile()));
    connect(play_button,SIGNAL(clicked()),this,SLOT(play()));
    connect(next_button,SIGNAL(clicked()),this,SLOT(next()));
    connect(answer_button,SIGNAL(clicked()),this,SLOT(answer()));
    connect(input_box,SIGNAL(returnPressed()),this,SLOT(checkAnswer()));
    connect(diff_button,SIGNAL(clicked()),this,SLOT(changeDiff()));
}

EListener::~EListener(){
    delete text;
    delete input_box;
    delete video_widget;
    delete play_button;
    delete next_button;
    delete answer_button;
    delete open_button;
    delete diff_button;
    delete player;
    delete questions;
    delete timer;
    delete message;
    delete correct_mes;
    delete incorrect_mes;
    delete percent_mes;
}

void EListener::openFile(){
    QString file_name = QFileDialog::getOpenFileName(this, tr("Open Movie"),QDir::homePath());

    if (!file_name.isEmpty()) {
        hits=0;
        correct=0;
        incorrect=0;
        difficulty=0;
        message->setText(QString("hits:")+QString::number(hits));
        correct_mes->setText(QString("correct:")+QString::number(correct));
        incorrect_mes->setText(QString("incorrect:")+QString::number(incorrect));
        percent_mes->setText(QString("percent:0"));
        questions->clear();
        player->setMedia(QUrl::fromLocalFile(file_name));
        play_button->setEnabled(true);
        next_button->setEnabled(true);
        answer_button->setEnabled(true);
        diff_button->setEnabled(true);
        input_box->setReadOnly(false);
        file_name.append("s");
        QFile words_file(file_name);
        if(words_file.open(QIODevice::Text|QIODevice::ReadOnly)){
            while(!words_file.atEnd()){
                Question q;
                QString start = words_file.readLine();
                QString end = words_file.readLine();
                //set question's time
                q.start_time = strToInt(start);
                q.end_time = strToInt(end);
                //set question's words
                q.words = words_file.readLine();
                q.words.chop(1);

                questions->append(q);
            }
            words_file.close();
            getQuestion();
        }
    }
}

void EListener::play(){
    timer->stop();
    qint64 begin = questions->at(present_question_index).start_time;
    begin = begin-800<0?0:begin-800;
    player->setPosition(begin);
    qint64 duration = questions->at(present_question_index).end_time - questions->at(present_question_index).start_time + 800;
    connect(timer,SIGNAL(timeout()),player,SLOT(pause()));
    timer->start(duration);
    player->play();
}

void EListener::next(){
    getQuestion();
    play();
}

void EListener::answer(){
    text->setPlainText(questions->at(present_question_index).words);
}

void EListener::checkAnswer(){
    QString answer_word = input_box->text();
    if(difficulty==0){
        QString hint = text->toPlainText();
        hint.replace("___",answer_word);
        answer_word = hint;
    }
    if(answer_word == questions->at(present_question_index).words){
        hits++;
        correct++;
        answer();
    }
    else{
        hits=0;
        incorrect++;
        play();
    }
    message->setText(QString("hits:")+QString::number(hits));
    correct_mes->setText(QString("correct:")+QString::number(correct));
    incorrect_mes->setText(QString("incorrect:")+QString::number(incorrect));
    percent_mes->setText(QString("percent:")+QString::number(((double)correct)/(incorrect+correct)));
    input_box->clear();
}

void EListener::getQuestion(){
    present_question_index = qrand()%(questions->size());
    if(difficulty==0){
        QString hint = questions->at(present_question_index).words;
        QStringList words = hint.split(" ");
        QString word = words[qrand()%(words.size())];
        hint.replace(word,QString("___"));
        text->setPlainText(hint);
    }
    else{
        text->clear();
    }
}

void EListener::changeDiff(){
    difficulty = (!difficulty);
}

qint64 EListener::strToInt(const QString &str){
    qint64 res = 0;
    res+=(3600000*(str.mid(0,2).toInt()));
    res+=(60000*(str.mid(3,2).toInt()));
    res+=(1000*(str.mid(6,2).toInt()));
    res+=(str.mid(9,3).toInt());
    return res;
}
