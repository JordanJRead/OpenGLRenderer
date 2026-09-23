#ifndef UI_PROPERTY_BASED
#define UI_PROPERTY_BASED

#include "uiproperties.hpp"

class UIPropertyBased {
public:
    UIPropertyBased(const JSON& propertyJSON);
    void         readOwnProperties();
    virtual void renderUIProperties();
    JSON         toJSON() const;

private:
    UIProperties mUIProperties;
    virtual void readProperties(UIProperties& properties) = 0;
};

#endif