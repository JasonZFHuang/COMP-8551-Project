#include <iostream>
//#include "RenderTutorial.h"
#include <vector>
#include <string>
#include <ctime>
#include <ratio>
#include <chrono>
#include <string>
#include "Renderer.h"
#include "PhysicsWorld.h"
#include "EntityCoordinator.h"
#include "TimerComponent.h"
#include "Types.h"
#include "Animator.h"
#include "InputTracker.h"
#include "InputComponent.h"
#include "inputSystem.h";
#include "GameEntityCreator.h"
#include "Components.h"
#include "Tags.h"

//ChunkManager* chunkManager;
EntityCoordinator* coordinator;

Renderer* renderer = Renderer::getInstance();
PhysicsWorld* physicsWorld;
Animator animator;
Archetype standardArch;

// test entities
//EntityID background;
EntityID roach;
EntityID wall;
EntityID mike;

using Clock = std::chrono::high_resolution_clock;
using Duration = std::chrono::duration<double, std::milli>;

Clock::time_point prevTime;
double catchupTime;
const double MS_PER_FRAME = (1.0 / 60.0) * 1000;

const int VIEW_WIDTH = 4;
const int VIEW_HEIGHT = 4;

// gets called once when engine starts
// put initilization code here
int initialize()
{  
    // when the engine starts
    renderer->init(VIEW_WIDTH, VIEW_HEIGHT);
    coordinator = &(EntityCoordinator::getInstance());

    physicsWorld = new PhysicsWorld();

    prevTime = Clock::now();

    return 0;
}

int test(){

    coordinator->RegisterComponent<Transform>();
    coordinator->RegisterComponent<RenderComponent>();
    coordinator->RegisterComponent<PhysicsComponent>();
    coordinator->RegisterComponent<AnimationComponent>();
    coordinator->RegisterComponent<TimerComponent>();

    //standardArch = coordinator->GetArchetype({
    //    coordinator->GetComponentType<Transform>(),
    //    coordinator->GetComponentType<RenderComponent>(),
    //    coordinator->GetComponentType<PhysicsComponent>(),
    //    coordinator->GetComponentType<AnimationComponent>()
    //    });

    //coordinator->addSystem<InputSystem>(coordinator);    
    //coordinator->addSystem(std::make_shared<InputSystem>());

    shared_ptr<InputSystem> inputSys = coordinator->addSystem<InputSystem>();


    //Equivalent to attaching code below
    shared_ptr<TestSystem> testSys = coordinator->addSystem<TestSystem>();
    shared_ptr<PrinterSystem> printerSys = coordinator->addSystem<PrinterSystem>();
    testSys.get()->Attach(printerSys.get());
    

    //coordinator->addSystem<TestSystem>().get()->Attach(coordinator->addSystem<PrinterSystem>().get());

    return 0;
}

// Use for now to make entities with components
//EntityID CreateStandardEntity(const char* spriteName) {
//    EntityID e = coordinator->CreateEntity(standardArch, {}, spriteName);
//
//
//    // data must be initialized
//    // if you know the data is going to be initialized later, you don't need to initialize it here
//    Transform t = Transform();
//    coordinator->GetComponent<Transform>(e) = t;
//
//    return e;
//}

// the main update function
// game loop will be put here eventually
int runEngine()

{
    Clock::time_point currTime = Clock::now();
    Duration delta = currTime - prevTime;
    prevTime = currTime;
    catchupTime += delta.count();

    // check input

    while (catchupTime >= MS_PER_FRAME)
    {
        InputTracker::getInstance().perFrameUpdate(window);

        // run physics
        physicsWorld->Update(coordinator);
        // run ECS systems
        coordinator->runSystemUpdates();

        catchupTime -= MS_PER_FRAME;
    }

    if (InputTracker::getInstance().isKeyJustDown(InputTracker::A)) {
        Animation* anim = renderer->getAnimation("running", coordinator->GetComponent<RenderComponent>(mike).spriteName);
        coordinator->GetComponent<RenderComponent>(mike).flipX = false;
        coordinator->GetComponent<AnimationComponent>(mike).currAnim = anim;
    }
    if (InputTracker::getInstance().isKeyJustDown(InputTracker::D)) {
        Animation* anim = renderer->getAnimation("running", coordinator->GetComponent<RenderComponent>(mike).spriteName);
        coordinator->GetComponent<RenderComponent>(mike).flipX = true;
        coordinator->GetComponent<AnimationComponent>(mike).currAnim = anim;
    }
    if (InputTracker::getInstance().isKeyJustDown(InputTracker::S)) {
        Animation* anim = renderer->getAnimation("hurt", coordinator->GetComponent<RenderComponent>(mike).spriteName);
        coordinator->GetComponent<AnimationComponent>(mike).currAnim = anim;
    }

    //animation
    animator.updateAnim(coordinator);

    // render
    renderer->update(coordinator);

    return 0;
}


// gets called once when engine ends
// put teardown code here
int teardown()
{
    // when the engine closes
    renderer->teardown();

    delete physicsWorld;

    return 0;
}

int main() {
    initialize();
    test();

    //entity test
    GameEntityCreator& creator = GameEntityCreator::getInstance();

    //background = creator.CreateScenery(0, 1, 1, 1, "background.png", { Tag::SCENERY });
    roach = creator.CreateActor(0.5, 3, 0.4, 0.4, "Giant_Roach.png", { Tag::ENEMY }, false);
    wall = creator.CreatePlatform(0, -1, 2, 1, "wall.jpg", { Tag:: PLATFORM });
    mike = creator.CreateActor(-0.5, 0, 1,1, "Edgar.png", { Tag::PLAYER }, true);

    coordinator->GetComponent<AnimationComponent>(mike) = Animator::createAnimationComponent(renderer->getAnimation("idle", "Edgar.png"),true);

    animator = Animator();
        
    //std::cout << "turtle " << coordinator->GetComponent<Transform>(roach) << std::endl;
    //std::cout << "wall " << coordinator->GetComponent<Transform>(wall) << std::endl;
    //std::cout << "Dude " << coordinator->GetComponent<Transform>(mike) << std::endl;
    //    
    //std::cout << "From Component array: x: " << coordinator->GetComponent<Transform>(roach).getPosition().x << std::endl;
    //std::cout << "Number of Entities: " << coordinator->GetEntityCount() << std::endl;

    //bool isdudeplayer = coordinator->entityHasTag(Tag::PLAYER,mike);
    //std::cout << "Is dude the player? " << isdudeplayer << std::endl;

    //bool isturtleplayer = coordinator->entityHasTag(Tag::PLAYER, roach);
    //std::cout << "Is turtle the player? " << isturtleplayer << std::endl;

    physicsWorld->AddObjects(coordinator);

    while (!glfwWindowShouldClose(window))
    {
        // tell glfw to keep track of window resize 
        // and input events
        glfwPollEvents();        
        runEngine();        
    }    

    teardown();

    return 0;
}
