#include "Window.h"
#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"
class Application
{
public:
    Application();
    ~Application();

    void run();

private:
    void init();
    void update(float dt);
    void render();
    void shutdown();

    /*void initTriangle();*/
    void drawScene();

private:
    bool m_running;
    Window m_window;

   /* unsigned int m_VAO = 0;
    unsigned int m_VBO = 0;*/

    Shader* m_shader = nullptr;
    Camera* m_camera = nullptr;
    Mesh* m_cube = nullptr;

};
