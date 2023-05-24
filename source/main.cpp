#include <core/application.h>
#include <core/components/actor.h>
#include <core/components/mesh.h>
#include <core/components/text.h>
#include <core/components/pointlight.h>
#include <core/components/cube.h>
#include <core/components/grid.h>
#include <core/components/fpscounter.h>

class MainMenuScene : public IScene
{  
private:
    Camera *camera;
    Sprite *cursor;
    Text *text;
    Text *option1;
    Text *option2;
    Text *option3;

    Text *selector;

    int option;

public:
    MainMenuScene();
    void Init();
    void Update();
    void UpdateAfterPhysics();
};

MainMenuScene::MainMenuScene()
{
}

void MainMenuScene::Init()
{
    camera = new Camera();
    cursor = new Sprite("data/cursor.png");
    text = new Text("REVIVAL", 500, 100);

    option1 = new Text("START GAME", 500, 300);
    option2 = new Text("FULLSCREEN", 500, 400);
    option3 = new Text("QUIT", 500, 500);

    selector = new Text(">", 470, 300);

    components.Add(camera);
    components.Add(cursor);
    components.Add(text);

    components.Add(option1);
    components.Add(option2);
    components.Add(option3);

    components.Add(selector);

    option = 1;
}

void MainMenuScene::Update()
{
    cursor->x = input.Mouse.x;
    cursor->y = input.Mouse.y;

    if (option > 1)
    {
        if (input.Pressed(input.Key.W) || input.Pressed(input.Key.UP))
        {
            selector->y -= 100;
            option--;
        }
    }
    if (option < 3)
    {
        if (input.Pressed(input.Key.S) || input.Pressed(input.Key.DOWN))
        {
            selector->y += 100;
            option++;
        }
    }

    if (input.Pressed(input.Key.F) || input.Pressed(input.Key.ENTER))
    {
        if (option == 1)
        {
            Application::NextScene();
        }
        else if (option == 2)
        {
            Application::fullscreen = true;
        }
        else if (option == 3)
        {
            Application::Quit();
        }
    }
}

void MainMenuScene::UpdateAfterPhysics()
{
}

class PauseScene : public IScene
{  
private:
    Camera *camera;
    Sprite *cursor;
    Text *text;

    Text *option1;
    Text *option2;
    Text *selector;

    int option;

public:
    PauseScene();
    void Init();
    void Update();
    void UpdateAfterPhysics();
};

PauseScene::PauseScene()
{
}

void PauseScene::Init()
{
    camera = new Camera();
    cursor = new Sprite("data/cursor.png");
    text = new Text("PAUSE", 500, 100);

    option1 = new Text("CONTINUE", 500, 200);
    option2 = new Text("QUIT", 500, 250);
    selector = new Text(">", 470, 200);

    option = 1;

    components.Add(camera);
    components.Add(cursor);
    components.Add(text);

    components.Add(option1);
    components.Add(option2);
    components.Add(selector);
}

void PauseScene::Update()
{
    cursor->x = input.Mouse.x;
    cursor->y = input.Mouse.y;

    if (option == 1)
    {
        if (input.Pressed(input.Key.DOWN) || input.Pressed(input.Key.S))
        {
            selector->y += 50.0f;
            option = 2;
        }
    }
    else
    {
        if (input.Pressed(input.Key.UP) || input.Pressed(input.Key.W))
        {
            selector->y -= 50.0f;
            option = 1;
        }
    }

    if (input.Pressed(input.Key.ENTER))
    {
        if (option == 2)
        {
            Application::Quit();
        }
    }
}

void PauseScene::UpdateAfterPhysics()
{
}

class FirstScene : public IScene
{
private:
    Camera *camera;
    Sprite *cursor;
    PointLight *light;
    Actor *protagonist;
    Mesh *pawn;
    Mesh *room;
    Mesh *gun;
    Mesh *idleMesh;
    Mesh *runningMesh;
    Mesh *scenery;
    Text *health;
    FPSCounter *counter;
    int dir;
    int oldDir;
    bool isCrouching;
    bool up;
    bool jumping;
    bool running;
    float jumpforce;
    float force;

    class Bullet : public Component
    {
    public:
        Bullet(int dir_, bool up_, glm::vec3 pos_)
        {
            dir = dir_;
            up = up_;

            bullet = new Mesh("data/pawn.blend",
                              "data/phong.vert",
                              "data/phong.frag");
            bullet->matrix.matrix[3].x = pos_.x;
            bullet->matrix.matrix[3].y = pos_.y;
            bullet->matrix.matrix[3].z = pos_.z;

            speed = 0.1f;
        }

