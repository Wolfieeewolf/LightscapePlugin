#include "effects/game/GameEffectBase.h"

GameEffectBase::GameEffectBase(SpatialGrid* grid, const QString& name)
    : grid(grid)
    , name(name)
    , finished(false)
{
}

void GameEffectBase::updateGridColor(const GridPosition& pos, const RGBColor& color)
{
    if (!grid->HasAssignments(pos))
        return;

    auto assignments = grid->GetAssignments(pos);
    for (int i = 0; i < assignments.size(); i++)
    {
        grid->UpdateAssignmentColor(pos, i, color);
    }
}
