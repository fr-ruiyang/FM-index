#include "widget.h"
#include "ui_widget.h"
#include <QIcon>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTime>
#include <string>
#include "fm_index.h"
#define M 2
#define D -1
#define DI -1

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle(tr("FM-index.exe"));
    setWindowIcon(QIcon(":/new/prefix1/huanglai.jpeg"));
}

Widget::~Widget()
{
    delete ui;
}

//全局变量
int *SA=NULL;
string T,S;

int max_flag(int a,int b,int c)
{
    if (c>=a&&c>=b)
        return 3;
    else if (a>=b&&a>=c)
        return 1;
    else
        return 2;
}

void Widget::on_pushButton_clicked()//浏览
{
    QString filename;
    filename=QFileDialog::getOpenFileName(this,
                                          tr("选择文件"),
                                          "",
                                          tr("所有文件(*.*)"));

    if(filename.isEmpty())
    {
         return;
    }
    else
    {
        ui->lineEdit->setText(filename);
        QFile file(filename);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) //加载文件
        {
            QMessageBox::warning(this,
                                     tr("警告"),
                                     tr("打开文件失败!"));
            return;
        }
        QString data=QString(file.readAll());
        data=data.toLower();
        T=data.toStdString();
        //除去非法字符
        for (unsigned int i=0;i<T.length();i++)
        {
            if ((T[i]!='a'&&T[i]!='c'&&T[i]!='g'&&T[i]!='t'&&T[i]!='u'&&T[i]!='A'&&T[i]!='C'&&T[i]!='G'&&T[i]!='T'&&T[i]!='U'))
            {
                T=T.erase(i,1);
                i--;
            }
        }
        SA=new int[T.length()+1];
        QTime time;
        time.start();
        S=fm_index::BWT(T,SA);
        int time_Diff = time.elapsed();
        float f = time_Diff/1000.0;

//        ui->textBrowser->append("源序列如下：");
        ui->textBrowser->setText(QString::fromStdString(T));
//        ui->textBrowser->append(QString("%1%2").arg("序列总长度为").arg(T.length()));

        ui->textBrowser->append(QString("%1%2%3").arg("耗时").arg(f).arg("s"));
//        ui->textBrowser->append(QString("%1").arg("**************分割线****************"));
    }
}

void Widget::on_pushButton_2_clicked()//匹配
{
    QTime time;
    time.start();
    QString temp = ui->lineEdit_2->text();
    temp=temp.toLower();
    string P=temp.toStdString();
    int n=P.length();
    if (!P.length())
    {
        QMessageBox::warning(this,
                                 tr("警告"),
                                 tr("请输入查找序列!"));
        return;
    }
    for (unsigned int i=0;i<P.length();i++)
    {
        if (P[i]!='a'&&P[i]!='c'&&P[i]!='g'&&P[i]!='t'&&P[i]!='u'&&P[i]!='A'&&P[i]!='C'&&P[i]!='G'&&P[i]!='T'&&P[i]!='U')
        {
            QMessageBox::warning(this,
                                     tr("警告"),
                                     tr("输入序列中有非法字符，请重新输入!\n提示：仅能含有a、c、g、t、u这5个字母（大小写均可）!"));
            return;
        }
    }
    int sp,ep;
    int index = fm_index::exactmatch(S,P,sp,ep);
    QTextCursor cursor = ui->textBrowser->textCursor();
    cursor.movePosition(QTextCursor::Start);
    cursor.movePosition( QTextCursor::End, QTextCursor::KeepAnchor);
    ui->textBrowser->setTextCursor( cursor );
    QTextCharFormat defcharfmt = ui->textBrowser->currentCharFormat();
    QTextCharFormat newcharfmt = defcharfmt;
    newcharfmt.setBackground(QColor(Qt::white));
    newcharfmt.setFontWeight(QFont::Normal);
    ui->textBrowser->setCurrentCharFormat( newcharfmt );
    if (index==-1)
    {
        for (int j=sp;j<ep;j++){
            QTextCursor cursor = ui->textBrowser->textCursor();
            cursor.movePosition(QTextCursor::Start);
            cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, SA[j]);
            cursor.movePosition( QTextCursor::NextCharacter, QTextCursor::KeepAnchor ,P.length());
            ui->textBrowser->setTextCursor( cursor );
            QTextCharFormat defcharfmt = ui->textBrowser->currentCharFormat();
            QTextCharFormat newcharfmt = defcharfmt;
            newcharfmt.setBackground(QColor(Qt::yellow));
            newcharfmt.setFontWeight(QFont::DemiBold);
            ui->textBrowser->setCurrentCharFormat( newcharfmt );
            ui->textBrowser->append(QString("%1%2%3").arg("查找序列").arg("在源序列中的位置为").arg(SA[j]));
        }
    }
    else
    {
        if (P.length()<35)
            Approximate_match(temp,QString::fromStdString(T));
        else
        {
            ui->textBrowser->append(QString("%1%2%3").arg("未匹配到全部序列，匹配度为").arg(100-index*100.0/P.length()).arg("%."));
        }
    }
    int time_Diff = time.elapsed();
    float f = time_Diff/1000.0;
