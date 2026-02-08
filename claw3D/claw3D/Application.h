#pragma once

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

    bool m_running;
};
