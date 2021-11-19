#pragma once
#include "EntityCoordinator.h"
#include "Components.h"

class GameEntityCreator
{
private:
    Archetype actorArchetype;
    Archetype platformArchetype;
    Archetype testArchetype;
    Archetype textArchetype;
    //Archetype sceneryArchetype;
    GameEntityCreator();
    RenderComponent standardRenderComponent(const char* spriteName, bool hasAnimation);
public:
    static GameEntityCreator& getInstance();
    EntityID CreateActor(float xPos, float yPos, float scaleX, float scaleY, const char* spriteName, std::vector<Tag> tags, bool hasAnimation, int state);
    EntityID CreatePlatform(float xPos, float yPos, float scaleX, float scaleY, const char* spriteName, std::vector<Tag> tags, int state);
    EntityID CreateTimer(const char* spriteName, std::vector<Tag> tags);
    EntityID CreateText(std::string text, float x, float y, float r, float g, float b, float size, std::vector<Tag> tags);
    //EntityID CreateScenery(float xPos, float yPos, float scaleX, float scaleY, const char* spriteName, std::vector<Tag> tags);
};
