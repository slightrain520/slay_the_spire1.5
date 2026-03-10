#ifndef CHARACTERSELECTSCENE_H
#define CHARACTERSELECTSCENE_H

#include <QWidget>
#include <QPushButton>

/**
 * @brief 角色选择场景
 *
 * 玩家选择战士或女王开始游戏。
 */
class CharacterSelectScene : public QWidget
{
    Q_OBJECT

public:
    explicit CharacterSelectScene(QWidget *parent = nullptr);

signals:
    /** @param index 0=战士, 1=女王 */
    void characterSelected(int index);
    void backClicked();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPushButton *m_warriorBtn;
    QPushButton *m_queenBtn;
    QPushButton *m_backBtn;
};

#endif // CHARACTERSELECTSCENE_H
