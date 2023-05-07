#include <core/application.h>
#include <core/components/actor.h>
#include <core/components/mesh.h>
#include <core/components/text.h>

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
    camera = new Camera();
    cursor = new Sprite("data/cursor.png");
    protagonist = new Actor();
//    protagonist->Add(new Mesh("data/protagonist.blender"));
    components.Add(camera);
    components.Add(cursor);
    components.Add(protagonist);
}

void FirstScene::Update()
{
    cursor->x = input.Mouse.x;
    cursor->y = input.Mouse.y;

    if (input.Held(input.Key.A))
    {
        protagonist->matrix.Translate(glm::vec3(1.0, 0.0, 0.0));
        Log("Left");
    }
    if (input.Held(input.Key.D))
    {
        protagonist->matrix.Translate(glm::vec3(-1.0, 0.0, 0.0));
        Log("Right");
    }
    if (input.Held(input.Key.W))
    {
        protagonist->matrix.Translate(glm::vec3(0.0, 0.0, 1.0));
        Log("Up");
    }
    if (input.Held(input.Key.S))
    {
        protagonist->matrix.Translate(glm::vec3(1.0, 0.0, -1.0));
        Log("Down");
    }
}

void FirstScene::UpdateAfterPhysics()
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
        Log("Next Scene");
    }
}

void SplashScreen::UpdateAfterPhysics()
{
}

int main(int argc, char **argv)
{
    Application application(argc, argv);

    application.AddScene(new PauseScene());
    application.AddScene(new SplashScreen());
    application.AddScene(new FirstScene());

    return application.Exec();
}