        void Update()
        {
            if (up)
            {
                bullet->matrix.Translate(glm::vec3(0.0f, speed * deltaTime, 0.0f));
            }
            else
            {
                bullet->matrix.Translate(glm::vec3(dir * speed * deltaTime, 0.0f, 0.0f));
            }
            bullet->Update();
        }

        void UpdateAfterPhysics()
        {
            // TODO: delete bullet when collided
        }

    private:
        Mesh* bullet;
        float dir;
        bool up;
        float speed;
    };

public:
    FirstScene();
    void Init();
    void Update();
    void UpdateAfterPhysics();
};

FirstScene::FirstScene()
{
}

void FirstScene::Init()
{
    camera = new Camera(glm::vec3(0, -1, 0), glm::vec3(0.0, 1.0, 0.0), 0, 0, 0); // (0, 3, 15)
    cursor = new Sprite("data/cursor.png");
    counter = new FPSCounter();
    health = new Text("100", 0,0);
    protagonist = new Actor();

    idleMesh = new Mesh("data/player/player_idle_1frame.blend",
                        "data/vertexanimation.vert",
                        "data/vertexanimation.frag");
    runningMesh = new Mesh("data/player/player_run.blend",
                           "data/vertexanimation.vert",
                           "data/vertexanimation.frag");

    scenery = new Mesh("data/enviroment1/enviroment.blend",
                       "data/vertexanimation.vert",
                       "data/vertexanimation.frag");

    protagonist->Add(idleMesh);
    protagonist->Add(runningMesh);
    protagonist->Uniform("colour", glm::vec4(0.4, 0.7, 0.4, 1.0));
    protagonist->matrix.Translate(glm::vec3(0,-5,-10));
    protagonist->matrix.Rotate(3.141593 * 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    light = new PointLight(glm::vec3(-9.683014, 16.498363, 7.318779));
    gun = new Mesh("data/queen.blend",
                   "data/phong.vert",
                   "data/phong.frag");
    room = new Mesh("data/room.blend",
                    "data/phong.vert",
                    "data/phong.frag");
    room->matrix.Translate(glm::vec3(0,0,-30));
    room->Uniform("colour", glm::vec4(0.9, 0.7, 0.4, 1.0));
    gun->Uniform("colour", glm::vec4(0.4, 0.7, 0.4, 1.0));
    gun->Hide();
    //gun->Rotate(45,0,0);
    pawn = new Mesh("data/pawn.blend");
    pawn->Hide();
    protagonist->Add(gun);
    components.Add(camera);
    components.Add(cursor);
    components.Add(protagonist);
    //components.Add(room);
    components.Add(pawn);
    components.Add(health);
    components.Add(counter);
    //components.Add(scenery);

    Grid level(32, 32);

    enum
    {
        GROUND = 1,
        FLOOR,
        WALL_LEFT,
        WALL_RIGHT
    };

    /*level.At(0,0) = GROUND;
    level.At(1,0) = GROUND;
    level.At(2,0) = GROUND;
    level.At(3,0) = GROUND;
    level.At(4,0) = GROUND;
    level.At(5,0) = GROUND;
    level.At(6,0) = GROUND;
    level.At(7,0) = GROUND;
    level.At(8,0) = GROUND;
    level.At(9,0) = FLOOR;
    level.At(10,0) = FLOOR;
    level.At(11,0) = FLOOR;
    level.At(12,0) = FLOOR;
    level.At(13,0) = FLOOR;
    level.At(14,0) = FLOOR;
    level.At(15,0) = FLOOR;
    level.At(16,0) = FLOOR;
    level.At(17,0) = FLOOR;
    level.At(18,0) = FLOOR;
    level.At(19,0) = FLOOR;
    level.At(20,0) = FLOOR;

    level.At(0,1) = GROUND;
    level.At(1,1) = GROUND;
    level.At(2,1) = FLOOR;
    level.At(3,1) = FLOOR;
    level.At(4,1) = FLOOR;
    level.At(5,1) = FLOOR;
    level.At(6,1) = FLOOR;
    level.At(7,1) = FLOOR;
    level.At(8,1) = WALL_RIGHT;

    level.At(0,2) = GROUND;
    level.At(1,2) = WALL_RIGHT;

    level.At(0,3) = GROUND;
    level.At(1,3) = WALL_RIGHT;

    level.At(1,4) = GROUND;
    level.At(2,4) = WALL_RIGHT;

    level.At(8,4) = GROUND;
    level.At(9,4) = GROUND;*/

    level.At(0,0) = GROUND;
    level.At(1,0) = GROUND;
    level.At(2,0) = GROUND;

    level.At(0,1) = FLOOR;
    level.At(1,1) = FLOOR;
    level.At(2,1) = FLOOR;

    for (unsigned int i = 0; i < level.width; i++)
    {
        for (unsigned int j = 0; j < level.height; j++)
        {
            if (level.At(i, j) == GROUND || level.At(i, j) == WALL_LEFT || level.At(i, j) == WALL_RIGHT)
            {
                float x = float(i) * 2.0f * 5.0f;
                float y = float(j) * 2.0f * 4.0f;

                y -= 10.f;

                Actor* cube = new Actor();
                cube->Add(new Cube(0,0,0, 5,.5,4));
                cube->matrix.Translate(glm::vec3(x, y, -10));
                cube->collisionBox->dimensions *= 4.0f;
                cube->collisionBox->dimensions.x = 5.0f;

                if (level.At(i, j) == GROUND)
                {
                    cube->collisionBox->type = "GROUND";
                }
                else if (level.At(i, j) == WALL_LEFT)
                {
                    cube->collisionBox->type = "WALL_LEFT";
                }
                else if (level.At(i, j) == WALL_RIGHT)
                {
                    cube->collisionBox->type = "WALL_RIGHT";
                }

                cube->Uniform("colour", glm::vec4(0.9, 0.7, 0.4, 1.0));
                cube->Uniform("u_lightPosition", static_cast<glm::vec3>(light->position));
                cube->Uniform("u_cameraPosition", static_cast<glm::vec3>(camera->position));
                components.Add(cube);
            }
            if (level.At(i, j) == FLOOR)
            {
                float x = float(i) * 2.0f * 5.0f;
                float y = float(j) * 2.0f * 5.0f;

                y -= 16.0f;

                Actor* cube = new Actor();
                cube->Add(new Mesh("data/enviroment1/enviroment.blend",
                        "data/simple.vert",
                        "data/simple.frag"));
                cube->matrix.Translate(glm::vec3(x, y, -12));

                cube->collisionBox->type = "FLOOR";
                //cube->collisionBox->dimensions *= 4.0f;

                components.Add(cube);
            }
        }
    }

    dir = 1.0;
    oldDir = 1.0;
    isCrouching = false;
    jumping = false;
    running = false;
    up = false;
    jumpforce = 0.0f;
    force = 0.0f;
}

void FirstScene::Update()
{
    cursor->x = input.Mouse.x;
    cursor->y = input.Mouse.y;

    room->Uniform("u_lightPosition", static_cast<glm::vec3>(light->position));
    room->Uniform("u_cameraPosition", static_cast<glm::vec3>(camera->position));
    protagonist->Uniform("u_lightPosition", static_cast<glm::vec3>(light->position));
    protagonist->Uniform("u_cameraPosition", static_cast<glm::vec3>(camera->position));

    if (input.Held(input.Key.SHIFT) || input.Held(input.Key.S) || input.Held(input.Key.DOWN))
    {
        isCrouching = true;
    }
    else
    {
        isCrouching = false;
    }

    if (input.Held(input.Key.CTRL) || input.Held(input.Key.ALT))
    {
        if (jumping == false)
        {
            jumping = true;
            jumpforce = 0.18f;
        }
    }

    protagonist->matrix.Translate(glm::vec3(0, jumpforce, 0));// * deltaTime, 0));
    jumpforce -= 0.015f;

    running = false;

    if (input.Pressed(input.Key.SPACE) || input.Mouse.Pressed)
    {
        glm::vec3 pos(
                    protagonist->matrix.matrix[3].x,
                    protagonist->matrix.matrix[3].y,
                    protagonist->matrix.matrix[3].z);

        if (isCrouching)
        {
            pos.y -= 3.0f;
        }

        Bullet* newBullet = new Bullet(dir, up, pos);

        components.Add(newBullet);
    }

    if (input.Held(input.Key.A) || input.Held(input.Key.LEFT))
    {
        dir = -1.0;
        running = true;
        force -= 0.05f;
    }

    if (input.Held(input.Key.D) || input.Held(input.Key.RIGHT))
    {
        dir = 1.0;
        running = true;
        force += 0.05f;
    }

    if (running == true)
    {
        if (force > 0.3)
        {
            force = 0.3;
        }
        if (force < -0.3)
        {
            force = -0.3;
        }
    }
    else
    {
        if (force < 0.00001f && force > -0.00001f)
        {
            force = 0.0f;
        }

        if (force > 0)
        {
            force -= 0.05;
        }
        else if (force < 0)
        {
            force += 0.05;
        }
    }

    protagonist->matrix.Translate(glm::vec3(force, 0.0, 0.0));

    if (input.Held(input.Key.W) || input.Held(input.Key.UP))
    {
        up = true;
    }
    else
    {
        up = false;
    }
    if (input.Held(input.Key.S) || input.Held(input.Key.DOWN))
    {
        //protagonist->matrix.Translate(glm::vec3(0.0, 0.0, .02 * deltaTime));
    }

    if (oldDir != dir)
    {
        // Rotate protagonist to face the right way
    }

    oldDir = dir;

    if (running)
    {
        runningMesh->Show();
        idleMesh->Hide();
    }
    else
    {
        runningMesh->Hide();
        idleMesh->Show();
    }
}

void FirstScene::UpdateAfterPhysics()
{
    if (physics->Collide(protagonist->collisionBox, "GROUND"))
    {
        //protagonist->matrix.Translate(glm::vec3(0.0f, physics->Collide(protagonist->collisionBox)->direction.y, 0.0f));
        jumpforce = 0;
        jumping = false;
    }

    if (physics->Collide(protagonist->collisionBox, "FLOOR"))
    {
        //protagonist->matrix.Translate(glm::vec3(0.0f, physics->Collide(protagonist->collisionBox)->direction.y, 0.0f));
        jumpforce = 0;
        jumping = false;
    }

    if (physics->Collide(protagonist->collisionBox, "WALL_LEFT"))
    {
        protagonist->matrix.Translate(glm::vec3(-.02 * deltaTime, 0.0f, 0.0f));
    }

    if (physics->Collide(protagonist->collisionBox, "WALL_RIGHT"))
    {
        protagonist->matrix.Translate(glm::vec3(.02 * deltaTime, 0.0f, 0.0f));
    }

    if (camera->matrix.x > protagonist->matrix.x + 50)
    {
        camera->matrix.position.x++;
    }
    else if (camera->matrix.x < protagonist->matrix.x + 50)
    {
        camera->matrix.position.x--;
    }
}

class Game : public IScene
{
private:
    IScene *pauseScreen;
    IScene *firstScreen;
    IScene *activeScene;
    bool paused;

public:
    Game();
    void Init();
    void Update();
    void UpdateAfterPhysics();
};

Game::Game()
{
}

void Game::Init()
{
    pauseScreen = new PauseScene();
    firstScreen = new FirstScene();

    pauseScreen->Init();
    firstScreen->Init();

    paused = false;
}

void Game::Update()
{
    if (paused)
    {
        activeScene = pauseScreen;
    }
    else
    {
        activeScene = firstScreen;
    }

    // Update game components
    for (unsigned int i = 0; i < activeScene->components.Size(); i++)
    {
        (*activeScene->components[i])->Update();
    }

    activeScene->Update();

    activeScene->UpdateAfterPhysics();

    // Update game components
    for (unsigned int i = 0; i < activeScene->components.Size(); i++)
    {
        (*activeScene->components[i])->UpdateAfterPhysics();
    }

    // Update pause screen (done last to allow update scene to process input)
    if (input.Pressed(input.Key.P) || input.Pressed(input.Key.Q) || input.Pressed(input.Key.ESCAPE))
    {
        paused = !paused;
    }
    if (paused)
    {
        if (input.Pressed(input.Key.F) || input.Pressed(input.Key.ENTER))
        {
            paused = false;
        }
    }
}

void Game::UpdateAfterPhysics()
{
}

class SplashScreen : public IScene
{  
private:
    Sprite *splash;
    Camera *camera;
    Text *text1;
    Text *text2;
    Text *text3;
    Text *text4;
    ITime  *timer;

public:
    SplashScreen();
    void Init();
    void Update();
    void UpdateAfterPhysics();
};

SplashScreen::SplashScreen()
{
}

void SplashScreen::Init()
{
    splash = new Sprite("data/splash.png", 360, 120);
    camera = new Camera(glm::vec3(0, 0, 0), glm::vec3(0.0, 1.0, 0.0), 0, 0, 0);
    text1 = new Text("PLANET EARTH...", 100, 330);
    text2 = new Text("I CALLED THIS PLACE HOME ONCE,", 100, 380);
    text3 = new Text("LONG BEFORE EVIL HAUNTED", 100, 410);
    text4 = new Text("THE PLANES AND WATERS.", 100, 440);
    timer  = Application::GetTime();
    //components.Add(splash);
    components.Add(text1);
    components.Add(text2);
    components.Add(text3);
    components.Add(text4);
    components.Add(camera);

    audio->PlaySound("data/intro.wav");
}

void SplashScreen::Update()
{
    if (int(timer->TimeSinceStarted()) > 3000)
    {
        Application::NextScene();
    }
}

void SplashScreen::UpdateAfterPhysics()
{
}

int main(int argc, char **argv)
{
    Application application(argc, argv);

//    application.AddScene(new MainMenuScene());
//    application.AddScene(new SplashScreen());
    application.AddScene(new Game());

    return application.Exec();
}

