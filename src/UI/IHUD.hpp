#pragma once

class IHUD
{
public:
    virtual ~IHUD() = default;

    virtual void SetVisible(bool visible) = 0;
    virtual bool IsVisible() const = 0;
};
