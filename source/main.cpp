#include <core/application.h>
#include <core/components/actor.h>
#include <core/components/mesh.h>
#include <core/components/text.h>
#include <core/components/pointlight.h>
#include <core/components/cube.h>
#include <core/components/grid.h>

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

    components.Add(camera);
    components.Add(cursor);
    components.Add(text);
}

void PauseScene::Update()
{
    cursor->x = input.Mouse.x;
    cursor->y = input.Mouse.y;
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
    Text *health;
    bool dir;
    bool isCrouching;
    bool up;
    bool jumping;
    float startY;
    float jumpforce;

    class Bullet : public Component
    {
    public:
        Bullet(bool dir_, bool up_, glm::vec3 pos_)
        {
            if (dir_)
            {
                dir = 1.0f;
            }
            else
            {
                dir = -1.0f;
            }

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
    camera = new Camera(glm::vec3(0, 3, 15), glm::vec3(0.0, 1.0, 0.0), 0, 0, 0);
    cursor = new Sprite("data/cursor.png");
    health = new Text("100");
    protagonist = new Actor();
    protagonist->Add(new Mesh("data/king.blend",
                    "data/phong.vert",
                    "data/phong.frag"));
    protagonist->Uniform("colour", glm::vec4(0.4, 0.7, 0.4, 1.0));
    protagonist->matrix.Translate(glm::vec3(0,-7,-10));
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

    Grid level(32, 32);

    unsigned int CUBE = 1;

    level.At(0,0) = CUBE;
    level.At(1,0) = CUBE;
    level.At(2,0) = CUBE;
    level.At(3,0) = CUBE;
    level.At(4,0) = CUBE;
    level.At(5,0) = CUBE;
    level.At(6,0) = CUBE;
    level.At(7,0) = CUBE;
    level.At(8,0) = CUBE;
    level.At(9,0) = CUBE;
    level.At(10,0) = CUBE;
    level.At(11,0) = CUBE;
    level.At(12,0) = CUBE;
    level.At(13,0) = CUBE;
    level.At(14,0) = CUBE;
    level.At(15,0) = CUBE;
    level.At(16,0) = CUBE;
    level.At(17,0) = CUBE;
    level.At(18,0) = CUBE;
    level.At(19,0) = CUBE;
    level.At(20,0) = CUBE;

    level.At(0,1) = CUBE;
    level.At(1,1) = CUBE;
    level.At(2,1) = CUBE;
    level.At(3,1) = CUBE;
    level.At(4,1) = CUBE;
    level.At(5,1) = CUBE;
    level.At(6,1) = CUBE;
    level.At(7,1) = CUBE;
    level.At(8,1) = CUBE;

    level.At(0,2) = CUBE;
    level.At(1,2) = CUBE;

    level.At(0,3) = CUBE;
    level.At(1,3) = CUBE;

    level.At(1,4) = CUBE;
    level.At(2,4) = CUBE;

    for (unsigned int i = 0; i < level.width; i++)
    {
        for (unsigned int j = 0; j < level.height; j++)
        {
            if (level.At(i, j) == CUBE)
            {
                float x = float(i) * 2.0f;
                float y = float(j) * 2.0f;

                x -= 10.f;
                y -= 10.f;
                
                Cube* cube = new Cube(x, y, -10);
                cube->Uniform("colour", glm::vec4(0.9, 0.7, 0.4, 1.0));
                cube->Uniform("u_lightPosition", static_cast<glm::vec3>(light->position));
                cube->Uniform("u_cameraPosition", static_cast<glm::vec3>(camera->position));
                components.Add(cube);
            }
        }
    }

    dir = true;
    isCrouching = false;
    jumping = false;
    up = false;
    startY = protagonist->matrix.matrix[3].y;
}

void FirstScene::Update()
{
    cursor->x = input.Mouse.x;
    cursor->y = input.Mouse.y;

    room->Uniform("u_lightPosition", static_cast<glm::vec3>(light->position));
    room->Uniform("u_cameraPosition", static_cast<glm::vec3>(camera->position));
    protagonist->Uniform("u_lightPosition", static_cast<glm::vec3>(light->position));
    protagonist->Uniform("u_cameraPosition", static_cast<glm::vec3>(camera->position));

    if (dir)
    {
        pawn->matrix.Translate(glm::vec3(.09 * deltaTime, 0.0, 0.0));
    }
    else
    {
        pawn->matrix.Translate(glm::vec3(-.09 * deltaTime, 0.0, 0.0));
    }

    if (input.Held(input.Key.SHIFT) || input.Held(input.Key.S))
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
            jumpforce = 0.08f;
        }
    }

    if (jumping)
    {
        protagonist->matrix.Translate(glm::vec3(0, jumpforce * deltaTime, 0));
        jumpforce -= 0.006f;

        if (protagonist->matrix.position.y < startY)
        {
            jumping = false;
            protagonist->matrix.position.y = startY;
        }
    }

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
        protagonist->matrix.Translate(glm::vec3(-.02 * deltaTime, 0.0, 0.0));
        dir = false;
    }
    if (input.Held(input.Key.D) || input.Held(input.Key.RIGHT))
    {
        protagonist->matrix.Translate(glm::vec3(.02 * deltaTime, 0.0, 0.0));
        dir = true;
    }
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
}

void FirstScene::UpdateAfterPhysics()
{
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
    if (input.Pressed(input.Key.P))
    {
        paused = !paused;
    }
    else if (input.Pressed(input.Key.Q) || input.Pressed(input.Key.ESCAPE))
    {
        Application::Quit();
    }

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