//    ui->textBrowser->append(QString("%1%2%3").arg("耗时").arg(f).arg("s"));
    ui->textBrowser->append(QString("%1").arg("**************分割线****************"));
}

void Widget::Approximate_match(QString s1,QString s2)
{
    string s,t;
    if (s1.length()<=s2.length())
        s=s1.toStdString(),t=s2.toStdString();
    else
        s=s2.toStdString(),t=s1.toStdString();
//    ui->textBrowser->append(tr("长串为："));
//    ui->textBrowser->append(QString::fromStdString(t));
//    ui->textBrowser->append(tr("短串为："));
//    ui->textBrowser->append(QString::fromStdString(s));
    int m=s.length(),n=t.length();
    int matrix[m+1][n+1],flag[m+1][n+1];
    int i,j,max=0,max_s,max_t;
    for (i=0;i<=m;i++)
    {
        matrix[i][0]=0;
        flag[i][0]=0;
    }
    for (j=0;j<=n;j++)
    {
        matrix[0][j]=0;
        flag[0][j]=0;
    }
    for (i=1;i<=m;i++)
        for (j=1;j<=n;j++)
        {
            int r1=matrix[i][j-1]+DI;
            int r2=matrix[i-1][j]+DI;
            int r3=matrix[i-1][j-1];
            if (s.at(i-1)!=t.at(j-1)) r3=r3+D;
            else r3=r3+M;
            flag[i][j]=max_flag(r1,r2,r3);
            switch (flag[i][j]) {
            case 1:
                matrix[i][j]=r1;
                break;
            case 2:
                matrix[i][j]=r2;
                break;
            case 3:
                matrix[i][j]=r3;
                break;
            default:
                break;
            }
            if (matrix[i][j]<0)
            {
                matrix[i][j]=0;
                flag[i][j]=0;
            }
            if (matrix[i][j]>max)
            {
                max=matrix[i][j];
                max_s=i;
                max_t=j;
            }
        }

//    for (i=0;i<=m;i++)
//    {
//        for (j=0;j<=n;j++)
//            cout<<matrix[i][j]<<" ";
//        cout<<endl;
//    }

    i=max_s;j=max_t;
    vector <int> op;
    while (flag[i][j]!=0)
    {
        op.push_back(flag[i][j]);
        switch (op.back()) {
        case 1:
            j--;
            break;
        case 2:
            i--;
            break;
        case 3:
            i--;
            j--;
            break;
        default:
            break;
        }
    }
    int min_s=i,min_t=j;
    int k=i,l=j;
    string p=s,q=t;
    while (!op.empty())
    {
        switch (op.back()) {
        case 1:
            p.insert(k,1,'_');
            break;
        case 2:
            q.insert(l,1,'_');
            break;
        default:
            break;
        }
        k++;
        l++;
        op.pop_back();
    }
//    p.insert(0,min_t-min_s,' ');
    q=q.substr(min_t-min_s,p.length());
    ui->textBrowser->append(tr("模糊匹配结果为："));
    ui->textBrowser->append(QString("%1%2").arg("在源序列中的位置为").arg(min_t-min_s));
    ui->textBrowser->append(QString::fromStdString(q));
    ui->textBrowser->append(QString::fromStdString(p));
}


void Widget::on_textBrowser_textChanged()
{
    ui->textBrowser->moveCursor(QTextCursor::End);
}
