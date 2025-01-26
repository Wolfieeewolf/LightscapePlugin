#include "ui/EffectManagerDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

EffectManagerDialog::EffectManagerDialog(EffectsEngine* engine, QWidget *parent)
    : QDialog(parent)
    , effectsEngine(engine)
{
    setupUi();
}

void EffectManagerDialog::setupUi()
{
    setWindowTitle(tr("Effect Manager"));
    
    auto mainLayout = new QVBoxLayout(this);
    
    effectList = new QListWidget(this);
    mainLayout->addWidget(effectList);
    
    auto buttonLayout = new QHBoxLayout();
    
    addButton = new QPushButton(tr("Add Effect"), this);
    removeButton = new QPushButton(tr("Remove Effect"), this);
    removeButton->setEnabled(false);
    
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(removeButton);
    
    mainLayout->addLayout(buttonLayout);
    
    connect(addButton, &QPushButton::clicked, this, &EffectManagerDialog::onAddEffect);
    connect(removeButton, &QPushButton::clicked, this, &EffectManagerDialog::onRemoveEffect);
    connect(effectList, &QListWidget::itemSelectionChanged, this, &EffectManagerDialog::onEffectSelected);
}

void EffectManagerDialog::onAddEffect()
{
    // TODO: Implement effect creation dialog
}

void EffectManagerDialog::onRemoveEffect()
{
    auto item = effectList->currentItem();
    if (item) {
        QString effectName = item->text();
        effectsEngine->removeEffect(effectName);
        delete item;
    }
}

void EffectManagerDialog::onEffectSelected()
{
    removeButton->setEnabled(effectList->currentItem() != nullptr);
}