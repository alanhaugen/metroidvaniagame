#include <core/application.h>
#include <core/components/actor.h>
#include <core/components/mesh.h>
#include <core/components/text.h>

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
    text = new Text("METROIDVANIA", 500, 100);

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
        if (input.Pressed(input.Key.W))
        {
            selector->y -= 100;
            option--;
        }
    }
    if (option < 3)
    {
        if (input.Pressed(input.Key.S))
        {
            selector->y += 100;
            option++;
        }
    }

    if (input.Pressed(input.Key.F))
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
    Actor *protagonist;
    Mesh *pawn;
    Mesh *gun;

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
    protagonist = new Actor();
    protagonist->Add(new Mesh("data/king.blend"));
    gun = new Mesh("data/queen.blend");
    //gun->Rotate(45,0,0);
    pawn = new Mesh("data/pawn.blend");
    pawn->Hide();
    protagonist->Add(gun);
    components.Add(camera);
    components.Add(cursor);
    components.Add(protagonist);
    components.Add(pawn);
}

void FirstScene::Update()
{
    cursor->x = input.Mouse.x;
    cursor->y = input.Mouse.y;

    pawn->matrix.Translate(glm::vec3(1.9, 0.0, 0.0));

    if (input.Pressed(input.Key.SPACE) || input.Mouse.Pressed)
    {
        pawn->Show();
        pawn->matrix.matrix[3] = protagonist->matrix.matrix[3];
    }

    if (input.Held(input.Key.A))
    {
        protagonist->matrix.Translate(glm::vec3(-1.0, 0.0, 0.0));
    }
    if (input.Held(input.Key.D))
    {
        protagonist->matrix.Translate(glm::vec3(1.0, 0.0, 0.0));
    }
    if (input.Held(input.Key.W))
    {
        protagonist->matrix.Translate(glm::vec3(0.0, 0.0, -1.0));
    }
    if (input.Held(input.Key.S))
    {
        protagonist->matrix.Translate(glm::vec3(0.0, 0.0, 1.0));
    }
}

void FirstScene::UpdateAfterPhysics()
{
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
    else if (input.Pressed(input.Key.Q))
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
    timer  = Application::GetTime();
    components.Add(splash);
    components.Add(camera);
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

    application.AddScene(new SplashScreen());
    application.AddScene(new MainMenuScene());
    application.AddScene(new Game());

    return application.Exec();
}

