#ifndef MAINMENUSCENE_H
#define MAINMENUSCENE_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

/**
 * @brief 主菜单场景
 *
 * 游戏启动时显示的初始界面，提供开始游戏和退出选项。
 */
class MainMenuScene : public QWidget
{
    Q_OBJECT

public:
    explicit MainMenuScene(QWidget *parent = nullptr);

signals:
    void startGameClicked();
    void exitGameClicked();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPushButton *m_startBtn;
    QPushButton *m_exitBtn;
};

#endif // MAINMENUSCENE_H
