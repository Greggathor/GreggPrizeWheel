#include "MyDrawNode.h"


Node* MyDrawNode::createNode()
{
    return MyDrawNode::create(); // Create an instance of this class and return the pointer
}

bool MyDrawNode::init()
{
    if (!Node::init())
    {
        return false;
    }

    isDrawn = false;

    // Screen coordinates
    auto visibleSize = Director::getInstance()->getVisibleSize(); // Screen's visible size
    Vec2 origin = Director::getInstance()->getVisibleOrigin();    // Screen's visible origin

    // Calculating the center of the screen
    float cx = origin.x + visibleSize.width * 0.5;  // X for the center of the screen
    float cy = origin.y + visibleSize.height * 0.5; // Y for the center of the screen

    setPosition(cx, cy);

    scheduleUpdate(); //Activate the update

    return true;
}

void MyDrawNode::update(float delta)
{
    auto position = getPosition();  // Get the current position of self
    position.x += (50 * delta);     // Increase the current position by 50 units per second
    setPosition(position);          // Set the position to the newly increased value
    auto rotation = getRotation();  // Get the current rotation of self
    rotation += 1;                  // Increase the current rotation
    setRotation(rotation);          // Set the rotation to the newly increased value
}

void MyDrawNode::draw(Renderer* renderer, const Mat4& transform, uint32_t flags)
{
    if (!isDrawn)
    {
        auto draw = DrawNode::create(); // Create a drawNode object that draws simple shapes
        // Draw a rectangle
        draw->drawLine(Vec2(-100, -100), Vec2(-100, +100), Color4F(1.0, 1.0, 1.0, 1.0));
        draw->drawLine(Vec2(+100, -100), Vec2(+100, +100), Color4F(1.0, 1.0, 1.0, 1.0));
        draw->drawLine(Vec2(-100, -100), Vec2(+100, -100), Color4F(1.0, 1.0, 1.0, 1.0));
        draw->drawLine(Vec2(-100, +100), Vec2(+100, +100), Color4F(1.0, 1.0, 1.0, 1.0));
        addChild(draw);   // Parenting the drawn object to this node
        isDrawn = true;   // Should stop drawing on every frame
    }
}