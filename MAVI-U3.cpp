#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <vector>

const float SCALE = 30.f;
const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

sf::Vector2f toSFMLPos(const b2Vec2& pos) 
{
    return sf::Vector2f(pos.x * SCALE, WINDOW_HEIGHT - pos.y * SCALE);
}

int main() 
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Ejercicio 1");
    window.setFramerateLimit(60);

    b2World world(b2Vec2(0.f, -10.f));

    //"Bordes"

    b2BodyDef wallDef;
    wallDef.position.Set(0, 0);
    b2Body* wall = world.CreateBody(&wallDef);

    b2EdgeShape edge;
    b2FixtureDef edgeFixture;
    edgeFixture.shape = &edge;

    //Piso

    edge.SetTwoSided(b2Vec2(0, 0), b2Vec2(WINDOW_WIDTH / SCALE, 0));
    wall->CreateFixture(&edgeFixture);

    //Techo

    edge.SetTwoSided(b2Vec2(0, WINDOW_HEIGHT / SCALE), b2Vec2(WINDOW_WIDTH / SCALE, WINDOW_HEIGHT / SCALE));
    wall->CreateFixture(&edgeFixture);

   //Izquierda

    edge.SetTwoSided(b2Vec2(0, 0), b2Vec2(0, WINDOW_HEIGHT / SCALE));
    wall->CreateFixture(&edgeFixture);

    //Derecha

    edge.SetTwoSided(b2Vec2(WINDOW_WIDTH / SCALE, 0), b2Vec2(WINDOW_WIDTH / SCALE, WINDOW_HEIGHT / SCALE));
    wall->CreateFixture(&edgeFixture);

    //Pelota

    b2BodyDef ballDef;
    ballDef.type = b2_dynamicBody;
    ballDef.position.Set(400.f / SCALE, 300.f / SCALE);
    b2Body* ball = world.CreateBody(&ballDef);

    ball->SetLinearVelocity(b2Vec2(8.f, -8.f));

    b2CircleShape circle;
    circle.m_radius = 15.f / SCALE;

    b2FixtureDef ballFixture;
    ballFixture.shape = &circle;
    ballFixture.density = 1.0f;
    ballFixture.restitution = 0.9f;
    ball->CreateFixture(&ballFixture);

    sf::CircleShape ballShape(15.f);
    ballShape.setFillColor(sf::Color::Cyan);
    ballShape.setOrigin(15.f, 15.f);

    while (window.isOpen()) 
    {
        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

        world.Step(1.f / 60.f, 6, 2);
        b2Vec2 pos = ball->GetPosition();

        ballShape.setPosition(toSFMLPos(pos));

        window.clear();
        window.draw(ballShape);
        window.display();
    }

    return 0;
}