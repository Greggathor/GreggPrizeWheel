#ifndef MyDrawNode_hpp
#define MyDrawNode_hpp

#include <stdio.h>
#include "cocos2d.h"

using namespace cocos2d;

class MyDrawNode :public Node
{
    bool isDrawn;
public:
    static Node* createNode();

    virtual bool init();
    virtual void draw(Renderer* renderer, const Mat4& transform, uint32_t flags);

    CREATE_FUNC(MyDrawNode); // Make the create function
};

#endif /* MyDrawNode_hpp */