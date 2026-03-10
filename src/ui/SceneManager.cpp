#include "SceneManager.h"

SceneManager::SceneManager(QWidget *parent)
    : QMainWindow(parent)
    , m_stack(new QStackedWidget(this))
{
    setCentralWidget(m_stack);
}

void SceneManager::registerScene(SceneId id, QWidget *widget)
{
    int index = m_stack->addWidget(widget);
    m_sceneIndexMap[id] = index;
}

void SceneManager::switchTo(SceneId id)
{
    if (m_sceneIndexMap.contains(id)) {
        m_stack->setCurrentIndex(m_sceneIndexMap[id]);
    }
}
